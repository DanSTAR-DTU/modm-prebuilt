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

#ifndef MODM_STM32_UARTHAL_6_HPP
#	error 	"Don't include this file directly, use uart_hal_6.hpp instead!"
#endif
#include <modm/platform/clock/rcc.hpp>

// ----------------------------------------------------------------------------
void
modm::platform::UsartHal6::setParity(const Parity parity)
{
	const bool usartEnabled = (USART6->CR1 & USART_CR1_UE);
	if(usartEnabled) {
		disableOperation();
	}
	uint32_t flags = USART6->CR1;
	flags &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M);
	flags |= static_cast<uint32_t>(parity);
	if (parity != Parity::Disabled) {
		// Parity Bit counts as 9th bit -> enable 9 data bits
		flags |= USART_CR1_M;
	}
	USART6->CR1 = flags;

	if(usartEnabled) {
		enableOperation();
	}
}

void
modm::platform::UsartHal6::enable()
{
	Rcc::enable<Peripheral::Usart6>();
	USART6->CR1 |= USART_CR1_UE;		// Uart Enable
}

void
modm::platform::UsartHal6::disable()
{
	// TX, RX, Uart, etc. Disable
	USART6->CR1 = 0;
	Rcc::disable<Peripheral::Usart6>();
}
template<class SystemClock, modm::baudrate_t baudrate,
		modm::platform::UsartHal6::OversamplingMode oversample>
void modm_always_inline
modm::platform::UsartHal6::initialize(Parity parity)
{
	initializeWithBrr(UartBaudrate::getBrr<SystemClock::Usart6, baudrate>(),
			parity,
			oversample);
}
template<class SystemClock, modm::baudrate_t baudrate>
void modm_always_inline
modm::platform::UsartHal6::initialize(Parity parity)
{
	initializeWithBrr(UartBaudrate::getBrr<SystemClock::Usart6, baudrate>(), parity,
					  UartBaudrate::getOversamplingMode(SystemClock::Usart6, baudrate));
}


void inline
modm::platform::UsartHal6::initializeWithBrr(uint16_t brr, Parity parity, OversamplingMode oversample)
{
	enable();
	// DIRTY HACK: disable and reenable uart to be able to set
	//             baud rate as well as parity
	USART6->CR1 &= ~USART_CR1_UE;	// Uart Disable
	// set baudrate
	USART6->BRR = brr;
	setParity(parity);
	setOversamplingMode(oversample);
	USART6->CR1 |=  USART_CR1_UE;	// Uart Reenable
}

void
modm::platform::UsartHal6::setOversamplingMode(OversamplingMode mode)
{
	const bool usartEnabled = (USART6->CR1 & USART_CR1_UE);
	if(usartEnabled) {
		disableOperation();
	}
	if(mode == OversamplingMode::By16) {
		USART6->CR1 &= ~static_cast<uint32_t>(OversamplingMode::By8);
	} else {
		USART6->CR1 |=  static_cast<uint32_t>(OversamplingMode::By8);
	}

	if(usartEnabled) {
		enableOperation();
	}
}
void
modm::platform::UsartHal6::setSpiClock(SpiClock clk)
{
	const bool usartEnabled = (USART6->CR1 & USART_CR1_UE);
	if(usartEnabled) {
		disableOperation();
	}
	if(clk == SpiClock::Disabled) {
		USART6->CR2 &= ~static_cast<uint32_t>(SpiClock::Enabled);
	} else {
		USART6->CR2 |=  static_cast<uint32_t>(SpiClock::Enabled);
	}

	if(usartEnabled) {
		enableOperation();
	}
}

void
modm::platform::UsartHal6::setSpiDataMode(SpiDataMode mode)
{
	const bool usartEnabled = (USART6->CR1 & USART_CR1_UE);
	if(usartEnabled) {
		disableOperation();
	}
	USART6->CR2 =
		(USART6->CR2 & ~static_cast<uint32_t>(SpiDataMode::Mode3))
		| static_cast<uint32_t>(mode);

	if(usartEnabled) {
		enableOperation();
	}
}

void
modm::platform::UsartHal6::setLastBitClockPulse(LastBitClockPulse pulse)
{
	const bool usartEnabled = (USART6->CR1 & USART_CR1_UE);
	if(usartEnabled) {
		disableOperation();
	}
	if(pulse == LastBitClockPulse::DoNotOutput) {
		USART6->CR2 &= ~static_cast<uint32_t>(LastBitClockPulse::Output);
	} else {
		USART6->CR2 |=  static_cast<uint32_t>(LastBitClockPulse::Output);
	}

	if(usartEnabled) {
		enableOperation();
	}
}
void
modm::platform::UsartHal6::write(uint8_t data)
{
	USART6->TDR = data;
}

void
modm::platform::UsartHal6::read(uint8_t &data)
{
	data = USART6->RDR;
}

void
modm::platform::UsartHal6::setTransmitterEnable(const bool enable)
{
	if (enable) {
		USART6->CR1 |=  USART_CR1_TE;
	} else {
		USART6->CR1 &= ~USART_CR1_TE;
	}
}

void
modm::platform::UsartHal6::setReceiverEnable(bool enable)
{
	if (enable) {
		USART6->CR1 |=  USART_CR1_RE;
	} else {
		USART6->CR1 &= ~USART_CR1_RE;
	}
}

void
modm::platform::UsartHal6::enableOperation()
{
	USART6->CR1 |= USART_CR1_UE;
}

void
modm::platform::UsartHal6::disableOperation()
{
	USART6->CR1 &= ~USART_CR1_UE;
}

bool
modm::platform::UsartHal6::isReceiveRegisterNotEmpty()
{
	return USART6->ISR & USART_ISR_RXNE;
}

bool
modm::platform::UsartHal6::isTransmitRegisterEmpty()
{
	return USART6->ISR & USART_ISR_TXE;
}

void
modm::platform::UsartHal6::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(USART6_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ(USART6_IRQn);
	}
	else {
		NVIC_DisableIRQ(USART6_IRQn);
	}
}

void
modm::platform::UsartHal6::enableInterrupt(Interrupt_t interrupt)
{
	USART6->CR1 |= interrupt.value;
}

void
modm::platform::UsartHal6::disableInterrupt(Interrupt_t interrupt)
{
	USART6->CR1 &= ~interrupt.value;
}

modm::platform::UsartHal6::InterruptFlag_t
modm::platform::UsartHal6::getInterruptFlags()
{
	return InterruptFlag_t( USART6->ISR );
}

void
modm::platform::UsartHal6::acknowledgeInterruptFlags(InterruptFlag_t flags)
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
	USART6->ICR = (flags.value & mask);
}