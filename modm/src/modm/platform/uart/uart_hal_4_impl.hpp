/*
 * Copyright (c) 2013-2014, 2016, Kevin Läufer
 * Copyright (c) 2013-2017, Niklas Hauser
 * Copyright (c) 2017, Fabian Greif
 * Copyright (c) 2017, Sascha Schade
 * Copyright (c) 2018, Christopher Durand
 * Copyright (c) 2018, Lucas Mösch
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

// ----------------------------------------------------------------------------
void
modm::platform::UartHal4::setParity(const Parity parity)
{
	uint32_t flags = UART4->CR1;
	flags &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M);
	flags |= static_cast<uint32_t>(parity);
	if (parity != Parity::Disabled) {
		// Parity Bit counts as 9th bit -> enable 9 data bits
		flags |= USART_CR1_M;
	}
	UART4->CR1 = flags;

}

void
modm::platform::UartHal4::enable()
{
	Rcc::enable<Peripheral::Uart4>();
	UART4->CR1 |= USART_CR1_UE;		// Uart Enable
}

void
modm::platform::UartHal4::disable()
{
	// TX, RX, Uart, etc. Disable
	UART4->CR1 = 0;
	Rcc::disable<Peripheral::Uart4>();
}
template<class SystemClock, modm::baudrate_t baudrate,
		modm::platform::UartHal4::OversamplingMode oversample>
void modm_always_inline
modm::platform::UartHal4::initialize(Parity parity)
{
	initializeWithBrr(UartBaudrate::getBrr<SystemClock::Uart4, baudrate>(),
			parity,
			oversample);
}
template<class SystemClock, modm::baudrate_t baudrate>
void modm_always_inline
modm::platform::UartHal4::initialize(Parity parity)
{
	initializeWithBrr(UartBaudrate::getBrr<SystemClock::Uart4, baudrate>(), parity,
					  UartBaudrate::getOversamplingMode(SystemClock::Uart4, baudrate));
}


void inline
modm::platform::UartHal4::initializeWithBrr(uint16_t brr, Parity parity, OversamplingMode oversample)
{
	enable();
	// DIRTY HACK: disable and reenable uart to be able to set
	//             baud rate as well as parity
	UART4->CR1 &= ~USART_CR1_UE;	// Uart Disable
	// set baudrate
	UART4->BRR = brr;
	setParity(parity);
	setOversamplingMode(oversample);
	UART4->CR1 |=  USART_CR1_UE;	// Uart Reenable
}

void
modm::platform::UartHal4::setOversamplingMode(OversamplingMode mode)
{
	if(mode == OversamplingMode::By16) {
		UART4->CR1 &= ~static_cast<uint32_t>(OversamplingMode::By8);
	} else {
		UART4->CR1 |=  static_cast<uint32_t>(OversamplingMode::By8);
	}

}
void
modm::platform::UartHal4::write(uint8_t data)
{
	UART4->DR = data;
}

void
modm::platform::UartHal4::read(uint8_t &data)
{
	data = UART4->DR;
}

void
modm::platform::UartHal4::setTransmitterEnable(const bool enable)
{
	if (enable) {
		UART4->CR1 |=  USART_CR1_TE;
	} else {
		UART4->CR1 &= ~USART_CR1_TE;
	}
}

void
modm::platform::UartHal4::setReceiverEnable(bool enable)
{
	if (enable) {
		UART4->CR1 |=  USART_CR1_RE;
	} else {
		UART4->CR1 &= ~USART_CR1_RE;
	}
}

void
modm::platform::UartHal4::enableOperation()
{
	UART4->CR1 |= USART_CR1_UE;
}

void
modm::platform::UartHal4::disableOperation()
{
	UART4->CR1 &= ~USART_CR1_UE;
}

bool
modm::platform::UartHal4::isReceiveRegisterNotEmpty()
{
	return UART4->SR & USART_SR_RXNE;
}

bool
modm::platform::UartHal4::isTransmitRegisterEmpty()
{
	return UART4->SR & USART_SR_TXE;
}

void
modm::platform::UartHal4::enableInterruptVector(bool enable, uint32_t priority)
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
modm::platform::UartHal4::enableInterrupt(Interrupt_t interrupt)
{
	UART4->CR1 |= interrupt.value;
}

void
modm::platform::UartHal4::disableInterrupt(Interrupt_t interrupt)
{
	UART4->CR1 &= ~interrupt.value;
}

modm::platform::UartHal4::InterruptFlag_t
modm::platform::UartHal4::getInterruptFlags()
{
	return InterruptFlag_t( UART4->SR );
}

void
modm::platform::UartHal4::acknowledgeInterruptFlags(InterruptFlag_t flags)
{
	/* Interrupts must be cleared manually by accessing SR and DR.
	 * Overrun Interrupt, Noise flag detected, Framing Error, Parity Error
	 * p779: "It is cleared by a software sequence (an read to the
	 * USART_SR register followed by a read to the USART_DR register"
	 */
	if (flags & InterruptFlag::OverrunError) {
		uint32_t tmp;
		tmp = UART4->SR;
		tmp = UART4->DR;
		(void) tmp;
	}
	(void) flags;	// avoid compiler warning
}