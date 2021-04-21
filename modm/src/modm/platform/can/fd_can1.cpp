/*
 * Copyright (c) 2019, Raphael Lehmann
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <queue>
#include <array>
#include <algorithm>

#include <modm/architecture/driver/atomic/queue.hpp>
#include <modm/platform/clock/rcc.hpp>
#include <modm/architecture/interface/delay.hpp>
#include <modm/architecture/interface/assert.hpp>
#include <modm/architecture/interface/interrupt.hpp>
#include <modm/debug/logger.hpp>
#include <cstring> // std::memcpy

#include "fd_can1.hpp"

#undef  MODM_LOG_LEVEL
#define MODM_LOG_LEVEL modm::log::INFO

static modm::atomic::Queue<modm::can::LongMessage, 32> txQueue;

modm::platform::Fdcan1::Mode modm::platform::Fdcan1::last_set_mode = modm::platform::Fdcan1::Mode::Normal;

struct RxMessage {
    modm::can::LongMessage message;
    uint8_t filter_id;
    uint16_t timestamp;
};
static modm::atomic::Queue<RxMessage, 32> rxQueue;


void modm::platform::Fdcan1::printRegs()
{
	MODM_LOG_INFO << "\nREGS can 1!!!!\n";
	MODM_LOG_INFO << "ECR: " << (uint32_t)(FDCAN1->ECR) << modm::endl;
	MODM_LOG_INFO << "PSR: " << (uint32_t)(FDCAN1->PSR) << modm::endl;
	MODM_LOG_INFO << "TXBTO: " << (uint32_t)(FDCAN1->TXBTO) << modm::endl;
	MODM_LOG_INFO << "CCCR: " << (uint32_t)(FDCAN1->CCCR) << modm::endl;
}

bool modm::platform::Fdcan1::enableProtectedAccess()
{
	FDCAN1->CCCR = FDCAN_CCCR_INIT;
	int deadlockPreventer = 10'000;
	while (((FDCAN1->CCCR & FDCAN_CCCR_INIT) == 0) and (deadlockPreventer-- > 0)) {
		modm::delay_us(1);
		// Wait until the initialization mode is entered.
	}
	if(deadlockPreventer < 0)
		return false;
	FDCAN1->CCCR |= FDCAN_CCCR_CCE;
	return true;

}

void modm::platform::Fdcan1::disableProtectedAccess()
{
	FDCAN1->CCCR &= ~FDCAN_CCCR_INIT;
}


void
modm::platform::Fdcan1::initializeWithPrescaler(
		uint16_t prescaler, uint8_t bs1, uint8_t bs2, uint8_t sjw,
		uint16_t dataPrescaler, uint8_t dataBs1, uint8_t dataBs2, uint8_t dataSjw,
		Preprescaler preprescaler,
		uint32_t interruptPriority, Mode startupMode, bool overwriteOnOverrun)
{
	if(!Rcc::isEnabled<Peripheral::Fdcan1>()) {
		RCC->CCIPR |= RCC_CCIPR_FDCANSEL_1;
		Rcc::enable<Peripheral::Fdcan1>();
	}

	modm_assert_continue_ignore(enableProtectedAccess(), "can.initializeWithPrescaler",
		"deadlockPreventer timed out", 1);
	
	// Configure pre-prescaler (common for all three FDCAN instances)
	// Fixme: TODO!!!!!
	FDCAN_CONFIG->CKDIV = static_cast<uint8_t>(preprescaler);

	// Configure nominal and data bitrates
	FDCAN1->NBTP =
			((sjw - 1) << FDCAN_NBTP_NSJW_Pos) |
			((bs2 - 1) << FDCAN_NBTP_NTSEG2_Pos) |
			((bs1 - 1) << FDCAN_NBTP_NTSEG1_Pos) |
			((prescaler - 1) << FDCAN_NBTP_NBRP_Pos);
	FDCAN1->DBTP =
			((dataSjw - 1) << FDCAN_DBTP_DSJW_Pos) |
			((dataBs2 - 1) << FDCAN_DBTP_DTSEG2_Pos) |
			((dataBs1 - 1) << FDCAN_DBTP_DTSEG1_Pos) |
			((dataPrescaler - 1) << FDCAN_DBTP_DBRP_Pos) /*|
			FDCAN_DBTP_TDC*/; // enable "Transceiver Delay Compensation"


	// Timestamp: FDCAN internal counter with prescaler=1
	// In CAN FD mode the internal timestamp counter TCP does not provide a constant time
	//  base due to the different CAN bit times between arbitration phase and data phase.
	FDCAN1->TSCC = (1 << FDCAN_TSCC_TSS_Pos);

	// Set vector priority
	NVIC_SetPriority(FDCAN1_IT0_IRQn, interruptPriority);
	NVIC_SetPriority(FDCAN1_IT1_IRQn, interruptPriority);

	// Register Interrupts at the NVIC
	NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
	NVIC_EnableIRQ(FDCAN1_IT1_IRQn);

	FDCAN1->ILE = FDCAN_ILE_EINT1 | FDCAN_ILE_EINT0;

	// TODO: assign interrupts to interrupt lines
	FDCAN1->IE = FDCAN_IE_RF0FE | FDCAN_IE_RF0NE | FDCAN_IE_RF1NE | FDCAN_IE_RF1FE
				| FDCAN_IE_TCE | FDCAN_IE_TFEE; // transmit

	// Write on interrupt line 1 and read on iterrupt line 0
	FDCAN1->ILS = FDCAN_ILS_SMSG | FDCAN_ILS_TFERR;

	FDCAN1->CCCR |= FDCAN_CCCR_FDOE | FDCAN_CCCR_BRSE; //FD mode allowed


	FDCAN1->RXGFC = overwriteOnOverrun ? 0 : (FDCAN_RXGFC_F1OM | FDCAN_RXGFC_F0OM); // TODO: or inverted?

	// Reject frames not matching any filter
	FDCAN1->RXGFC |= FDCAN_RXGFC_ANFE | FDCAN_RXGFC_ANFS;

	// Tx buffer: queue mode
	FDCAN1->TXBC = FDCAN_TXBC_TFQM;

	_setMode(startupMode);

	disableProtectedAccess();
}


