/*
 * Copyright (c) 2019, Raphael Lehmann
 * Copyright (c) 2021, Christopher Durand
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
#include <cstring>
#include <optional>

#include <modm/architecture/driver/atomic/queue.hpp>
#include <modm/platform/clock/rcc.hpp>
#include <modm/architecture/interface/delay.hpp>
#include <modm/architecture/interface/assert.hpp>
#include <modm/architecture/interface/interrupt.hpp>

#include "can_2.hpp"

namespace {

using MessageRam = modm::platform::fdcan::MessageRam<1>;

modm::atomic::Queue<modm::can::LongMessage, 32> txQueue;
struct RxMessage {
    modm::can::LongMessage message;
    uint8_t filter_id;
    uint16_t timestamp;
};
modm::atomic::Queue<RxMessage, 32> rxQueue;
bool
isHardwareTxQueueFull()
{
	return ((FDCAN2->TXFQS & FDCAN_TXFQS_TFQF) != 0);
}

bool
rxFifo0HasMessage()
{
	return ((FDCAN2->RXF0S & FDCAN_RXF0S_F0FL_Msk) > 0);
}

bool
rxFifo1HasMessage()
{
	return ((FDCAN2->RXF1S & FDCAN_RXF1S_F1FL_Msk) > 0);
}

void
acknowledgeRxFifoRead(uint8_t fifoIndex, uint8_t getIndex)
{
	if (fifoIndex == 0) {
		FDCAN2->RXF0A = getIndex;
	}
	else {
		FDCAN2->RXF1A = getIndex;
	}
}

uint8_t
retrieveRxFifoGetIndex(uint8_t fifoIndex)
{
	if (fifoIndex == 0) {
		return ((FDCAN2->RXF0S & FDCAN_RXF0S_F0GI_Msk) >> FDCAN_RXF0S_F0GI_Pos);
	} else {
		return ((FDCAN2->RXF1S & FDCAN_RXF1S_F1GI_Msk) >> FDCAN_RXF1S_F1GI_Pos);
	}
}

uint8_t
retrieveTxFifoPutIndex()
{
	return ((FDCAN2->TXFQS & FDCAN_TXFQS_TFQPI_Msk) >> FDCAN_TXFQS_TFQPI_Pos);
}

// Internal function to receive a message from an RX Fifo.
// Called by RX interrupt or by getMessage()
void
readMsg(modm::can::IMessage& message, uint8_t fifoIndex, uint8_t* filter_id, uint16_t *timestamp)
{
	using namespace modm::platform;
	using CommonHeader = MessageRam::CommonFifoHeader;
	using RxFifoAddress = MessageRam::RxFifoAddress;

	// retrieve index of next frame in RX fifo
	const uint8_t getIndex = retrieveRxFifoGetIndex(fifoIndex);
	const RxFifoAddress address = {fifoIndex, getIndex};

	const auto [commonHeader, rxHeader] = MessageRam::readRxHeaders(address);

	message.setExtended(bool(commonHeader & CommonHeader::ExtendedId));
	message.setRemoteTransmitRequest(bool(commonHeader & CommonHeader::RemoteFrame));
	const auto id = MessageRam::CanId_t::get(commonHeader);
	if(message.isExtended()) {
		message.setIdentifier(id);
	} else {
		message.setIdentifier(id >> 18);
	}

	if (filter_id != nullptr) {
		*filter_id = MessageRam::FilterIndex_t::get(rxHeader);
	}

	if (timestamp != nullptr) {
		*timestamp = MessageRam::Timestamp_t::get(rxHeader);
	}

	const uint8_t dlcValue = MessageRam::RxDlc_t::get(rxHeader);
	// TODO: fd large frames not supported yet
	if (rxHeader & MessageRam::RxFifoHeader::FdFrame)
		message.setDLC(dlcValue);
	 else
		message.setLength(std::min<uint8_t>(8u, dlcValue));

	// required for optimization in MessageRam::readData()
	//static_assert((std::size(decltype(message.data){}) % 4) == 0);

	MessageRam::readData(address, {&message.data[0], message.getLength()});
	acknowledgeRxFifoRead(fifoIndex, getIndex);
}

// Internal function to send a CAN message.
// called by sendMessage and by TX Interrupt.
bool
sendMsg(const modm::can::IMessage& message)
{
	using namespace modm::platform;

	if (!Fdcan2::isReadyToSend()) {
		return false;
	}

	// TODO: for fdcan frame format with bit rate switching set:
	// (TxFifoHeader::FdFrame | TxFifoHeader::RateSwitching)
	MessageRam::TxFifoHeader_t txHeader{};

	// TODO: large frame support
	const uint8_t dlc = message.getDLC();
	MessageRam::TxDlc_t::set(txHeader, dlc);

	const uint8_t putIndex = retrieveTxFifoPutIndex();
	const auto commonHeader = MessageRam::headerFromMessage(message);
	MessageRam::writeTxHeaders(putIndex, commonHeader, txHeader);

	// required for optimization in MessageRam::readData()
	//static_assert((std::size(decltype(message.data){}) % 4) == 0);

	MessageRam::writeData(putIndex, {&message.data[0], message.getLength()});

	// Activate the corresponding transmission request
	FDCAN2->TXBAR = (1u << putIndex);

	return true;
}

}

void
modm::platform::Fdcan2::initializeWithPrescaler(
		CanBitTimingConfiguration standardTimings,
		std::optional<CanBitTimingConfiguration> fdDataTimings,
		uint32_t interruptPriority, Mode startupMode, bool overwriteOnOverrun)
{
	Rcc::enable<Peripheral::Fdcan1>();

	EnterInitMode init;

	// Configure nominal bitrate
	FDCAN2->NBTP =
		((standardTimings.sjw - 1) << FDCAN_NBTP_NSJW_Pos) |
		((standardTimings.bs2 - 1) << FDCAN_NBTP_NTSEG2_Pos) |
		((standardTimings.bs1 - 1) << FDCAN_NBTP_NTSEG1_Pos) |
		((standardTimings.prescaler - 1) << FDCAN_NBTP_NBRP_Pos);

	if(fdDataTimings) {
		// Configure FD mode fast data bitrate
		const auto& timings = *fdDataTimings;
		FDCAN2->DBTP =
			((timings.sjw - 1) << FDCAN_DBTP_DSJW_Pos) |
			((timings.bs2 - 1) << FDCAN_DBTP_DTSEG2_Pos) |
			((timings.bs1 - 1) << FDCAN_DBTP_DTSEG1_Pos) |
			((timings.prescaler - 1) << FDCAN_DBTP_DBRP_Pos)/* |
			FDCAN_DBTP_TDC*/; // enable "Transceiver Delay Compensation"
	}

	// Timestamp: FDCAN internal counter with prescaler=1
	// In CAN FD mode the internal timestamp counter TCP does not provide a constant time
	// base due to the different CAN bit times between arbitration phase and data phase.
	FDCAN2->TSCC = (1 << FDCAN_TSCC_TSS_Pos);

	configureInterrupts(interruptPriority);

	FDCAN2->RXGFC = overwriteOnOverrun ? (FDCAN_RXGFC_F1OM | FDCAN_RXGFC_F0OM) : 0;

	// enable all filters, reject non-matching frames
	FDCAN2->RXGFC |= FDCAN_RXGFC_LSE | FDCAN_RXGFC_LSS | FDCAN_RXGFC_ANFE | FDCAN_RXGFC_ANFS;

	// Tx buffer: queue mode
	FDCAN2->TXBC = FDCAN_TXBC_TFQM;

	// Enable bit rate switching and CANFD frame format
	if(fdDataTimings) {
		FDCAN2->CCCR |= FDCAN_CCCR_BRSE | FDCAN_CCCR_FDOE;
	} else {
		FDCAN2->CCCR &= ~(FDCAN_CCCR_BRSE | FDCAN_CCCR_FDOE);
	}

	configureMode(startupMode);
}

