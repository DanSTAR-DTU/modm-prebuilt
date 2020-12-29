/*
 * Copyright (c) 2013-2014, Kevin Läufer
 * Copyright (c) 2014, Sascha Schade
 * Copyright (c) 2014, 2016-2017, Niklas Hauser
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32F3_ADC3_HPP
#	error 	"Don't include this file directly, use 'adc_3.hpp' instead!"
#endif

#include <modm/architecture/interface/delay.hpp>	// modm::delayMicroseconds
#include <modm/platform/clock/rcc.hpp>

void
modm::platform::Adc3::initialize(const ClockMode clk,
		const ClockSource clk_src,
		const Prescaler pre,
		const CalibrationMode cal, const bool blocking)
{
	uint32_t tmp = 0;

	// enable clock
	RCC->AHB2ENR |= RCC_AHB2ENR_ADC345EN;
	// select clock source
	RCC->CCIPR |= static_cast<uint32_t>(clk_src);
	// Disable deep power down
	ADC3->CR &= ~ADC_CR_DEEPPWD;
	// reset ADC
	// FIXME: not a good idea since you can only reset both
	// ADC1/ADC2 or ADC3/ADC4 at once ....

	// set ADC "analog" clock source
	if (clk != ClockMode::DoNotChange) {
		if (clk == ClockMode::Asynchronous) {
			setPrescaler(pre);
		}
		tmp  =  ADC345_COMMON->CCR;
		tmp &= ~ADC_CCR_CKMODE;
		tmp |=  static_cast<uint32_t>(clk);
		ADC345_COMMON->CCR = tmp;
	}

	// enable regulator
	ADC3->CR &= ~ADC_CR_ADVREGEN;
	ADC3->CR |= static_cast<uint32_t>(VoltageRegulatorState::Enabled);
	modm::delay_us(10);	// FIXME: this is ugly -> find better solution

	// Clear ready flag
	ADC3->ISR |= ADC_ISR_ADRDY;

	calibrate(cal, true);	// blocking calibration

	ADC3->CR |= ADC_CR_ADEN;
	if (blocking) {
		while(not isReady());
		acknowledgeInterruptFlag(InterruptFlag::Ready);
	}
}

void
modm::platform::Adc3::disable(const bool blocking)
{
	ADC3->CR |= ADC_CR_ADDIS;
	if (blocking) {
		// wait for ADC_CR_ADDIS to be cleared by hw
		while(ADC3->CR & ADC_CR_ADDIS);
	}
	// disable clock
	RCC->AHB2ENR &= ~RCC_AHB2ENR_ADC345EN;
}

void
modm::platform::Adc3::setPrescaler(const Prescaler pre)
{
	uint32_t tmp;
	tmp  = ADC345_COMMON->CCR;
	tmp &= ~static_cast<uint32_t>(Prescaler::Div256AllBits);
	tmp |=  static_cast<uint32_t>(pre);
	ADC345_COMMON->CCR = tmp;
}

bool
modm::platform::Adc3::isReady()
{
	return static_cast<bool>(getInterruptFlags() & InterruptFlag::Ready);
}

void
modm::platform::Adc3::calibrate(const CalibrationMode mode,
									const bool blocking)
{
	if (mode != CalibrationMode::DoNotCalibrate) {
		ADC3->CR |= ADC_CR_ADCAL |
										static_cast<uint32_t>(mode);
		if(blocking) {
			// wait for ADC_CR_ADCAL to be cleared by hw
			while(ADC3->CR & ADC_CR_ADCAL);
		}
	}
}

void
modm::platform::Adc3::setLeftAdjustResult(const bool enable)
{
	if (enable) {
		ADC3->CFGR |= ADC_CFGR_ALIGN;
	}
	else {
		ADC3->CFGR &= ~ADC_CFGR_ALIGN;
	}
}

bool
modm::platform::Adc3::setChannel(	const Channel channel,
										const SampleTime sampleTime)
{
	if(static_cast<uint8_t>(channel) > 18) {
		return false;
	}

	uint32_t tmpreg;
	// SQR1[10:6] contain SQ1[4:0]
	ADC3->SQR1 = (static_cast<uint8_t>(channel) & 0b11111) << 6;

	if (static_cast<uint8_t>(channel) < 10) {
		tmpreg = ADC3->SMPR1
			& ((~ADC_SMPR1_SMP0) << (static_cast<uint8_t>(channel) * 3));
		tmpreg |= static_cast<uint32_t>(sampleTime) <<
						(static_cast<uint8_t>(channel) * 3);
		ADC3->SMPR1 = tmpreg;
	}
	else {
		tmpreg = ADC3->SMPR2
			& ((~ADC_SMPR2_SMP10) << ((static_cast<uint8_t>(channel)-10) * 3));
		tmpreg |= static_cast<uint32_t>(sampleTime) <<
						((static_cast<uint8_t>(channel)-10) * 3);
		ADC3->SMPR2 = tmpreg;
	}
	return true;
}

void
modm::platform::Adc3::setFreeRunningMode(const bool enable)
{
	if (enable) {
		ADC3->CFGR |=  ADC_CFGR_CONT; // set to continuous mode
	} else {
		ADC3->CFGR &= ~ADC_CFGR_CONT; // set to single mode
	}
}


void
modm::platform::Adc3::startConversion(void)
{
	// TODO: maybe add more interrupt flags
	acknowledgeInterruptFlag(InterruptFlag::EndOfRegularConversion |
			InterruptFlag::EndOfSampling | InterruptFlag::Overrun);
	// starts single conversion for the regular group
	ADC3->CR |= ADC_CR_ADSTART;
}

bool
modm::platform::Adc3::isConversionFinished(void)
{
	return static_cast<bool>(getInterruptFlags() & InterruptFlag::EndOfRegularConversion);
}

// ----------------------------------------------------------------------------
// TODO: move this to some shared header for all cortex m3 platforms
// Re-implemented here to save some code space. As all arguments in the calls
// below are constant the compiler is able to calculate everything at
// compile time.

#ifndef MODM_CUSTOM_NVIC_FUNCTIONS
#define MODM_CUSTOM_NVIC_FUNCTIONS

static modm_always_inline void
nvicEnableInterrupt(const IRQn_Type IRQn)
{
	NVIC->ISER[(static_cast<uint32_t>(IRQn) >> 5)] =
								(1 << ((uint32_t)(IRQn) & 0x1F));
}

static modm_always_inline void
nvicDisableInterrupt(IRQn_Type IRQn)
{
	NVIC_DisableIRQ(IRQn);
}

#endif // MODM_CUSTOM_NVIC_FUNCTIONS

void
modm::platform::Adc3::enableInterruptVector(const uint32_t priority,
												const bool enable)
{
	const IRQn_Type INTERRUPT_VECTOR = ADC3_IRQn;
	if (enable) {
		NVIC_SetPriority(INTERRUPT_VECTOR, priority);
		nvicEnableInterrupt(INTERRUPT_VECTOR);
	} else {
		NVIC_DisableIRQ(INTERRUPT_VECTOR);
	}
}

void
modm::platform::Adc3::enableInterrupt(const Interrupt_t interrupt)
{
	ADC3->IER |= interrupt.value;
}

void
modm::platform::Adc3::disableInterrupt(const Interrupt_t interrupt)
{
	ADC3->IER &= ~interrupt.value;
}

modm::platform::Adc3::InterruptFlag_t
modm::platform::Adc3::getInterruptFlags()
{
	return InterruptFlag_t(ADC3->ISR);
}

void
modm::platform::Adc3::acknowledgeInterruptFlag(const InterruptFlag_t flags)
{
	// Flags are cleared by writing a one to the flag position.
	// Writing a zero is ignored.
	ADC3->ISR = flags.value;
}