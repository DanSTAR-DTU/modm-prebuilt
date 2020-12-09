/*
 * Copyright (c) 2017-2018, Niklas Hauser
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_GPIO_PIN_B4_HPP
#define MODM_STM32_GPIO_PIN_B4_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm::platform
{

/// @cond
class GpioB4;
using GpioOutputB4 = GpioB4;
using GpioInputB4  = GpioB4;
/// @endcond

/// IO class for Pin B4
/// @ingroup	modm_platform_gpio
class GpioB4 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioB4>;
	friend class Adc;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioB4;
	using Input = GpioB4;
	using IO = GpioB4;
	using Type = GpioB4;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::B; ///< Port name
	static constexpr uint8_t pin = 4; ///< Pin number
	static constexpr IRQn_Type ExternalInterruptIRQ = EXTI4_IRQn;

protected:
	/// Bitmask for registers that contain a 1bit value for every pin.
	static constexpr uint16_t mask  = 0x1 << pin;
	/// Bitmask for registers that contain a 2bit value for every pin.
	static constexpr uint32_t mask2 = 0x3 << (pin * 2);
	/// Port Number.
	static constexpr uint8_t port_nr = uint8_t(port);
	/// Alternate Function register id. 0 for pin 0-7. 1 for pin 8-15.
	static constexpr uint8_t af_id  = pin / 8;
	/// Alternate Function offset.
	static constexpr uint8_t af_offset = (pin * 4) % 32;
	/// Alternate Function register mask.
	static constexpr uint32_t af_mask  = 0xf << af_offset;

public:
	/// @cond
	inline static void setAlternateFunction(uint8_t af) {
		GPIOB->AFR[af_id] = (GPIOB->AFR[af_id] & ~af_mask) | ((af & 0xf) << af_offset);
		GPIOB->MODER = (GPIOB->MODER & ~mask2) | (i(Mode::AlternateFunction) << (pin * 2));
	}

	/// Enable Analog Mode which is needed to use this pin as an ADC input.
	inline static void setAnalogInput() { PinSet::setAnalogInput(); }
	/// @endcond

public:
	// GpioOutput
	// start documentation inherited
	inline static void setOutput() { PinSet::setOutput(); }
	inline static void setOutput(bool status) { PinSet::setOutput(status); }
	inline static void set() { PinSet::set(); }
	inline static void set(bool status) { PinSet::set(status); }
	inline static void reset() { PinSet::reset(); }
	inline static bool toggle() {
		if (isSet()) { reset(); return true; }
		else         { set();   return false; }
	}
	inline static bool isSet() { return (GPIOB->ODR & mask); }
	// stop documentation inherited
	inline static void configure(OutputType type, OutputSpeed speed = OutputSpeed::MHz50) { PinSet::configure(type, speed); }
	inline static void setOutput(OutputType type, OutputSpeed speed = OutputSpeed::MHz50) { PinSet::setOutput(type, speed); }
	// GpioInput
	// start documentation inherited
	inline static void setInput() { PinSet::setInput(); }
	inline static bool read() { return (GPIOB->IDR & mask); }
	// end documentation inherited
	inline static void configure(InputType type) { PinSet::configure(type); }
	inline static void setInput(InputType type) { PinSet::setInput(type); }
	// External Interrupts
	// Warning: This will disable any previously enabled interrupt which is
	// routed to the same interupt line, e.g. PA3 will disable PB3.
	// This is a hardware limitation by the STM32 EXTI.
	inline static void enableExternalInterrupt()
	{
		// PA[x], x =  0 ..  3 maps to EXTICR[0]
		// PA[x], x =  4 ..  7 maps to EXTICR[1]
		// PA[x], x =  8 .. 11 maps to EXTICR[2]
		// PA[x], x = 12 .. 15 maps to EXTICR[3]
		// => bit3 and bit2 (mask 0x0c) specify the register
		// => bit1 and bit0 (mask 0x03) specify the bit position
		constexpr uint8_t index   = (pin & 0b1100) >> 2;
		constexpr uint8_t bit_pos = (pin & 0b0011) << 2;
		constexpr uint16_t syscfg_mask = (0b1111) << bit_pos;
		constexpr uint16_t syscfg_value = (port_nr & (0b1111)) << bit_pos;
		SYSCFG->EXTICR[index] = (SYSCFG->EXTICR[index] & ~syscfg_mask) | syscfg_value;
		EXTI->IMR1 |= mask;
	}
	inline static void disableExternalInterrupt() { EXTI->IMR1 &= ~mask; }
	inline static void enableExternalInterruptVector(const uint32_t priority)
	{
		NVIC_SetPriority(ExternalInterruptIRQ, priority);
		NVIC_EnableIRQ(ExternalInterruptIRQ);
	}
	inline static void disableExternalInterruptVector() { NVIC_DisableIRQ(ExternalInterruptIRQ); }
	inline static void setInputTrigger(const InputTrigger trigger)
	{
		switch (trigger)
		{
		case InputTrigger::RisingEdge:
			EXTI->RTSR1 |=  mask;
			EXTI->FTSR1 &= ~mask;
			break;
		case InputTrigger::FallingEdge:
			EXTI->RTSR1 &= ~mask;
			EXTI->FTSR1 |=  mask;
			break;
		case InputTrigger::BothEdges:
			EXTI->RTSR1 |=  mask;
			EXTI->FTSR1 |=  mask;
			break;
		}
	}
	inline static bool getExternalInterruptFlag() { return (EXTI->PR1 & mask); }
	inline static void acknowledgeExternalInterruptFlag() { EXTI->PR1 = mask; }
	// GpioIO
	// start documentation inherited
	inline static Direction getDirection() {
		uint32_t mode = (GPIOB->MODER & mask2);
		if (mode == (i(Mode::Input) << pin * 2)) {
			return Direction::In;
		}
		if (mode == (i(Mode::Output) << pin * 2)) {
			return Direction::Out;
		}
		return Direction::Special;
	}
	// end documentation inherited
	inline static void lock() { PinSet::lock(); }
	inline static void disconnect() {
		setInput(InputType::Floating);
		GPIOB->AFR[af_id] &= ~af_mask;
	}

public:
#ifdef __DOXYGEN__
	/// @{
	/// Connect to any software peripheral
	using BitBang = GpioSignal;
	/// Connect to Tim17
	using Bkin = GpioSignal;
	/// Connect to Ucpd1
	using Cc2 = GpioSignal;
	/// Connect to Tim16 or Tim3
	using Ch1 = GpioSignal;
	/// Connect to Tim8
	using Ch2n = GpioSignal;
	/// Connect to Uart5
	using De = GpioSignal;
	/// Connect to Hrtim1
	using Eev7 = GpioSignal;
	/// Connect to Sys
	using Jtrst = GpioSignal;
	/// Connect to Sai1
	using Mclkb = GpioSignal;
	/// Connect to Spi1 or Spi3
	using Miso = GpioSignal;
	/// Connect to Uart5
	using Rts = GpioSignal;
	/// Connect to Usart2
	using Rx = GpioSignal;
	/// Connect to Fdcan3
	using Tx = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioB4::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct Bkin { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim17),
			"GpioB4::Bkin only connects to Tim17!");
	};
	template< Peripheral peripheral >
	struct Cc2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Ucpd1),
			"GpioB4::Cc2 only connects to Ucpd1!");
	};
	template< Peripheral peripheral >
	struct Ch1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim16) ||
			(peripheral == Peripheral::Tim3),
			"GpioB4::Ch1 only connects to Tim16 or Tim3!");
	};
	template< Peripheral peripheral >
	struct Ch2n { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim8),
			"GpioB4::Ch2n only connects to Tim8!");
	};
	template< Peripheral peripheral >
	struct De { static void connect();
		static_assert(
			(peripheral == Peripheral::Uart5),
			"GpioB4::De only connects to Uart5!");
	};
	template< Peripheral peripheral >
	struct Eev7 { static void connect();
		static_assert(
			(peripheral == Peripheral::Hrtim1),
			"GpioB4::Eev7 only connects to Hrtim1!");
	};
	template< Peripheral peripheral >
	struct Jtrst { static void connect();
		static_assert(
			(peripheral == Peripheral::Sys),
			"GpioB4::Jtrst only connects to Sys!");
	};
	template< Peripheral peripheral >
	struct Mclkb { static void connect();
		static_assert(
			(peripheral == Peripheral::Sai1),
			"GpioB4::Mclkb only connects to Sai1!");
	};
	template< Peripheral peripheral >
	struct Miso { static void connect();
		static_assert(
			(peripheral == Peripheral::Spi1) ||
			(peripheral == Peripheral::Spi3),
			"GpioB4::Miso only connects to Spi1 or Spi3!");
	};
	template< Peripheral peripheral >
	struct Rts { static void connect();
		static_assert(
			(peripheral == Peripheral::Uart5),
			"GpioB4::Rts only connects to Uart5!");
	};
	template< Peripheral peripheral >
	struct Rx { static void connect();
		static_assert(
			(peripheral == Peripheral::Usart2),
			"GpioB4::Rx only connects to Usart2!");
	};
	template< Peripheral peripheral >
	struct Tx { static void connect();
		static_assert(
			(peripheral == Peripheral::Fdcan3),
			"GpioB4::Tx only connects to Fdcan3!");
	};
	/// @endcond
private:
	template< Peripheral peripheral >
	static constexpr int8_t AdcChannel = -1;
	template< Peripheral peripheral >
	static constexpr int8_t DacChannel = -1;
};

/// @cond
template<>
struct GpioB4::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioB4::Bkin<Peripheral::Tim17>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Bkin;
	static constexpr int af = 10;
	inline static void
	connect()
	{
		setAlternateFunction(10);
	}
};
template<>
struct GpioB4::Cc2<Peripheral::Ucpd1>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Cc2;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioB4::Ch1<Peripheral::Tim16>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1;
	static constexpr int af = 1;
	inline static void
	connect()
	{
		setAlternateFunction(1);
	}
};
template<>
struct GpioB4::Ch1<Peripheral::Tim3>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1;
	static constexpr int af = 2;
	inline static void
	connect()
	{
		setAlternateFunction(2);
	}
};
template<>
struct GpioB4::Ch2n<Peripheral::Tim8>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch2n;
	static constexpr int af = 4;
	inline static void
	connect()
	{
		setAlternateFunction(4);
	}
};
template<>
struct GpioB4::De<Peripheral::Uart5>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::De;
	static constexpr int af = 8;
	inline static void
	connect()
	{
		setAlternateFunction(8);
	}
};
template<>
struct GpioB4::Eev7<Peripheral::Hrtim1>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Eev7;
	static constexpr int af = 13;
	inline static void
	connect()
	{
		setAlternateFunction(13);
	}
};
template<>
struct GpioB4::Jtrst<Peripheral::Sys>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Jtrst;
	static constexpr int af = 0;
	inline static void
	connect()
	{
		setAlternateFunction(0);
	}
};
template<>
struct GpioB4::Mclkb<Peripheral::Sai1>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Mclkb;
	static constexpr int af = 14;
	inline static void
	connect()
	{
		setAlternateFunction(14);
	}
};
template<>
struct GpioB4::Miso<Peripheral::Spi1>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Miso;
	static constexpr int af = 5;
	inline static void
	connect()
	{
		setAlternateFunction(5);
	}
};
template<>
struct GpioB4::Miso<Peripheral::Spi3>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Miso;
	static constexpr int af = 6;
	inline static void
	connect()
	{
		setAlternateFunction(6);
	}
};
template<>
struct GpioB4::Rts<Peripheral::Uart5>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Rts;
	static constexpr int af = 8;
	inline static void
	connect()
	{
		setAlternateFunction(8);
	}
};
template<>
struct GpioB4::Rx<Peripheral::Usart2>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Rx;
	static constexpr int af = 7;
	inline static void
	connect()
	{
		setAlternateFunction(7);
	}
};
template<>
struct GpioB4::Tx<Peripheral::Fdcan3>
{
	using Gpio = GpioB4;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Tx;
	static constexpr int af = 11;
	inline static void
	connect()
	{
		setAlternateFunction(11);
	}
};
/// @endcond

} // namespace modm::platform

#endif // MODM_STM32_GPIO_PIN_B4_HPP