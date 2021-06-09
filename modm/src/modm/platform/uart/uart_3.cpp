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
#include "uart_3.hpp"
namespace modm::platform
{

void
Usart3::writeBlocking(uint8_t data)
{
	while(!UsartHal3::isTransmitRegisterEmpty());
	UsartHal3::write(data);
}

void
Usart3::writeBlocking(const uint8_t *data, std::size_t length)
{
	while (length-- != 0) {
		writeBlocking(*data++);
	}
}

void
Usart3::flushWriteBuffer()
{
	return;
}

bool
Usart3::write(uint8_t data)
{
	if(UsartHal3::isTransmitRegisterEmpty()) {
		UsartHal3::write(data);
		return true;
	} else {
		return false;
	}
}

std::size_t
Usart3::write(const uint8_t *data, std::size_t length)
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
Usart3::isWriteFinished()
{
	return UsartHal3::isTransmitRegisterEmpty();
}

std::size_t
Usart3::transmitBufferSize()
{
	return UsartHal3::isTransmitRegisterEmpty() ? 0 : 1;
}

std::size_t
Usart3::discardTransmitBuffer()
{
	return 0;
}

bool
Usart3::read(uint8_t &data)
{
	if(UsartHal3::isReceiveRegisterNotEmpty()) {
		UsartHal3::read(data);
		return true;
	} else {
		return false;
	}
}

std::size_t
Usart3::read(uint8_t *data, std::size_t length)
{
	(void)length; // avoid compiler warning
	if(read(*data)) {
		return 1;
	} else {
		return 0;
	}
}

std::size_t
Usart3::receiveBufferSize()
{
	return UsartHal3::isReceiveRegisterNotEmpty() ? 1 : 0;
}

std::size_t
Usart3::discardReceiveBuffer()
{
	return 0;
}

bool
Usart3::hasError()
{
	return UsartHal3::getInterruptFlags().any(
		UsartHal3::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UsartHal3::InterruptFlag::NoiseError |
#endif
		UsartHal3::InterruptFlag::OverrunError | UsartHal3::InterruptFlag::FramingError);
}
void
Usart3::clearError()
{
	return UsartHal3::acknowledgeInterruptFlags(
		UsartHal3::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		UsartHal3::InterruptFlag::NoiseError |
#endif
		UsartHal3::InterruptFlag::OverrunError | UsartHal3::InterruptFlag::FramingError);
}

}	// namespace modm::platform