// Configure the mailbox to send a CAN message.
// Low level function called by sendMessage and by Tx Interrupt.
static bool
sendMsg(const modm::can::LongMessage& message)
{
	using namespace modm::platform;
	//if(!Fdcan1::isReadyToSend()) {
	//	return false;
	//}
	// Retrieve the Tx queue put index
	uint8_t putIndex = (FDCAN1->TXFQS & FDCAN_TXFQS_TFQPI_Msk) >> FDCAN_TXFQS_TFQPI_Pos;


	uint32_t* msgRam = (uint32_t*)(Fdcan1::TxFifo + (putIndex * Fdcan1::TxFifoElementSize));
	*msgRam++ = ((message.isExtended() ? 1 : 0) << 30)
		| ((message.isRemoteTransmitRequest() ? 1 : 0) << 29)
		| (message.isExtended() ? message.getIdentifier() : (message.getIdentifier() << 18));
	*msgRam++ =  ((message.isFlexibleData() ? 1 : 0) << 21) 
		| ((message.isBitRateSwitching() ? 1 : 0) << 20)
		| (message.getDLC() << 16);

	uint8_t length = std::min(message.getLength(), message.getCapacity());

	if(not message.isFlexibleData())
		length = std::min((uint8_t)8, length);

	for(uint8_t i = 0; i < length; i+=4)
	{
		uint32_t data = 0;
		std::memcpy(&data, message.data+i, std::min(4, length - i));
		*msgRam++ = data;
	}

	//memcpy doesnot work and keeps copying same byte for each 32 bit.
	//std::memcpy(msgRam, message.data, std::min(message.getLength(), uint8_t(64)));

	FDCAN1->TXBAR = ((uint32_t)1 << putIndex);
	return true;
}


