/*
 * Copyright (c) 2013-2014, 2016, Kevin Läufer
 * Copyright (c) 2013-2017, Niklas Hauser
 * Copyright (c) 2017, Fabian Greif
 * Copyright (c) 2017, Sascha Schade
 * Copyright (c) 2018, Christopher Durand
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

#ifndef MODM_STM32_UARTHAL_1_HPP
#	error 	"Don't include this file directly, use uart_hal_1.hpp instead!"
#endif
#include <modm/platform/clock/rcc.hpp>
#include <modm/math/algorithm/prescaler.hpp>

namespace modm::platform
{

// ----------------------------------------------------------------------------
void
UsartHal1::enable()
{
	Rcc::enable<Peripheral::Usart1>();
}

void
UsartHal1::disable()
{
	// TX, RX, Uart, etc. Disable
	USART1->CR1 = 0;
	Rcc::disable<Peripheral::Usart1>();
}

void
UsartHal1::enableOperation()
{
	USART1->CR1 |= USART_CR1_UE;
}

void
UsartHal1::disableOperation()
{
	USART1->CR1 &= ~USART_CR1_UE;
}

template< class SystemClock, modm::baudrate_t baudrate, modm::percent_t tolerance >
void
UsartHal1::initialize(Parity parity, WordLength length)
{
	enable();
	disableOperation();

	constexpr uint32_t max = (1ul << 16) - 1ul;
	constexpr auto result = Prescaler::from_range(SystemClock::Usart1, baudrate, 1, max);
	modm::PeripheralDriver::assertBaudrateInTolerance< result.frequency, baudrate, tolerance >();

	uint32_t cr1 = USART1->CR1;
	// Set baudrate
	USART1->BRR = result.prescaler;
	// Set parity
	cr1 &= ~(USART_CR1_PCE | USART_CR1_PS);
	cr1 |= static_cast<uint32_t>(parity);

	// Set word length
#ifdef USART_CR1_M1
	cr1	&= ~(USART_CR1_M0 | USART_CR1_M1);
#else
	cr1	&= ~USART_CR1_M;
#endif
	cr1 |= static_cast<uint32_t>(length);

	USART1->CR1 = cr1;
}

void
UsartHal1::setSpiClock(SpiClock clk, LastBitClockPulse pulse)
{
	uint32_t cr2 = USART1->CR2;
	cr2 &= ~(USART_CR2_LBCL | USART_CR2_CLKEN);
	cr2 |= static_cast<uint32_t>(clk) | static_cast<uint32_t>(pulse);
	USART1->CR2 = cr2;
}

void
UsartHal1::setSpiDataMode(SpiDataMode mode)
{
	uint32_t cr2 = USART1->CR2;
	cr2 &= ~(USART_CR2_CPOL | USART_CR2_CPHA);
	cr2 |= static_cast<uint32_t>(mode);
	USART1->CR2 = cr2;
}
void
UsartHal1::write(uint16_t data)
{
	USART1->TDR = data;
}

void
UsartHal1::read(uint8_t &data)
{
	data = USART1->RDR;
}

void
UsartHal1::read(uint16_t &data)
{
	data = USART1->RDR;
}

void
UsartHal1::setTransmitterEnable(bool enable)
{
	if (enable) {
		USART1->CR1 |=  USART_CR1_TE;
	} else {
		USART1->CR1 &= ~USART_CR1_TE;
	}
}

void
UsartHal1::setReceiverEnable(bool enable)
{
	if (enable) {
		USART1->CR1 |=  USART_CR1_RE;
	} else {
		USART1->CR1 &= ~USART_CR1_RE;
	}
}

bool
UsartHal1::isReceiveRegisterNotEmpty()
{
	return USART1->ISR & USART_ISR_RXNE;
}

bool
UsartHal1::isTransmitRegisterEmpty()
{
	return USART1->ISR & USART_ISR_TXE;
}

void
UsartHal1::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(USART1_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ(USART1_IRQn);
	}
	else {
		NVIC_DisableIRQ(USART1_IRQn);
	}
}

void
UsartHal1::setInterruptPriority(uint32_t priority)
{
	NVIC_SetPriority(USART1_IRQn, priority);
}

void
UsartHal1::enableInterrupt(Interrupt_t interrupt)
{
	USART1->CR1 |= interrupt.value;
}

void
UsartHal1::disableInterrupt(Interrupt_t interrupt)
{
	USART1->CR1 &= ~interrupt.value;
}

UsartHal1::InterruptFlag_t
UsartHal1::getInterruptFlags()
{
	return InterruptFlag_t( USART1->ISR );
}

void
UsartHal1::acknowledgeInterruptFlags(InterruptFlag_t flags)
{
	// Flags are cleared by writing a one to the flag position.
	// Writing a zero is (hopefully) ignored.
	USART1->ICR = flags.value;
}

} // namespace modm::platform