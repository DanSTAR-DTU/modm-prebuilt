// coding: utf-8
/*
 * Copyright (c) 2017, Niklas Hauser
 * Copyright (c) 2017, Sascha Schade
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

/* To debug the internal state of the driver, you can instantiate a
 * modm::IOStream in your main source file, which will then be used to dump
 * state data of the operations via the serial port, e.g.
 *   #include <modm/io/iostream.hpp>
 *   modm::IODeviceWrapper< Uart5, modm::IOBuffer::BlockIfFull > device;
 *   modm::IOStream stream(device);

 * Be advised, that a typical I2C read/write operation can take 10 to 100 times longer
 * because the strings have to be copied during the interrupt!
 *
 * If you send too much debug information in the IRQs the program will hang.
 *
 * Is it advised to increase the UART buffer dramatically so that the messages
 * can be stored in the buffer during the interrupt. Do so by adding to project.cfg
 *
 * [parameters]
 * uart.stm32.2.tx_buffer = 60000
 *
 * You can then enable serial debugging with this define by changing 0 to 1.
 */
#define SERIAL_DEBUGGING 0

#if SERIAL_DEBUGGING
#	include "../../uart/stm32/uart_2.hpp"
	using DebugUart = modm::platform::Usart2;
#	include <modm/io/iostream.hpp>
	extern modm::IOStream stream;
#	define DEBUG_STREAM(x) stream << x << "\n"
#	define DEBUG(x) modm::platform::Usart2::write(x)
#else
#	define DEBUG_STREAM(x)
#	define DEBUG(x)
#endif

#include "i2c_master_4.hpp"
#include <modm/architecture/interface/accessor.hpp>
#include <modm/architecture/driver/atomic/queue.hpp>
#include <modm/architecture/interface/atomic_lock.hpp>
#include <modm/architecture/interface/interrupt.hpp>
#include <modm/container.hpp>
#include <modm/platform/clock/rcc.hpp>

MODM_ISR_DECL(I2C4_ER);
namespace
{
	static modm::I2c::Operation nextOperation;

	// transaction queue management
	struct ConfiguredTransaction
	{
		ConfiguredTransaction()
		:	transaction(nullptr), configuration(nullptr) {}

		ConfiguredTransaction(modm::I2cTransaction *transaction, modm::I2c::ConfigurationHandler configuration)
		:	transaction(transaction), configuration(configuration) {}

		modm::I2cTransaction *transaction;
		modm::I2c::ConfigurationHandler configuration;
	};

	static modm::BoundedQueue<ConfiguredTransaction, 8> queue;
	static modm::I2c::ConfigurationHandler configuration(nullptr);

	// delegating
	static modm::I2cTransaction *transaction(nullptr);
	static modm::I2cMaster::Error error(modm::I2cMaster::Error::NoError);

	// buffer management
	static modm::I2cTransaction::Starting starting(0, modm::I2c::OperationAfterStart::Stop);
	static modm::I2cTransaction::Writing writing(nullptr, 0, modm::I2c::OperationAfterWrite::Stop);
	static modm::I2cTransaction::Reading reading(nullptr, 0, modm::I2c::OperationAfterRead::Stop);

	// helper functions
	static inline void
	callWriteOperation(const bool startCondition)
	{
		DEBUG_STREAM("write op: writing=" << writing.length);
		DEBUG_STREAM("nextOperation=" << nextOperation);

		// Write first data byte to TXDR
		if ((writing.length > 0) and (writing.buffer != nullptr)) {
			I2C4->TXDR = *writing.buffer++;
		}

		// Only 255 bytes can be written at once.
		bool autoend = false;
		bool reload  = false;
		uint8_t nbytes = (writing.length > 255) ? 255 : writing.length;

		if ((nextOperation == modm::I2c::Operation::Write) or (writing.length > 255)) {
			// RELOAD if the operation continues
			reload = true;
		} else if ((nextOperation == modm::I2c::Operation::Stop) /* and (writing.length <= 0xff) */) {
			// AUTOEND has no effect if RELOAD is set.
			autoend = true;
		}

		DEBUG_STREAM("autoend=" << autoend << ", reload=" << reload << ", nbytes=" << nbytes);

		I2C4->CR2 = (autoend ? I2C_CR2_AUTOEND : 0 ) |
						   (reload  ? I2C_CR2_RELOAD  : 0 ) |
						   (nbytes << I2C_CR2_NBYTES_Pos)   |
						   (startCondition ? (I2C_CR2_START | (starting.address & 0xfe)) : 0 );

		if (writing.length > 0) {
			--writing.length;
		}

		I2C4->CR1 &= ~(I2C_CR1_STOPIE | I2C_CR1_TCIE | I2C_CR1_RXIE | I2C_CR1_TXIE);

		// Always enable the NACK interrupt in case we're trying to write to a slave that's not responding
		I2C4->CR1 |= I2C_CR1_NACKIE;

		if (autoend and (writing.length == 0))
		{
			// Transfer is ended by hardware, so wait for Stop condition generated by hardware.
			DEBUG_STREAM("Wait for STOP IRQ");
			I2C4->CR1 |= I2C_CR1_STOPIE;
		}
		else if (writing.length == 0)
		{
			// All (1 byte) written. Waiting for the end of the transfer.
			DEBUG_STREAM("Wait for TC IRQ");
			I2C4->CR1 |= I2C_CR1_TCIE;
		} else {
			// More to write
			DEBUG_STREAM("Wait for TXIE IRQ 1");
			I2C4->CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE;
		}
	}