// Low level function to receive a message from mailbox.
// Called by Rx Interrupt or by getMessage.
static void
readMsg(modm::can::LongMessage& message, uint8_t* filter_id, uint16_t *timestamp, bool fifo0)
{
	using namespace modm::platform;
	uint8_t getIndex;
	uint32_t* msgRam;
	if(fifo0) {
		getIndex = (FDCAN1->RXF0S & FDCAN_RXF0S_F0GI_Msk) >> FDCAN_RXF0S_F0GI_Pos;
		msgRam = (uint32_t*)(Fdcan1::RxFifo0 + (getIndex * Fdcan1::RxFifoElementSize));
	}
	else {
		getIndex = (FDCAN1->RXF1S & FDCAN_RXF1S_F1GI_Msk) >> FDCAN_RXF1S_F1GI_Pos;
		msgRam = (uint32_t*)(Fdcan1::RxFifo1 + (getIndex * Fdcan1::RxFifoElementSize));
	}

	message.setExtended(*msgRam & (1 << 30));
	message.setRemoteTransmitRequest(*msgRam & (1 << 29));
	message.setIdentifier((*msgRam & ((1 << 29) - 1)) >> (message.isExtended() ? 0 : 18));
	
	msgRam++;

	if(filter_id != nullptr) {
		*filter_id = (*msgRam >> 24) & ((1 << 7)-1);
	}
	message.setFlexibleData(*msgRam & (1 << 21));
	message.setBitRateSwitching(*msgRam & (1 << 20));

	if(message.isFlexibleData())
		message.setDLC((*msgRam >> 16) & ((1 << 4)-1));
	else
		message.setLength(std::min((uint8_t)8, (uint8_t)((*msgRam>>16) & ((1 << 4)-1))));
	
	if(timestamp != nullptr) {
		*timestamp = *msgRam & ((1 << 16) -1 );
	}

	msgRam++;

	std::memcpy((uint8_t*)message.data, (uint8_t*)msgRam, std::min(message.getLength(), message.getCapacity()));

	if(fifo0) {
		FDCAN1->RXF0A = getIndex;
	}
	else {
		FDCAN1->RXF1A = getIndex;
	}
}


/* Transmit Interrupt
 *
 */
MODM_ISR(FDCAN1_IT1)
{
	uint32_t ir = FDCAN1->IR;
	uint8_t free_spots = 0;

	if (ir & FDCAN_IR_TC) { // Transmission completed
		FDCAN1->IR = FDCAN_IR_TC;
		free_spots = 1;
	}

	if (ir & FDCAN_IR_TFE) // Transmission Fifo empty
	{
		FDCAN1->IR = FDCAN_IR_TFE;
		free_spots = modm::platform::Fdcan1::TxFifoSize;
	}

	free_spots = std::min(txQueue.getSize(), free_spots);

	for (; free_spots > 0; free_spots--)
	{
		sendMsg(txQueue.get());
		txQueue.pop();
	}	
}



/* FIFO0 Interrupt
 *
 
 */

