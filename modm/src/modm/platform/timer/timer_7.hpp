/*
 * Copyright (c) 2009, Georgi Grinshpun
 * Copyright (c) 2009-2013, Fabian Greif
 * Copyright (c) 2010, Martin Rosekeit
 * Copyright (c) 2011, 2014-2017, Niklas Hauser
 * Copyright (c) 2013-2014, Kevin Läufer
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_TIMER_7_HPP
#define MODM_STM32_TIMER_7_HPP

#include "basic_base.hpp"
#include <modm/platform/gpio/connector.hpp>

namespace modm
{

namespace platform
{

/**
 * @brief		Basic Timer 7
 *
 * Interrupt handler:
 * \code
 * MODM_ISR(TIM7)
 * {
 *     Timer7::resetInterruptFlags(Timer7::FLAG_UPDATE);
 *
 *     ...
 * }
 * \endcode
 *
 
 *
 * @author		Fabian Greif
 * @ingroup		modm_platform_timer
 */
class Timer7 : public BasicTimer
{
public:
	template< template<Peripheral _> class... Signals >
	static void
	connect()
	{
		using Connector = GpioConnector<Peripheral::Tim7, Signals...>;
		Connector::connect();
	}

	static void
	enable();

	static void
	disable();

	static inline void
	pause()
	{
		TIM7->CR1 &= ~TIM_CR1_CEN;
	}

	static inline void
	start()
	{
		TIM7->CR1 |= TIM_CR1_CEN;
	}

	static void
	setMode(Mode mode);

	static inline void
	setPrescaler(uint16_t prescaler)
	{
		// Because a prescaler of zero is not possible the actual
		// prescaler value is \p prescaler - 1 (see Datasheet)
		TIM7->PSC = prescaler - 1;
	}

	static inline void
	setOverflow(uint16_t overflow)
	{
		TIM7->ARR = overflow;
	}

	template<class SystemClock>
	static uint16_t
	setPeriod(uint32_t microseconds, bool autoApply = true)
	{
		// This will be inaccurate for non-smooth frequencies (last six digits
		// unequal to zero)
		uint32_t cycles = microseconds * (SystemClock::Timer7 / 1000000UL);

		uint16_t prescaler = (cycles + 65535) / 65536;	// always round up
		uint16_t overflow = cycles / prescaler;

		overflow = overflow - 1;	// e.g. 36000 cycles are from 0 to 35999
		setPrescaler(prescaler);
		setOverflow(overflow);

		if (autoApply) {
			// Generate Update Event to apply the new settings for ARR
			TIM7->EGR |= TIM_EGR_UG;
		}

		return overflow;
	}

	static inline void
	applyAndReset()
	{
		// Generate Update Event to apply the new settings for ARR
		TIM7->EGR |= TIM_EGR_UG;
	}

	static inline uint16_t
	getValue()
	{
		return TIM7->CNT;
	}

	static inline void
	setValue(uint16_t value)
	{
		TIM7->CNT = value;
	}

	static void
	enableInterruptVector(bool enable, uint32_t priority);

	static inline void
	enableInterrupt(Interrupt_t interrupt)
	{
		TIM7->DIER |= interrupt.value;
	}

	static inline void
	disableInterrupt(Interrupt_t interrupt)
	{
		TIM7->DIER &= ~interrupt.value;
	}

	static inline InterruptFlag_t
	getInterruptFlags()
	{
		return InterruptFlag_t(TIM7->SR);
	}

	static void
	acknowledgeInterruptFlags(InterruptFlag_t interrupt)
	{
		// Flags are cleared by writing a zero to the flag position.
		// Writing a one is ignored.
		TIM7->SR = ~interrupt.value;
	}
};

}	// namespace platform

}	// namespace modm

#endif // MODM_STM32_TIMER_7_HPP