// line 0 used as TX and error interrupt
// generated on finished frame transmission and error state
MODM_ISR(FDCAN2_IT0)
{
	if (txQueue.isNotEmpty()) {
		sendMsg(txQueue.get());
		txQueue.pop();
	}
	const auto callback = modm::platform::Fdcan2::getErrorInterruptCallback();
	if (callback) {
		const bool hasErrorInterrupt = (FDCAN2->IR & (FDCAN_IR_BO | FDCAN_IR_EW | FDCAN_IR_EP));
		if (hasErrorInterrupt) {
			callback();
		}
	}
	FDCAN2->IR = FDCAN_IR_TC | FDCAN_IR_BO | FDCAN_IR_EW | FDCAN_IR_EP;
}


// line 1 used as RX interrupt
// generated on received frame
MODM_ISR(FDCAN2_IT1)
{
	RxMessage rxMessage;

	if (rxFifo0HasMessage()) {
		readMsg(rxMessage.message, 0, &rxMessage.filter_id, &rxMessage.timestamp);
		// acknowledge interrupt flag
		FDCAN2->IR = FDCAN_IR_RF0N;

		modm_assert_continue_ignore(rxQueue.push(rxMessage), "fdcan.rx.buffer",
			"CAN receive software buffer overflowed!", 2);
	}

	if (rxFifo1HasMessage()) {
		readMsg(rxMessage.message, 1, &rxMessage.filter_id, &rxMessage.timestamp);
		// acknowledge interrupt flag
		FDCAN2->IR = FDCAN_IR_RF1N;

		modm_assert_continue_ignore(rxQueue.push(rxMessage), "fdcan.rx.buffer",
			"CAN receive software buffer overflowed!", 2);
	}
}


