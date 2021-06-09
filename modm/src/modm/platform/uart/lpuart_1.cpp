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
#include "lpuart_1.hpp"
namespace modm::platform
{

void
Lpuart1::writeBlocking(uint8_t data)
{
	while(!LpuartHal1::isTransmitRegisterEmpty());
	LpuartHal1::write(data);
}

void
Lpuart1::writeBlocking(const uint8_t *data, std::size_t length)
{
	while (length-- != 0) {
		writeBlocking(*data++);
	}
}

void
Lpuart1::flushWriteBuffer()
{
	return;
}

bool
Lpuart1::write(uint8_t data)
{
	if(LpuartHal1::isTransmitRegisterEmpty()) {
		LpuartHal1::write(data);
		return true;
	} else {
		return false;
	}
}

std::size_t
Lpuart1::write(const uint8_t *data, std::size_t length)
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
Lpuart1::isWriteFinished()
{
	return LpuartHal1::isTransmitRegisterEmpty();
}

std::size_t
Lpuart1::transmitBufferSize()
{
	return LpuartHal1::isTransmitRegisterEmpty() ? 0 : 1;
}

std::size_t
Lpuart1::discardTransmitBuffer()
{
	return 0;
}

bool
Lpuart1::read(uint8_t &data)
{
	if(LpuartHal1::isReceiveRegisterNotEmpty()) {
		LpuartHal1::read(data);
		return true;
	} else {
		return false;
	}
}

std::size_t
Lpuart1::read(uint8_t *data, std::size_t length)
{
	(void)length; // avoid compiler warning
	if(read(*data)) {
		return 1;
	} else {
		return 0;
	}
}

std::size_t
Lpuart1::receiveBufferSize()
{
	return LpuartHal1::isReceiveRegisterNotEmpty() ? 1 : 0;
}

std::size_t
Lpuart1::discardReceiveBuffer()
{
	return 0;
}

bool
Lpuart1::hasError()
{
	return LpuartHal1::getInterruptFlags().any(
		LpuartHal1::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		LpuartHal1::InterruptFlag::NoiseError |
#endif
		LpuartHal1::InterruptFlag::OverrunError | LpuartHal1::InterruptFlag::FramingError);
}
void
Lpuart1::clearError()
{
	return LpuartHal1::acknowledgeInterruptFlags(
		LpuartHal1::InterruptFlag::ParityError |
#ifdef USART_ISR_NE
		LpuartHal1::InterruptFlag::NoiseError |
#endif
		LpuartHal1::InterruptFlag::OverrunError | LpuartHal1::InterruptFlag::FramingError);
}

}	// namespace modm::platform

