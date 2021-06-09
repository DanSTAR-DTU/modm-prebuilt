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

#ifndef MODM_STM32_UARTHAL_4_HPP
#	error 	"Don't include this file directly, use uart_hal_4.hpp instead!"
#endif
#include <modm/platform/clock/rcc.hpp>
#include <modm/math/algorithm/prescaler.hpp>

namespace modm::platform
{

// ----------------------------------------------------------------------------
void
UartHal4::enable()
{
	Rcc::enable<Peripheral::Uart4>();
}

void
UartHal4::disable()
{
	// TX, RX, Uart, etc. Disable
	UART4->CR1 = 0;
	Rcc::disable<Peripheral::Uart4>();
}

void
UartHal4::enableOperation()
{
	UART4->CR1 |= USART_CR1_UE;
}

void
UartHal4::disableOperation()
{
	UART4->CR1 &= ~USART_CR1_UE;
}

template< class SystemClock, modm::baudrate_t baudrate, modm::percent_t tolerance >
void
UartHal4::initialize(Parity parity, WordLength length)
{
	enable();
	disableOperation();

	constexpr uint32_t max = (1ul << 16) - 1ul;
	constexpr auto result = Prescaler::from_range(SystemClock::Uart4, baudrate, 1, max);
	modm::PeripheralDriver::assertBaudrateInTolerance< result.frequency, baudrate, tolerance >();

	uint32_t cr1 = UART4->CR1;
	// Set baudrate
	UART4->BRR = result.prescaler;
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

	UART4->CR1 = cr1;
}

void
UartHal4::write(uint16_t data)
{
	UART4->TDR = data;
}

void
UartHal4::read(uint8_t &data)
{
	data = UART4->RDR;
}

void
UartHal4::read(uint16_t &data)
{
	data = UART4->RDR;
}

void
UartHal4::setTransmitterEnable(bool enable)
{
	if (enable) {
		UART4->CR1 |=  USART_CR1_TE;
	} else {
		UART4->CR1 &= ~USART_CR1_TE;
	}
}

void
UartHal4::setReceiverEnable(bool enable)
{
	if (enable) {
		UART4->CR1 |=  USART_CR1_RE;
	} else {
		UART4->CR1 &= ~USART_CR1_RE;
	}
}

bool
UartHal4::isReceiveRegisterNotEmpty()
{
	return UART4->ISR & USART_ISR_RXNE;
}

bool
UartHal4::isTransmitRegisterEmpty()
{
	return UART4->ISR & USART_ISR_TXE;
}

void
UartHal4::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(UART4_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ(UART4_IRQn);
	}
	else {
		NVIC_DisableIRQ(UART4_IRQn);
	}
}

void
UartHal4::setInterruptPriority(uint32_t priority)
{
	NVIC_SetPriority(UART4_IRQn, priority);
}

void
UartHal4::enableInterrupt(Interrupt_t interrupt)
{
	UART4->CR1 |= interrupt.value;
}

void
UartHal4::disableInterrupt(Interrupt_t interrupt)
{
	UART4->CR1 &= ~interrupt.value;
}

UartHal4::InterruptFlag_t
UartHal4::getInterruptFlags()
{
	return InterruptFlag_t( UART4->ISR );
}

void
UartHal4::acknowledgeInterruptFlags(InterruptFlag_t flags)
{
	// Flags are cleared by writing a one to the flag position.
	// Writing a zero is (hopefully) ignored.
	UART4->ICR = flags.value;
}

} // namespace modm::platform