void
modm::platform::Fdcan2::setMode(Mode mode)
{
	EnterInitMode init;
	FDCAN2->CCCR |= FDCAN_CCCR_CCE;
	configureMode(mode);
}


void
modm::platform::Fdcan2::configureMode(Mode mode)
{
	// Reset all mode register bits
	FDCAN2->TEST = 0;
	FDCAN2->CCCR &= ~(FDCAN_CCCR_ASM | FDCAN_CCCR_MON | FDCAN_CCCR_CSR | FDCAN_CCCR_TEST);

	// set mode
	switch(mode) {
		case Mode::Normal:
			break;
		case Mode::Restricted:
			FDCAN2->CCCR |= FDCAN_CCCR_ASM;
			break;
		case Mode::Monitoring:
			FDCAN2->CCCR |= FDCAN_CCCR_MON;
			break;
		case Mode::Sleep:
			FDCAN2->CCCR |= FDCAN_CCCR_CSR;
			break;
		case Mode::TestExternalLoopback:
			FDCAN2->CCCR |= FDCAN_CCCR_TEST;
			FDCAN2->TEST = FDCAN_TEST_LBCK;
			break;
		case Mode::TestInternalLoopback:
			FDCAN2->CCCR |= FDCAN_CCCR_TEST | FDCAN_CCCR_MON;
			FDCAN2->TEST = FDCAN_TEST_LBCK;
			break;
	}
}


void
modm::platform::Fdcan2::configureInterrupts(uint32_t interruptPriority)
{
	NVIC_SetPriority(FDCAN2_IT0_IRQn, interruptPriority);
	NVIC_SetPriority(FDCAN2_IT1_IRQn, interruptPriority);

	NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
	NVIC_EnableIRQ(FDCAN2_IT1_IRQn);

	// enable both interrupts lines (0 and 1)
	FDCAN2->ILE = FDCAN_ILE_EINT1 | FDCAN_ILE_EINT0;
	// assign receive interrupts to line 1
	FDCAN2->ILS = (FDCAN_ILS_RXFIFO0 | FDCAN_ILS_RXFIFO1);

	FDCAN2->IE = 0;
	// enable transmission complete and fifo empty interrupt
	FDCAN2->IE |= FDCAN_IE_TCE;
	// enable TX interrupts for all 3 buffers
	FDCAN2->TXBTIE |= FDCAN_TXBTIE_TIE;
	// enable message receive interrupts for both RX FIFOs
	FDCAN2->IE |= (FDCAN_IE_RF0NE | FDCAN_IE_RF1NE);
}


void
modm::platform::Fdcan2::setAutomaticRetransmission(bool retransmission)
{
	if (retransmission) {
		// Enable retransmission
		FDCAN2->CCCR = (FDCAN2->CCCR & ~FDCAN_CCCR_DAR);
	} else {
		// Disable retransmission
		FDCAN2->CCCR = (FDCAN2->CCCR | FDCAN_CCCR_DAR);
	}
}


bool
modm::platform::Fdcan2::isMessageAvailable()
{
	return rxQueue.isNotEmpty();
}


bool
modm::platform::Fdcan2::getMessage(can::IMessage& message, uint8_t *filter_id, uint16_t *timestamp)
{
	if (rxQueue.isEmpty()) {
		// no message in the receive buffer
		return false;
	} else {
		auto& rxMessage = rxQueue.get();
		message = rxMessage.message;
		if (filter_id != nullptr) {
			(*filter_id) = rxMessage.filter_id;
		}
		if (timestamp != nullptr) {
			(*timestamp) = rxMessage.timestamp;
		}
		rxQueue.pop();
		return true;
	}
}


bool
modm::platform::Fdcan2::isReadyToSend()
{
	return txQueue.isNotFull();
}


