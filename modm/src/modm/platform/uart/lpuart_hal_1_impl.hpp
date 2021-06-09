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

#ifndef MODM_STM32_LPUARTHAL_1_HPP
#	error 	"Don't include this file directly, use lpuart_hal_1.hpp instead!"
#endif
#include <modm/platform/clock/rcc.hpp>
#include <modm/math/algorithm/prescaler.hpp>

namespace modm::platform
{

// ----------------------------------------------------------------------------
void
LpuartHal1::enable()
{
	Rcc::enable<Peripheral::Lpuart1>();
}

void
LpuartHal1::disable()
{
	// TX, RX, Uart, etc. Disable
	LPUART1->CR1 = 0;
	Rcc::disable<Peripheral::Lpuart1>();
}

void
LpuartHal1::enableOperation()
{
	LPUART1->CR1 |= USART_CR1_UE;
}

void
LpuartHal1::disableOperation()
{
	LPUART1->CR1 &= ~USART_CR1_UE;
}

template< class SystemClock, modm::baudrate_t baudrate, modm::percent_t tolerance >
void
LpuartHal1::initialize(Parity parity, WordLength length)
{
	enable();
	disableOperation();

	static_assert(SystemClock::Lpuart1 >= baudrate * 3,
		"fck must be in the range [3 x baud rate, 4096 x baud rate].");
	static_assert(SystemClock::Lpuart1 <= uint64_t(baudrate) * 4096,
		"fck must be in the range [3 x baud rate, 4096 x baud rate].");
	constexpr uint32_t max = (1ul << 20) - 1ul;
	constexpr auto result = GenericPrescaler<uint64_t>::from_range(
		uint64_t(SystemClock::Lpuart1) * 256,
		baudrate,
		0x300, max);
	modm::PeripheralDriver::assertBaudrateInTolerance< result.frequency, baudrate, tolerance >();

	uint32_t cr1 = LPUART1->CR1;
	// Set baudrate
	LPUART1->BRR = result.prescaler;
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

	LPUART1->CR1 = cr1;
}

void
LpuartHal1::write(uint16_t data)
{
	LPUART1->TDR = data;
}

void
LpuartHal1::read(uint8_t &data)
{
	data = LPUART1->RDR;
}

void
LpuartHal1::read(uint16_t &data)
{
	data = LPUART1->RDR;
}

void
LpuartHal1::setTransmitterEnable(bool enable)
{
	if (enable) {
		LPUART1->CR1 |=  USART_CR1_TE;
	} else {
		LPUART1->CR1 &= ~USART_CR1_TE;
	}
}

void
LpuartHal1::setReceiverEnable(bool enable)
{
	if (enable) {
		LPUART1->CR1 |=  USART_CR1_RE;
	} else {
		LPUART1->CR1 &= ~USART_CR1_RE;
	}
}

bool
LpuartHal1::isReceiveRegisterNotEmpty()
{
	return LPUART1->ISR & USART_ISR_RXNE;
}

bool
LpuartHal1::isTransmitRegisterEmpty()
{
	return LPUART1->ISR & USART_ISR_TXE;
}

void
LpuartHal1::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(LPUART1_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ(LPUART1_IRQn);
	}
	else {
		NVIC_DisableIRQ(LPUART1_IRQn);
	}
}

void
LpuartHal1::setInterruptPriority(uint32_t priority)
{
	NVIC_SetPriority(LPUART1_IRQn, priority);
}

void
LpuartHal1::enableInterrupt(Interrupt_t interrupt)
{
	LPUART1->CR1 |= interrupt.value;
}

void
LpuartHal1::disableInterrupt(Interrupt_t interrupt)
{
	LPUART1->CR1 &= ~interrupt.value;
}

LpuartHal1::InterruptFlag_t
LpuartHal1::getInterruptFlags()
{
	return InterruptFlag_t( LPUART1->ISR );
}

void
LpuartHal1::acknowledgeInterruptFlags(InterruptFlag_t flags)
{
	// Flags are cleared by writing a one to the flag position.
	// Writing a zero is (hopefully) ignored.
	LPUART1->ICR = flags.value;
}

} // namespace modm::platform