	static inline void
	callReadOperation(const bool startCondition)
	{
		DEBUG_STREAM("read op: reading=" << reading.length);
		DEBUG_STREAM("nextOperation=" << nextOperation);

		// Only 255 bytes can be written at once.
		bool autoend = false;
		bool reload = false;
		uint8_t nbytes = (reading.length > 255) ? 255 : reading.length;

		if ((nextOperation != modm::I2c::Operation::Stop) or (reading.length > 255)) {
			// RELOAD if the operation continues
			reload = true;
		} else if ((nextOperation == modm::I2c::Operation::Stop)) {
			// AUTOEND has no effect if RELOAD is set.
			autoend = true;
		}

		DEBUG_STREAM("autoend=" << autoend << ", reload=" << reload << ", nbytes=" << nbytes);

		I2C4->CR2 = (autoend ? I2C_CR2_AUTOEND : 0) |
						   (reload  ? I2C_CR2_RELOAD  : 0) |
						   (nbytes << I2C_CR2_NBYTES_Pos)  |
						   (I2C_CR2_RD_WRN)                |
						   (startCondition ? (I2C_CR2_START | (starting.address & 0xfe)) : 0);

		I2C4->CR1 &= ~(I2C_CR1_STOPIE | I2C_CR1_TCIE | I2C_CR1_RXIE | I2C_CR1_TXIE);

		// Always enable the NACK interrupt in case we're trying to read from a slave that's not responding
		I2C4->CR1 |= I2C_CR1_NACKIE;

		if (autoend and (reading.length == 0))
		{
			// Transfer is ended by hardware, so wait for Stop condition generated by hardware.
			// RXNE will not be set
			DEBUG_STREAM("Wait for STOP IRQ");
			I2C4->CR1 |= I2C_CR1_STOPIE;
		}
		else if (reading.length == 0)
		{
			// RXNE will not be set
			DEBUG_STREAM("Wait for TC IRQ");
			I2C4->CR1 |= I2C_CR1_TCIE;
		}
		else
		{
			// Wait until next byte received by hardware. RXIE waits for RXNE (RX not empty)
			// Wait for Stop condition after last byte.
			// Wait for Transfer Complete Reload if a reload is required.
			DEBUG_STREAM("Wait for RXNE IRQ");
			I2C4->CR1 |= I2C_CR1_RXIE | I2C_CR1_STOPIE | (reload ? I2C_CR1_TCIE : 0);
		}
	}

	static inline void
	callStarting()
	{
		starting = transaction->starting();
		switch (starting.next)
		{
			case modm::I2c::OperationAfterStart::Read:
				reading = transaction->reading();
				nextOperation = static_cast<modm::I2c::Operation>(reading.next);

				callReadOperation(/* startCondition */ true);
				break;

			case modm::I2c::OperationAfterStart::Write:
				// Advance transaction
				writing = transaction->writing();
				nextOperation = static_cast<modm::I2c::Operation>(writing.next);

				callWriteOperation(/* startCondition = */ true);
				break;

			case modm::I2c::OperationAfterStart::Stop:
				// No data after address write, like ping
				writing.length = 0;
				reading.length = 0;
				nextOperation = modm::I2c::Operation::Stop;

				DEBUG_STREAM("S AW P");

				// Reload = 0
				I2C4->CR2 = I2C_CR2_AUTOEND | (0 << I2C_CR2_NBYTES_Pos) | \
								   I2C_CR2_START   | (starting.address & 0xfe);

				// Only wait for STOPF Interrupt as frame is automatically terminated with stop condition
				DEBUG_STREAM("Wait for STOP IRQ");
				I2C4->CR1 &= ~(I2C_CR1_STOPIE | I2C_CR1_TCIE | I2C_CR1_RXIE | I2C_CR1_TXIE);
				I2C4->CR1 |= I2C_CR1_STOPIE;

				break;
		}

		error = modm::I2cMaster::Error::NoError;
	}