bool
modm::platform::Fdcan2::sendMessage(const can::IMessage& message)
{
	if (isHardwareTxQueueFull()) {
		if (txQueue.isFull()) {
			return false;
		}
		txQueue.push((const can::LongMessage&)message);
		return true;
	} else {
		sendMsg(message);
		return true;
	}
}


modm::platform::Fdcan2::BusState
modm::platform::Fdcan2::getBusState()
{
	if (FDCAN2->PSR & FDCAN_PSR_BO) {
		return BusState::Off;
	}
	else if (FDCAN2->PSR & FDCAN_PSR_EP) {
		return BusState::ErrorPassive;
	}
	else if (FDCAN2->PSR & FDCAN_PSR_EW) {
		return BusState::ErrorWarning;
	}
	else {
		return BusState::Connected;
	}
}


bool
modm::platform::Fdcan2::setStandardFilter(
	uint8_t standardIndex, FilterConfig config,
	modm::can::StandardIdentifier id,
	modm::can::StandardMask mask)
{
	if (standardIndex >= StandardFilterCount) {
		return false;
	}

	EnterInitMode init;

	MessageRam::setStandardFilter(standardIndex,
		MessageRam::FilterType::Classic,
		config, uint16_t(id), uint16_t(mask));

	return true;
}


bool
modm::platform::Fdcan2::setStandardFilter(
	uint8_t standardIndex, FilterConfig config,
	modm::can::StandardIdentifier id0,
	modm::can::StandardIdentifier id1)
{
	if (standardIndex >= StandardFilterCount) {
		return false;
	}

	EnterInitMode init;

	MessageRam::setStandardFilter(standardIndex,
		MessageRam::FilterType::Dual,
		config, uint16_t(id0), uint16_t(id1));

	return true;
}


bool
modm::platform::Fdcan2::setStandardRangeFilter(
	uint8_t standardIndex, FilterConfig config,
	modm::can::StandardIdentifier first,
	modm::can::StandardIdentifier last)
{
	if (standardIndex >= StandardFilterCount) {
		return false;
	}

	EnterInitMode init;

	MessageRam::setStandardFilter(standardIndex,
		MessageRam::FilterType::Range,
		config, uint16_t(first), uint16_t(last));

	return true;
}


bool
modm::platform::Fdcan2::setExtendedFilter(
	uint8_t extendedIndex, FilterConfig config,
	modm::can::ExtendedIdentifier id,
	modm::can::ExtendedMask mask)
{
	if (extendedIndex >= ExtendedFilterCount) {
		return false;
	}

	EnterInitMode init;

	MessageRam::setExtendedFilter0(extendedIndex,
		config, uint32_t(id));
	MessageRam::setExtendedFilter1(extendedIndex,
		MessageRam::FilterType::Classic, uint32_t(mask));

	return true;
}


bool
modm::platform::Fdcan2::setExtendedFilter(
	uint8_t extendedIndex, FilterConfig config,
	modm::can::ExtendedIdentifier id0,
	modm::can::ExtendedIdentifier id1)
{
	if (extendedIndex >= ExtendedFilterCount) {
		return false;
	}

	EnterInitMode init;

	MessageRam::setExtendedFilter0(extendedIndex,
		config, uint32_t(id0));
	MessageRam::setExtendedFilter1(extendedIndex,
		MessageRam::FilterType::Dual, uint32_t(id1));

	return true;
}


bool
modm::platform::Fdcan2::setExtendedRangeFilter(
	uint8_t extendedIndex, FilterConfig config,
	modm::can::ExtendedIdentifier first,
	modm::can::ExtendedIdentifier last)
{
	if (extendedIndex >= ExtendedFilterCount) {
		return false;
	}

	EnterInitMode init;

	MessageRam::setExtendedFilter0(extendedIndex,
		config, uint32_t(first));
	MessageRam::setExtendedFilter1(extendedIndex,
		MessageRam::FilterType::Range, uint32_t(last));

	return true;
}


void
modm::platform::Fdcan2::clearStandardFilters()
{
	EnterInitMode init;
	for (unsigned i = 0; i < StandardFilterCount; ++i) {
		MessageRam::setStandardFilterDisabled(i);
	}
}


void
modm::platform::Fdcan2::clearExtendedFilters()
{
	EnterInitMode init;
	for (unsigned i = 0; i < ExtendedFilterCount; ++i) {
		MessageRam::setExtendedFilterDisabled(i);
	}
}