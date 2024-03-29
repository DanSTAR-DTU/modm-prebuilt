/*
 * Copyright (c) 2009, Martin Rosekeit
 * Copyright (c) 2009-2011, Fabian Greif
 * Copyright (c) 2010-2011, 2013, Georgi Grinshpun
 * Copyright (c) 2013-2014, Sascha Schade
 * Copyright (c) 2013, 2016, Kevin Läufer
 * Copyright (c) 2013-2017, Niklas Hauser
 * Copyright (c) 2018, Lucas Mösch
 * Copyright (c) 2021, Raphael Lehmann
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include "../device.hpp"
#include "uart_5.hpp"
#include <modm/architecture/interface/atomic_lock.hpp>
#include <modm/architecture/driver/atomic/queue.hpp>

namespace
{
	static modm::atomic::Queue<uint8_t, 256> rxBuffer;
	static modm::atomic::Queue<uint8_t, 256> txBuffer;
	static modm::platform::UartBase::InterruptFlag_t flags;
}
namespace modm::platform
{

void
Uart5::writeBlocking(uint8_t data)
{
	while(!UartHal5::isTransmitRegisterEmpty());
	UartHal5::write(data);
}

void
Uart5::writeBlocking(const uint8_t *data, std::size_t length)
{
	while (length-- != 0) {
		writeBlocking(*data++);
	}
}

void
Uart5::flushWriteBuffer()
{
	while(!isWriteFinished());
}

bool
Uart5::write(uint8_t data)
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
Uart5::write(const uint8_t *data, std::size_t length)
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
Uart5::isWriteFinished()
{
	return txBuffer.isEmpty() && UartHal5::isTransmitRegisterEmpty();
}

std::size_t
Uart5::transmitBufferSize()
{
	return txBuffer.getSize();
}

std::size_t
Uart5::discardTransmitBuffer()
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
Uart5::read(uint8_t &data)
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
Uart5::read(uint8_t *data, std::size_t length)
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
Uart5::receiveBufferSize()
{
	return rxBuffer.getSize();
}

std::size_t
Uart5::discardReceiveBuffer()
{
	std::size_t count = 0;
	while(!rxBuffer.isEmpty()) {
		++count;
		rxBuffer.pop();
	}
	return count;
}

bool
Uart5::hasError()
{
	return UartHal5::getInterruptFlags().any(
		UartHal5::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UartHal5::InterruptFlag::NoiseError |
#endif
		UartHal5::InterruptFlag::OverrunError | UartHal5::InterruptFlag::FramingError);
}
void
Uart5::clearError()
{
	return UartHal5::acknowledgeInterruptFlags(
		UartHal5::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UartHal5::InterruptFlag::NoiseError |
#endif
		UartHal5::InterruptFlag::OverrunError | UartHal5::InterruptFlag::FramingError);
}

bool Uart5::overrunErrorOccurred()
{
	if(flags & modm::platform::UartHal5::InterruptFlag::OverrunError)
	 	return true;
	else
		return false;
}

void Uart5::clearOverrunErrorOccurred()
{
	flags &= (~modm::platform::UartHal5::InterruptFlag::OverrunError);
}


}	// namespace modm::platform

MODM_ISR(UART5)
{
	using namespace modm::platform;
	if (UartHal5::isReceiveRegisterNotEmpty()) {
		// TODO: save the errors
		uint8_t data;
		UartHal5::read(data);
		rxBuffer.push(data);
	}
	if (UartHal5::isTransmitRegisterEmpty()) {
		if (txBuffer.isEmpty()) {
			// transmission finished, disable TxEmpty interrupt
			UartHal5::disableInterrupt(UartHal5::Interrupt::TxEmpty);
		}
		else {
			UartHal5::write(txBuffer.get());
			txBuffer.pop();
		}
	}
	if(modm::platform::UartHal5::getInterruptFlags() & modm::platform::UartHal5::InterruptFlag::OverrunError)
	{
		modm::platform::UartHal5::acknowledgeInterruptFlags(modm::platform::UartHal5::InterruptFlag::OverrunError);
		flags |= modm::platform::UartHal5::InterruptFlag::OverrunError;
	}
}