	static inline void
	callNextTransaction()
	{
		if (queue.isNotEmpty())
		{
			// wait until the stop condition has been generated
			uint_fast32_t deadlockPreventer = 100'000;
			while ((I2C4->ISR & I2C_ISR_STOPF) and (deadlockPreventer-- > 0))
				{};

			ConfiguredTransaction next = queue.get();
			queue.pop();
			// configure the peripheral if necessary
			if (next.configuration and (configuration != next.configuration)) {
				configuration = next.configuration;
				configuration();
			}

			DEBUG_STREAM("\n###\n");
			::transaction = next.transaction;
			// start the transaction
			callStarting();
		}
	}

	bool modm_always_inline
	handleError()
	{
		uint16_t sr1 = I2C4->ISR;

		if (sr1 & I2C_ISR_BERR)
		{
			DEBUG_STREAM("BUS ERROR");
			I2C4->ICR |= I2C_ICR_BERRCF;
			error = modm::I2cMaster::Error::BusCondition;
		}
		else if (sr1 & I2C_ISR_ARLO)
		{	// arbitration lost
			I2C4->ICR |= I2C_ICR_ARLOCF;
			DEBUG_STREAM("ARBITRATION LOST");
			error = modm::I2cMaster::Error::ArbitrationLost;
		}
		else if ((sr1 & I2C_ISR_TIMEOUT) || (sr1 & I2C_ISR_ALERT) || (sr1 & I2C_ISR_PECERR))
		{
			// should only occur in unsupported SMBus mode
			DEBUG_STREAM("UNKNOWN, SMBUS");
			I2C4->ICR |= I2C_ICR_ALERTCF;
			I2C4->ICR |= I2C_ICR_TIMOUTCF;
			I2C4->ICR |= I2C_ICR_PECCF;
			error = modm::I2cMaster::Error::Unknown;
		}
		else if (sr1 & I2C_ISR_OVR)
		{
			// should not occur in master mode
			DEBUG_STREAM("UNKNOWN");
			I2C4->ICR |= I2C_ICR_OVRCF;
			error = modm::I2cMaster::Error::Unknown;
		}
		else
		{
			return false;
		}

		if (transaction) transaction->detaching(modm::I2c::DetachCause::ErrorCondition);
		transaction = nullptr;

		// Clear flags and interrupts
		writing.length = 0;
		reading.length = 0;

		DEBUG_STREAM("disable interrupts");
		I2C4->CR1 &= ~(
			I2C_CR1_STOPIE |
			I2C_CR1_TCIE   |
			I2C_CR1_RXIE   |
			I2C_CR1_TXIE   |
			I2C_CR1_RXIE);

		callNextTransaction();
		return true;
	}

}

