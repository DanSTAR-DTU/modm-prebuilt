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

#ifndef MODM_STM32_UARTHAL_5_HPP
#	error 	"Don't include this file directly, use uart_hal_5.hpp instead!"
#endif
#include <modm/platform/clock/rcc.hpp>

// ----------------------------------------------------------------------------
void
modm::platform::UartHal5::setParity(const Parity parity)
{
	const bool usartEnabled = (UART5->CR1 & USART_CR1_UE);
	if(usartEnabled) {
		disableOperation();
	}
	uint32_t flags = UART5->CR1;
	flags &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M);
	flags |= static_cast<uint32_t>(parity);
	if (parity != Parity::Disabled) {
		// Parity Bit counts as 9th bit -> enable 9 data bits
		flags |= USART_CR1_M;
	}
	UART5->CR1 = flags;

	if(usartEnabled) {
		enableOperation();
	}
}

void
modm::platform::UartHal5::enable()
{
	Rcc::enable<Peripheral::Uart5>();
	UART5->CR1 |= USART_CR1_UE;		// Uart Enable
}

void
modm::platform::UartHal5::disable()
{
	// TX, RX, Uart, etc. Disable
	UART5->CR1 = 0;
	Rcc::disable<Peripheral::Uart5>();
}
template<class SystemClock, modm::baudrate_t baudrate,
		modm::platform::UartHal5::OversamplingMode oversample>
void modm_always_inline
modm::platform::UartHal5::initialize(Parity parity)
{
	initializeWithBrr(UartBaudrate::getBrr<SystemClock::Uart5, baudrate>(),
			parity,
			oversample);
}
template<class SystemClock, modm::baudrate_t baudrate>
void modm_always_inline
modm::platform::UartHal5::initialize(Parity parity)
{
	initializeWithBrr(UartBaudrate::getBrr<SystemClock::Uart5, baudrate>(), parity,
					  UartBaudrate::getOversamplingMode(SystemClock::Uart5, baudrate));
}


void inline
modm::platform::UartHal5::initializeWithBrr(uint16_t brr, Parity parity, OversamplingMode oversample)
{
	enable();
	// DIRTY HACK: disable and reenable uart to be able to set
	//             baud rate as well as parity
	UART5->CR1 &= ~USART_CR1_UE;	// Uart Disable
	// set baudrate
	UART5->BRR = brr;
	setParity(parity);
	setOversamplingMode(oversample);
	UART5->CR1 |=  USART_CR1_UE;	// Uart Reenable
}

void
modm::platform::UartHal5::setOversamplingMode(OversamplingMode mode)
{
	const bool usartEnabled = (UART5->CR1 & USART_CR1_UE);
	if(usartEnabled) {
		disableOperation();
	}
	if(mode == OversamplingMode::By16) {
		UART5->CR1 &= ~static_cast<uint32_t>(OversamplingMode::By8);
	} else {
		UART5->CR1 |=  static_cast<uint32_t>(OversamplingMode::By8);
	}

	if(usartEnabled) {
		enableOperation();
	}
}
void
modm::platform::UartHal5::write(uint8_t data)
{
	UART5->TDR = data;
}

void
modm::platform::UartHal5::read(uint8_t &data)
{
	data = UART5->RDR;
}

void
modm::platform::UartHal5::setTransmitterEnable(const bool enable)
{
	if (enable) {
		UART5->CR1 |=  USART_CR1_TE;
	} else {
		UART5->CR1 &= ~USART_CR1_TE;
	}
}

void
modm::platform::UartHal5::setReceiverEnable(bool enable)
{
	if (enable) {
		UART5->CR1 |=  USART_CR1_RE;
	} else {
		UART5->CR1 &= ~USART_CR1_RE;
	}
}

void
modm::platform::UartHal5::enableOperation()
{
	UART5->CR1 |= USART_CR1_UE;
}

void
modm::platform::UartHal5::disableOperation()
{
	UART5->CR1 &= ~USART_CR1_UE;
}

bool
modm::platform::UartHal5::isReceiveRegisterNotEmpty()
{
	return UART5->ISR & USART_ISR_RXNE;
}

bool
modm::platform::UartHal5::isTransmitRegisterEmpty()
{
	return UART5->ISR & USART_ISR_TXE;
}

void
modm::platform::UartHal5::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(UART5_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ(UART5_IRQn);
	}
	else {
		NVIC_DisableIRQ(UART5_IRQn);
	}
}

void
modm::platform::UartHal5::enableInterrupt(Interrupt_t interrupt)
{
	UART5->CR1 |= interrupt.value;
}

void
modm::platform::UartHal5::disableInterrupt(Interrupt_t interrupt)
{
	UART5->CR1 &= ~interrupt.value;
}

modm::platform::UartHal5::InterruptFlag_t
modm::platform::UartHal5::getInterruptFlags()
{
	return InterruptFlag_t( UART5->ISR );
}

void
modm::platform::UartHal5::acknowledgeInterruptFlags(InterruptFlag_t flags)
{
	// Not all flags can be cleared by writing to this reg
#ifdef USART_ICR_NECF
#define USART_ICR_NCF USART_ICR_NECF
#endif
	const uint32_t mask = USART_ICR_PECF  | USART_ICR_FECF   |
		USART_ICR_NCF   | USART_ICR_ORECF | USART_ICR_IDLECF |
		USART_ICR_TCCF  | USART_ICR_CTSCF | USART_ICR_RTOCF  |
		USART_ICR_CMCF
#ifdef USART_ICR_LBDCF // F0x0 do not have LIN mode!
		| USART_ICR_LBDCF
#endif
#ifdef USART_ICR_EOBCF // F0x0 do not have Smartcard mode!
		| USART_ICR_EOBCF
#endif
#ifdef USART_ICR_WUCF
		| USART_ICR_WUCF
#endif
		;
	// Flags are cleared by writing a one to the flag position.
	// Writing a zero is (hopefully) ignored.
	UART5->ICR = (flags.value & mask);
}