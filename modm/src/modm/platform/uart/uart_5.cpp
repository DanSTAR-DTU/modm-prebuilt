/*
 * Copyright (c) 2009, Martin Rosekeit
 * Copyright (c) 2009-2011, Fabian Greif
 * Copyright (c) 2010-2011, 2013, Georgi Grinshpun
 * Copyright (c) 2013-2014, Sascha Schade
 * Copyright (c) 2013, 2016, Kevin Läufer
 * Copyright (c) 2013-2017, Niklas Hauser
 * Copyright (c) 2018, Lucas Mösch
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include "../device.hpp"
#include "uart_hal_5.hpp"
#include "uart_5.hpp"

#include <modm/architecture/interface/atomic_lock.hpp>
#include <modm/architecture/driver/atomic/queue.hpp>

namespace
{
	static modm::atomic::Queue<uint8_t, 16> rxBuffer;
	static modm::atomic::Queue<uint8_t, 250> txBuffer;
}
void
modm::platform::Uart5::initializeBuffered(uint32_t interruptPriority)
{
	UartHal5::enableInterruptVector(true, interruptPriority);
	UartHal5::enableInterrupt(Interrupt::RxNotEmpty);
}
void
modm::platform::Uart5::writeBlocking(uint8_t data)
{
	while(!UartHal5::isTransmitRegisterEmpty());
	UartHal5::write(data);
}

void
modm::platform::Uart5::writeBlocking(const uint8_t *data, std::size_t length)
{
	while (length-- != 0) {
		writeBlocking(*data++);
	}
}

void
modm::platform::Uart5::flushWriteBuffer()
{
	while(!isWriteFinished());
}

bool
modm::platform::Uart5::write(uint8_t data)
{
	if(txBuffer.isEmpty() && UartHal5::isTransmitRegisterEmpty()) {
		UartHal5::write(data);
	} else {
		if (!txBuffer.push(data))
			return false;
		// Disable interrupts while enabling the transmit interrupt
		atomic::Lock lock;
		// Transmit Data Register Empty Interrupt Enable
		UartHal5::enableInterrupt(Interrupt::TxEmpty);
	}
	return true;
}

std::size_t
modm::platform::Uart5::write(const uint8_t *data, std::size_t length)
{
	uint32_t i = 0;
	for (; i < length; ++i)
	{
		if (!write(*data++)) {
			return i;
		}
	}
	return i;
}

bool
modm::platform::Uart5::isWriteFinished()
{
	return txBuffer.isEmpty() && UartHal5::isTransmitRegisterEmpty();
}

std::size_t
modm::platform::Uart5::transmitBufferSize()
{
	return txBuffer.getSize();
}

std::size_t
modm::platform::Uart5::discardTransmitBuffer()
{
	std::size_t count = 0;
	// disable interrupt since buffer will be cleared
	UartHal5::disableInterrupt(UartHal5::Interrupt::TxEmpty);
	while(!txBuffer.isEmpty()) {
		++count;
		txBuffer.pop();
	}
	return count;
}

bool
modm::platform::Uart5::read(uint8_t &data)
{
	if (rxBuffer.isEmpty()) {
		return false;
	} else {
		data = rxBuffer.get();
		rxBuffer.pop();
		return true;
	}
}

std::size_t
modm::platform::Uart5::read(uint8_t *data, std::size_t length)
{
	uint32_t i = 0;
	for (; i < length; ++i)
	{
		if (rxBuffer.isEmpty()) {
			return i;
		} else {
			*data++ = rxBuffer.get();
			rxBuffer.pop();
		}
	}
	return i;
}

std::size_t
modm::platform::Uart5::receiveBufferSize()
{
	return rxBuffer.getSize();
}

std::size_t
modm::platform::Uart5::discardReceiveBuffer()
{
	std::size_t count = 0;
	while(!rxBuffer.isEmpty()) {
		++count;
		rxBuffer.pop();
	}
	return count;
}

bool
modm::platform::Uart5::hasError()
{
	return UartHal5::getInterruptFlags().any(
		UartHal5::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UartHal5::InterruptFlag::NoiseError |
#endif
		UartHal5::InterruptFlag::OverrunError | UartHal5::InterruptFlag::FramingError);
}
void
modm::platform::Uart5::clearError()
{
	return UartHal5::acknowledgeInterruptFlags(
		UartHal5::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UartHal5::InterruptFlag::NoiseError |
#endif
		UartHal5::InterruptFlag::OverrunError | UartHal5::InterruptFlag::FramingError);
}


MODM_ISR(UART5)
{
	if (modm::platform::UartHal5::isReceiveRegisterNotEmpty()) {
		// TODO: save the errors
		uint8_t data;
		modm::platform::UartHal5::read(data);
		rxBuffer.push(data);
	}
	if (modm::platform::UartHal5::isTransmitRegisterEmpty()) {
		if (txBuffer.isEmpty()) {
			// transmission finished, disable TxEmpty interrupt
			modm::platform::UartHal5::disableInterrupt(modm::platform::UartHal5::Interrupt::TxEmpty);
		}
		else {
			modm::platform::UartHal5::write(txBuffer.get());
			txBuffer.pop();
		}
	}
	modm::platform::UartHal5::acknowledgeInterruptFlags(modm::platform::UartHal5::InterruptFlag::OverrunError);
}