MODM_ISR(FDCAN1_IT0)
{
	bool fifo0;
	uint32_t ir = FDCAN1->IR;
	if (not modm_assert_continue_ignore(not (ir & FDCAN_IE_RF0FE),
			"canfd.rx.hw0", "CAN receive fif0 buffer overflowed!", 1)) // Fifo 0 overrun
	{
		// release overrun flag & access the next message
		FDCAN1->IR = FDCAN_IE_RF0FE;
		fifo0 = true;
	}
	if (not modm_assert_continue_ignore(not (ir & FDCAN_IE_RF1FE),
			"canfd.rx.hw0", "CAN receive fif0 buffer overflowed!", 1)) // Fifo 1 overrun
	{
		// release overrun flag & access the next message
		FDCAN1->IR = FDCAN_IE_RF1FE;
		fifo0 = false;
	}

	if(ir & FDCAN_IE_RF0NE) //Fifo0 receive
	{
		FDCAN1->IR = FDCAN_IE_RF0NE;
		fifo0 = true;
	}


	if(ir & FDCAN_IE_RF1NE) //Fifo1 receive
	{
		FDCAN1->IR = FDCAN_IE_RF1NE;
		fifo0 = false;
	}

	RxMessage rxMessage;
	readMsg(rxMessage.message, &(rxMessage.filter_id), &(rxMessage.timestamp), fifo0);
	
	modm_assert_continue_ignore(rxQueue.push(rxMessage), "can.rx.sw0",
		"CAN receive software buffer overflowed!", 1);
}


void
modm::platform::Fdcan1::setMode(Mode mode)
{
	modm_assert_continue_ignore(enableProtectedAccess(), "can.setMode",
		"deadlockPreventer timed out", 1);
	
	FDCAN1->CCCR |= FDCAN_CCCR_CCE | FDCAN_CCCR_TEST;

	_setMode(mode);

	disableProtectedAccess();	
}

void modm::platform::Fdcan1::setFilter(uint16_t sfid1, uint16_t sfid2, StandardFilterMode mode, FilterConfig config, uint8_t index)
{
	if(index < Fdcan1::FilterListStandardSize)
	{
		modm_assert_continue_ignore(enableProtectedAccess(), "can.setFilter",
			"deadlockPreventer timed out", 1);

		uint32_t* base = (uint32_t*)(Fdcan1::FilterListStandard + (index * Fdcan1::FilterListStandardElementSize));
		*base = (((uint32_t) mode) << 30) |
				(((uint32_t) config) << 27) |
				((((uint32_t) sfid1) & 0x7FF) << 16) |
				(((uint32_t) sfid2) & 0x7FF);

		_setMode(last_set_mode); // Mode might get changed when modifying filters
		
		disableProtectedAccess();
	}
}

void modm::platform::Fdcan1::setExtendedFilter(uint32_t efid1, uint32_t efid2, ExtendedFilterMode mode, FilterConfig config, uint8_t index)
{
	if(index < Fdcan1::FilterListExtendedSize)
	{
		modm_assert_continue_ignore(enableProtectedAccess(), "can.setExtendedFilter",
			"deadlockPreventer timed out", 1);
		
		uint32_t* base = (uint32_t*)(Fdcan1::FilterListExtended + (index * Fdcan1::FilterListExtendedElementSize));

		*base++ = (((uint32_t) config) << 29) |
				(efid1 & 0x1FFFFFFF);
		*base = (((uint32_t) mode) << 30) |
				(efid2 & 0x1FFFFFFF);

		_setMode(last_set_mode); // Mode might get changed when modifying filters

		disableProtectedAccess();
	}
}

void
modm::platform::Fdcan1::_setMode(Mode mode)
{
	// Reset all mode register bits
	FDCAN1->TEST = 0;
	FDCAN1->CCCR &= ~(FDCAN_CCCR_ASM | FDCAN_CCCR_MON | FDCAN_CCCR_CSR | FDCAN_CCCR_TEST);
	last_set_mode = mode;
	// set mode
	switch(mode) {
		case Mode::Normal:
			break;
		case Mode::Restricted:
			FDCAN1->CCCR |= FDCAN_CCCR_ASM;
			break;
		case Mode::Monitoring:
			FDCAN1->CCCR |= FDCAN_CCCR_MON;
			break;
		case Mode::Sleep:
			FDCAN1->CCCR |= FDCAN_CCCR_CSR;
			break;
		case Mode::TestExternalLoopback:
			FDCAN1->CCCR |= FDCAN_CCCR_TEST;
			FDCAN1->TEST = FDCAN_TEST_LBCK;
			break;
		case Mode::TestInternalLoopback:
			FDCAN1->CCCR |= FDCAN_CCCR_TEST | FDCAN_CCCR_MON;
			FDCAN1->TEST = FDCAN_TEST_LBCK;
			break;
	}
}


