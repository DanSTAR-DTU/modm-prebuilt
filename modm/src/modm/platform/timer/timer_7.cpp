/*
 * Copyright (c) 2009, Martin Rosekeit
 * Copyright (c) 2009-2012, 2016-2017, Fabian Greif
 * Copyright (c) 2011, Georgi Grinshpun
 * Copyright (c) 2013, Kevin Läufer
 * Copyright (c) 2014, Sascha Schade
 * Copyright (c) 2014-2017, Niklas Hauser
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include "timer_7.hpp"
#include <modm/platform/clock/rcc.hpp>

void
modm::platform::Timer7::enable()
{
	Rcc::enable<Peripheral::Tim7>();
}

void
modm::platform::Timer7::disable()
{
	TIM7->CR1 = 0;
	TIM7->DIER = 0;

	Rcc::disable<Peripheral::Tim7>();
}

void
modm::platform::Timer7::setMode(Mode mode)
{
	// ARR Register is buffered, only Under/Overflow generates update interrupt
	TIM7->CR1 = TIM_CR1_ARPE | TIM_CR1_URS | static_cast<uint32_t>(mode);
	TIM7->CR2 = 0;
}

void
modm::platform::Timer7::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable)
	{
		NVIC_SetPriority(TIM7_DAC_IRQn, priority);
		NVIC_EnableIRQ(TIM7_DAC_IRQn);
	}
	else
	{
		NVIC_DisableIRQ(TIM7_DAC_IRQn);
	}
}