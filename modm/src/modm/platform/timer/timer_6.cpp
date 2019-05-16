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

#include "timer_6.hpp"
#include <modm/platform/clock/rcc.hpp>

void
modm::platform::Timer6::enable()
{
	Rcc::enable<Peripheral::Tim6>();
}

void
modm::platform::Timer6::disable()
{
	TIM6->CR1 = 0;
	TIM6->DIER = 0;

	Rcc::disable<Peripheral::Tim6>();
}

void
modm::platform::Timer6::setMode(Mode mode)
{
	// ARR Register is buffered, only Under/Overflow generates update interrupt
	TIM6->CR1 = TIM_CR1_ARPE | TIM_CR1_URS | static_cast<uint32_t>(mode);
	TIM6->CR2 = 0;
}

void
modm::platform::Timer6::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable)
	{
		NVIC_SetPriority(TIM6_DAC_IRQn, priority);
		NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}
	else
	{
		NVIC_DisableIRQ(TIM6_DAC_IRQn);
	}
}