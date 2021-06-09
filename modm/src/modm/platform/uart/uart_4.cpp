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
#include "uart_4.hpp"
namespace modm::platform
{

void
Uart4::writeBlocking(uint8_t data)
{
	while(!UartHal4::isTransmitRegisterEmpty());
	UartHal4::write(data);
}

void
Uart4::writeBlocking(const uint8_t *data, std::size_t length)
{
	while (length-- != 0) {
		writeBlocking(*data++);
	}
}

void
Uart4::flushWriteBuffer()
{
	return;
}

bool
Uart4::write(uint8_t data)
{
	if(UartHal4::isTransmitRegisterEmpty()) {
		UartHal4::write(data);
		return true;
	} else {
		return false;
	}
}

std::size_t
Uart4::write(const uint8_t *data, std::size_t length)
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
Uart4::isWriteFinished()
{
	return UartHal4::isTransmitRegisterEmpty();
}

std::size_t
Uart4::transmitBufferSize()
{
	return UartHal4::isTransmitRegisterEmpty() ? 0 : 1;
}

std::size_t
Uart4::discardTransmitBuffer()
{
	return 0;
}

bool
Uart4::read(uint8_t &data)
{
	if(UartHal4::isReceiveRegisterNotEmpty()) {
		UartHal4::read(data);
		return true;
	} else {
		return false;
	}
}

std::size_t
Uart4::read(uint8_t *data, std::size_t length)
{
	(void)length; // avoid compiler warning
	if(read(*data)) {
		return 1;
	} else {
		return 0;
	}
}

std::size_t
Uart4::receiveBufferSize()
{
	return UartHal4::isReceiveRegisterNotEmpty() ? 1 : 0;
}

std::size_t
Uart4::discardReceiveBuffer()
{
	return 0;
}

bool
Uart4::hasError()
{
	return UartHal4::getInterruptFlags().any(
		UartHal4::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UartHal4::InterruptFlag::NoiseError |
#endif
		UartHal4::InterruptFlag::OverrunError | UartHal4::InterruptFlag::FramingError);
}
void
Uart4::clearError()
{
	return UartHal4::acknowledgeInterruptFlags(
		UartHal4::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UartHal4::InterruptFlag::NoiseError |
#endif
		UartHal4::InterruptFlag::OverrunError | UartHal4::InterruptFlag::FramingError);
}

}	// namespace modm::platform