// ----------------------------------------------------------------------------
MODM_ISR(I2C4_EV)
{
	DEBUG_STREAM("\n=== IRQ ===");

	uint16_t isr = I2C4->ISR;

	I2C4->CR1 &= ~(I2C_CR1_STOPIE | I2C_CR1_TCIE | I2C_CR1_RXIE | I2C_CR1_TXIE);

	I2C4->ICR |= I2C_ICR_STOPCF;

#if SERIAL_DEBUGGING
	if (isr & I2C_ISR_BUSY)  { DEBUG_STREAM_N("BUSY " ); } else { DEBUG_STREAM_N("busy " ); }
	if (isr & I2C_ISR_ARLO)  { DEBUG_STREAM_N("ARLO " ); } else { DEBUG_STREAM_N("arlo " ); }
	if (isr & I2C_ISR_BERR)  { DEBUG_STREAM_N("BERR " ); } else { DEBUG_STREAM_N("berr " ); }
	if (isr & I2C_ISR_TCR)   { DEBUG_STREAM_N("TCR "  ); } else { DEBUG_STREAM_N("tcr "  ); }
	if (isr & I2C_ISR_TC)    { DEBUG_STREAM_N("TC "   ); } else { DEBUG_STREAM_N("tc "   ); }
	if (isr & I2C_ISR_STOPF) { DEBUG_STREAM_N("STOPF "); } else { DEBUG_STREAM_N("stopf "); }
	if (isr & I2C_ISR_NACKF) { DEBUG_STREAM_N("NACKF "); } else { DEBUG_STREAM_N("nackf "); }
	if (isr & I2C_ISR_RXNE)  { DEBUG_STREAM_N("RXNE " ); } else { DEBUG_STREAM_N("rxne " ); }
	if (isr & I2C_ISR_TXIS)  { DEBUG_STREAM_N("TXIS " ); } else { DEBUG_STREAM_N("txis " ); }
	if (isr & I2C_ISR_TXE)   { DEBUG_STREAM  ("TXE"   ); } else { DEBUG_STREAM  ("txe"   ); }
#endif

	// First read from RXDR before checking STOP
	if (isr & I2C_ISR_RXNE)
	{
		*reading.buffer++ = I2C4->RXDR & 0xff;
		--reading.length;

		if (reading.length > 0) {
			// Wait for next RXIE interrupt
			DEBUG_STREAM("Wait for RXIE IRQ. rx.len = " << reading.length);
			I2C4->CR1 |= I2C_CR1_RXIE;
		} else {
			DEBUG_STREAM("rx.len = 0");
			DEBUG_STREAM("RXNE: nextOperation = " << nextOperation);
			if (nextOperation == modm::I2c::Operation::Stop)
			{
				if (not (isr & I2C_ISR_STOPF))
				{
					// Stop was not yet generated. Wait for next interrupt.
					DEBUG_STREAM("Wait for STOP IRQ");
					I2C4->CR1 |= I2C_CR1_STOPIE;
				} else {
					// Process STOP condition below.
				}
			}
			// IMHO: There is no other operation after read allowed.
		}
	}

	if (isr & I2C_ISR_NACKF)
	{
		// acknowledge fail
		I2C4->ICR |= I2C_ICR_NACKCF;
		DEBUG_STREAM("ACK FAIL");
		// may also be ADDRESS_NACK
		error = starting.address ? modm::I2cMaster::Error::AddressNack : modm::I2cMaster::Error::DataNack;
		if (transaction) {
			transaction->detaching(modm::I2c::DetachCause::ErrorCondition);
			transaction = nullptr;
		}
	}

	if (isr & I2C_ISR_STOPF)
	{
		// Stop condition was generated
		if (nextOperation == modm::I2c::Operation::Stop)
		{
			if (transaction) {
				transaction->detaching(modm::I2c::DetachCause::NormalStop);
				transaction = nullptr;
			}
			DEBUG_STREAM("transaction finished");
			callNextTransaction();
		}
	}

	if (isr & I2C_ISR_TXIS)
	{
		// Transmit Interrupt Status (transmitters)

		// Write another data byte to TXDR
		if ((writing.length > 0) and (writing.buffer != nullptr)) {
			I2C4->TXDR = *writing.buffer++;
		}

		if (writing.length > 0) {
			--writing.length;
		}

		if (writing.length == 0)
		{
			if ((nextOperation == modm::I2c::Operation::Restart) or
				(nextOperation == modm::I2c::Operation::Write))
			{
				DEBUG_STREAM("Wait for TC IRQ");
				I2C4->CR1 |= I2C_CR1_TCIE;
			} else {
				// All (1 byte) written. Waiting for the end of the transfer.
				DEBUG_STREAM("Wait for STOP IRQ");
				I2C4->CR1 |= I2C_CR1_STOPIE;
			}
		} else {
			// More to write
			DEBUG_STREAM("Wait for TXIE IRQ");
			I2C4->CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE;
		}
	}

	if (isr & I2C_ISR_TCR)
	{
		DEBUG_STREAM("--- TCR ---");

		if (writing.length > 0)
		{
			// RELOAD called, more to write
			callWriteOperation(/* startCondition = */ false);
		}
		else if (reading.length > 0)
		{
			// RELOAD was set, more to read
			callReadOperation(/* startCondition = */ false);
		}
		else if (nextOperation == modm::I2c::Operation::Write)
		{
			// Advance transaction
			writing = transaction->writing();
			nextOperation = static_cast<modm::I2c::Operation>(writing.next);

			callWriteOperation(/* startCondition = */ false);
		}
		else if (nextOperation == modm::I2c::Operation::Read)
		{
			// Advance transaction
			reading = transaction->reading();
			nextOperation = static_cast<modm::I2c::Operation>(reading.next);

			callReadOperation(/* startCondition = */ false);
		}
	}

	if (isr & I2C_ISR_TC)
	{
		DEBUG_STREAM("--- TC ---");

		// Transfer Complete (master mode)
		if (nextOperation == modm::I2c::Operation::Restart)
		{
			DEBUG_STREAM("restart op");
			callStarting();
		}
	}
}

