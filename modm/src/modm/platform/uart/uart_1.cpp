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
#include "uart_1.hpp"
namespace modm::platform
{

void
Usart1::writeBlocking(uint8_t data)
{
	while(!UsartHal1::isTransmitRegisterEmpty());
	UsartHal1::write(data);
}

void
Usart1::writeBlocking(const uint8_t *data, std::size_t length)
{
	while (length-- != 0) {
		writeBlocking(*data++);
	}
}

void
Usart1::flushWriteBuffer()
{
	return;
}

bool
Usart1::write(uint8_t data)
{
	if(UsartHal1::isTransmitRegisterEmpty()) {
		UsartHal1::write(data);
		return true;
	} else {
		return false;
	}
}

std::size_t
Usart1::write(const uint8_t *data, std::size_t length)
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
Usart1::isWriteFinished()
{
	return UsartHal1::isTransmitRegisterEmpty();
}

std::size_t
Usart1::transmitBufferSize()
{
	return UsartHal1::isTransmitRegisterEmpty() ? 0 : 1;
}

std::size_t
Usart1::discardTransmitBuffer()
{
	return 0;
}

bool
Usart1::read(uint8_t &data)
{
	if(UsartHal1::isReceiveRegisterNotEmpty()) {
		UsartHal1::read(data);
		return true;
	} else {
		return false;
	}
}

std::size_t
Usart1::read(uint8_t *data, std::size_t length)
{
	(void)length; // avoid compiler warning
	if(read(*data)) {
		return 1;
	} else {
		return 0;
	}
}

std::size_t
Usart1::receiveBufferSize()
{
	return UsartHal1::isReceiveRegisterNotEmpty() ? 1 : 0;
}

std::size_t
Usart1::discardReceiveBuffer()
{
	return 0;
}

bool
Usart1::hasError()
{
	return UsartHal1::getInterruptFlags().any(
		UsartHal1::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UsartHal1::InterruptFlag::NoiseError |
#endif
		UsartHal1::InterruptFlag::OverrunError | UsartHal1::InterruptFlag::FramingError);
}
void
Usart1::clearError()
{
	return UsartHal1::acknowledgeInterruptFlags(
		UsartHal1::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UsartHal1::InterruptFlag::NoiseError |
#endif
		UsartHal1::InterruptFlag::OverrunError | UsartHal1::InterruptFlag::FramingError);
}

}	// namespace modm::platform