void
modm::platform::Fdcan1::setAutomaticRetransmission(bool retransmission)
{
	if (retransmission) {
		// Enable retransmission
		FDCAN1->CCCR = (FDCAN1->CCCR & ~FDCAN_CCCR_DAR);
	} else {
		// Disable retransmission
		FDCAN1->CCCR = (FDCAN1->CCCR | FDCAN_CCCR_DAR);
	}
}


bool
modm::platform::Fdcan1::isMessageAvailable()
{
	return rxQueue.isNotEmpty();
}


bool
modm::platform::Fdcan1::getMessage(can::IMessage& message, uint8_t *filter_id, uint16_t *timestamp)
{
	if (rxQueue.isEmpty())
	{
		// no message in the receive buffer
		return false;
	}
	else {
		auto& rxMessage = rxQueue.get();
		message = rxMessage.message;

		if(filter_id != nullptr) {
			(*filter_id) = rxMessage.filter_id;
		}
		if(timestamp != nullptr) {
			(*timestamp) = rxMessage.timestamp;
		}
		rxQueue.pop();
		return true;
	}

}


bool
modm::platform::Fdcan1::isReadyToSend()
{
	return txQueue.isNotFull();
}


bool
modm::platform::Fdcan1::sendMessage(const can::IMessage& message)
{
	// This function is not reentrant. If one of the buffers is not full it
	// means that the software buffer is empty too. Therefore the buffer
	// will stay empty and won't be taken by an interrupt.
	if ((FDCAN1->TXFQS & FDCAN_TXFQS_TFQF) != 0)
	{
		// Buffer completely filled at the moment

		if (txQueue.isFull()) {
			return false;
		}
		txQueue.push(reinterpret_cast<const can::LongMessage&>(message));
		return true;
	}
	else {
		sendMsg(reinterpret_cast<const can::LongMessage&>(message));
		return true;
	}
}


modm::platform::Fdcan1::BusState
modm::platform::Fdcan1::getBusState()
{
	if (FDCAN1->PSR & FDCAN_PSR_BO) {
		return BusState::Off;
	}
	else if (FDCAN1->PSR & FDCAN_PSR_EP) {
		return BusState::ErrorPassive;
	}
	else if (FDCAN1->PSR & FDCAN_PSR_EP) {
		return BusState::ErrorWarning;
	}
	else {
		return BusState::Connected;
	}
}


void modm::platform::Fdcan1::setNumberOfFilters(uint8_t n)
{
	modm_assert_continue_ignore(enableProtectedAccess(), "can.setNumberOfFilters",
		"deadlockPreventer timed out", 1);
	
	FDCAN1->RXGFC |= (n << FDCAN_RXGFC_LSS_Pos) & FDCAN_RXGFC_LSS_Msk;

	_setMode(last_set_mode); // Mode might get changed when modifying filters
	
	disableProtectedAccess();
}

void modm::platform::Fdcan1::setNumberOfExtendedFilters(uint8_t n)
{

	modm_assert_continue_ignore(enableProtectedAccess(), "can.setNumberOfExtendedFilters",
		"deadlockPreventer timed out", 1);
	
	FDCAN1->RXGFC |= (n << FDCAN_RXGFC_LSE_Pos) & FDCAN_RXGFC_LSE_Msk;
	
	_setMode(last_set_mode); // Mode might get changed when modifying filters
	
	disableProtectedAccess();	
}



void
modm::platform::Fdcan1::enableStatusChangeInterrupt(uint32_t interruptEnable, uint32_t interruptPriority)
{
	// TODO
}