// ----------------------------------------------------------------------------
MODM_ISR(I2C4_ER)
{
	handleError();
}
// ----------------------------------------------------------------------------

void
modm::platform::I2cMaster4::initializeWithPrescaler(uint32_t timingRegisterValue)
{
	Rcc::enable<Peripheral::I2c4>();

	// Disable the I2C peripheral which causes a software reset
	I2C4->CR1 &= ~I2C_CR1_PE;

	// Configure I2Cx: Frequency range
	// 39.4.8: Before enabling the peripheral, the I2C master clock must be configured by setting the
	// SCLH and SCLL bits in the I2C_TIMINGR register.

	static constexpr uint32_t TIMING_CLEAR_MASK = 0xF0FFFFFF;
	I2C4->TIMINGR = timingRegisterValue & TIMING_CLEAR_MASK;

	// Disable Own Address1 before set the Own Address1 configuration
	I2C4->OAR1 &= ~I2C_OAR1_OA1EN;

	// Configure I2Cx: Own Address1 and ack own address1 mode
	static constexpr uint32_t own_address = 0;
	enum class AddressingMode
	{
		Bit7,
		Bit10,
	};

	static constexpr AddressingMode addressingMode = AddressingMode::Bit7;

	// Configure Addressing Master mode
	switch (addressingMode)
	{
		case AddressingMode::Bit7:
			I2C4->OAR1 = (I2C_OAR1_OA1EN | own_address);
			break;
		case AddressingMode::Bit10:
			I2C4->OAR1 = (I2C_OAR1_OA1EN | I2C_OAR1_OA1MODE | own_address);
			I2C4->CR2  = (I2C_CR2_ADD10);
			break;
	}

	// Disable Own Address 2
	I2C4->OAR2 = 0;

	// Configure Generalcall and NoStretch mode
	I2C4->CR1 = (0 | I2C_CR1_NOSTRETCH);

	// Enable Error Interrupt
	NVIC_SetPriority(I2C4_ER_IRQn, 10);
	NVIC_EnableIRQ(I2C4_ER_IRQn);

	// Enable Event Interrupt
	NVIC_SetPriority(I2C4_EV_IRQn, 10);
	NVIC_EnableIRQ(I2C4_EV_IRQn);
	// Enable peripheral
	I2C4->CR1 |= I2C_CR1_PE;
}

void
modm::platform::I2cMaster4::reset()
{
	reading.length = 0;
	writing.length = 0;
	error = Error::SoftwareReset;
	if (transaction) transaction->detaching(DetachCause::ErrorCondition);
	transaction = nullptr;
	// remove all queued transactions
	while (queue.isNotEmpty())
	{
		ConfiguredTransaction next = queue.get();
		if (next.transaction) { next.transaction->detaching(DetachCause::ErrorCondition); }
		queue.pop();
	}
}

bool
modm::platform::I2cMaster4::start(I2cTransaction *transaction, ConfigurationHandler handler)
{
	DEBUG_STREAM("\n$$$\n");
	starting = transaction->starting();
	// stream.printf("starting.address = %02x\n", starting.address >> 1);
	DEBUG_STREAM("starting.next    = " << starting.next);

	modm::atomic::Lock lock;
	// if we have a place in the queue and the transaction object is valid
	if (queue.isNotFull() && transaction)
	{
		// if the transaction object wants to attach to the queue
		if (transaction->attaching())
		{
			// if no current transaction is taking place
			if (!modm::accessor::asVolatile(::transaction))
			{
				// configure the peripheral if necessary
				if (handler and (configuration != handler)) {
					configuration = handler;
					configuration();
				}

				DEBUG_STREAM("\n###\n");
				::transaction = transaction;
				// start the transaction
				callStarting();
			}
			else
			{
				// queue the transaction for later execution
				queue.push(ConfiguredTransaction(transaction, configuration));
			}
			return true;
		}
		else {
			transaction->detaching(modm::I2c::DetachCause::FailedToAttach);
		}
	}
	return false;
}

modm::I2cMaster::Error
modm::platform::I2cMaster4::getErrorState()
{
	return error;
}