/*
 * Copyright (c) 2013, Kevin Läufer
 * Copyright (c) 2013-2017, Niklas Hauser
 * Copyright (c) 2014, Daniel Krebs
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_SPI_HAL6_HPP
#	error 	"Don't include this file directly, use 'spi_hal6.hpp' instead!"
#endif
#include <modm/platform/clock/rcc.hpp>

void inline
modm::platform::SpiHal6::enable()
{
	Rcc::enable<Peripheral::Spi6>();
	SPI6->CR1 |= SPI_CR1_SPE;		// SPI Enable
}

void inline
modm::platform::SpiHal6::disable()
{
	SPI6->CR1 = 0;
	Rcc::disable<Peripheral::Spi6>();
}

void inline
modm::platform::SpiHal6::initialize(Prescaler prescaler,
		MasterSelection masterSelection, DataMode dataMode,
		DataOrder dataOrder, DataSize dataSize)
{
	enable();
	// disable peripheral
	SPI6->CR1 &= ~SPI_CR1_SPE;
	// set parameters
	SPI6->CR1 = 	  static_cast<uint32_t>(dataMode)
						| static_cast<uint32_t>(dataOrder)
						| static_cast<uint32_t>(masterSelection)
						| static_cast<uint32_t>(prescaler)
						;
	SPI6->CR2 = static_cast<uint32_t>(dataSize);

	if(static_cast<uint8_t>(dataSize) <= static_cast<uint8_t>(DataSize::Bit8))
	{
		SPI6->CR2 |= SPI_CR2_FRXTH;
	}

	if(masterSelection == MasterSelection::Master) {
		SPI6->CR2 |=  SPI_CR2_SSOE; // for master mode
	}
	// reenable peripheral
	SPI6->CR1 |= SPI_CR1_SPE;
}

void inline
modm::platform::SpiHal6::setDataMode(DataMode dataMode)
{
	SPI6->CR1 = (SPI6->CR1 & ~static_cast<uint32_t>(DataMode::All))
										 | static_cast<uint32_t>(dataMode);
}

void inline
modm::platform::SpiHal6::setDataOrder(DataOrder dataOrder)
{
	SPI6->CR1 = (SPI6->CR1 & ~static_cast<uint32_t>(DataOrder::All))
										 | static_cast<uint32_t>(dataOrder);
}

void inline
modm::platform::SpiHal6::setDataSize(DataSize dataSize)
{
	// TODO: implement as set/reset bit
	SPI6->CR1 = (SPI6->CR1 & ~static_cast<uint32_t>(DataSize::All))
										 | static_cast<uint32_t>(dataSize);
}

void inline
modm::platform::SpiHal6::setMasterSelection(MasterSelection masterSelection)
{
	// TODO: implement as set/reset bit
	SPI6->CR1 = (SPI6->CR1 & ~static_cast<uint32_t>(MasterSelection::All))
										 | static_cast<uint32_t>(masterSelection);
}

inline bool
modm::platform::SpiHal6::isReceiveRegisterNotEmpty()
{
	return static_cast<bool>(getInterruptFlags() & InterruptFlag::RxBufferNotEmpty);
}

inline bool
modm::platform::SpiHal6::isTransmitRegisterEmpty()
{
	return static_cast<bool>(getInterruptFlags() & InterruptFlag::TxBufferEmpty);
}

void inline
modm::platform::SpiHal6::write(uint16_t data)
{
	SPI6->DR = data;
}

void inline
modm::platform::SpiHal6::write(uint8_t data)
{
	*((__IO uint8_t *) &SPI6->DR) = data;
}

void inline
modm::platform::SpiHal6::read(uint8_t &data)
{
	data = static_cast<uint8_t>(SPI6->DR);
}

void inline
modm::platform::SpiHal6::read(uint16_t &data)
{
	data = static_cast<uint16_t>(SPI6->DR);
}

void inline
modm::platform::SpiHal6::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(SPI6_IRQn, priority);
		// register IRQ at the NVIC
		NVIC_EnableIRQ(SPI6_IRQn);
	}
	else {
		NVIC_DisableIRQ(SPI6_IRQn);
	}
}

void inline
modm::platform::SpiHal6::enableInterrupt(Interrupt_t interrupt)
{
	SPI6->CR2 |= interrupt.value;
}

void inline
modm::platform::SpiHal6::disableInterrupt(Interrupt_t interrupt)
{
	SPI6->CR2 &= ~interrupt.value;
}

modm::platform::SpiHal6::InterruptFlag_t inline
modm::platform::SpiHal6::getInterruptFlags()
{
	return InterruptFlag_t(SPI6->SR);
}

void inline
modm::platform::SpiHal6::acknowledgeInterruptFlag(InterruptFlag_t /*flags*/)
{
	// TODO: implement; see STM32F3 reference manual p. 736
	// SPI6->SR = flags.value;
}