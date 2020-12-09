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

#ifndef MODM_STM32_GPIO_PIN_B6_HPP
#define MODM_STM32_GPIO_PIN_B6_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm::platform
{

/// @cond
class GpioB6;
using GpioOutputB6 = GpioB6;
using GpioInputB6  = GpioB6;
/// @endcond

/// IO class for Pin B6
/// @ingroup	modm_platform_gpio
class GpioB6 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioB6>;
	friend class Adc;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioB6;
	using Input = GpioB6;
	using IO = GpioB6;
	using Type = GpioB6;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::B; ///< Port name
	static constexpr uint8_t pin = 6; ///< Pin number
	static constexpr IRQn_Type ExternalInterruptIRQ = EXTI9_5_IRQn;

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
	/// Connect to Tim8
	using Bkin2 = GpioSignal;
	/// Connect to Ucpd1
	using Cc1 = GpioSignal;
	/// Connect to Tim4 or Tim8
	using Ch1 = GpioSignal;
	/// Connect to Tim16
	using Ch1n = GpioSignal;
	/// Connect to Hrtim1
	using Eev4 = GpioSignal;
	/// Connect to Tim8 or Lptim1
	using Etr = GpioSignal;
	/// Connect to Sai1
	using Fsb = GpioSignal;
	/// Connect to Comp4
	using Out = GpioSignal;
	/// Connect to Hrtim1
	using Scin = GpioSignal;
	/// Connect to Usart1 or Fdcan2
	using Tx = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioB6::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct Bkin2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim8),
			"GpioB6::Bkin2 only connects to Tim8!");
	};
	template< Peripheral peripheral >
	struct Cc1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Ucpd1),
			"GpioB6::Cc1 only connects to Ucpd1!");
	};
	template< Peripheral peripheral >
	struct Ch1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim4) ||
			(peripheral == Peripheral::Tim8),
			"GpioB6::Ch1 only connects to Tim4 or Tim8!");
	};
	template< Peripheral peripheral >
	struct Ch1n { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim16),
			"GpioB6::Ch1n only connects to Tim16!");
	};
	template< Peripheral peripheral >
	struct Eev4 { static void connect();
		static_assert(
			(peripheral == Peripheral::Hrtim1),
			"GpioB6::Eev4 only connects to Hrtim1!");
	};
	template< Peripheral peripheral >
	struct Etr { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim8) ||
			(peripheral == Peripheral::Lptim1),
			"GpioB6::Etr only connects to Tim8 or Lptim1!");
	};
	template< Peripheral peripheral >
	struct Fsb { static void connect();
		static_assert(
			(peripheral == Peripheral::Sai1),
			"GpioB6::Fsb only connects to Sai1!");
	};
	template< Peripheral peripheral >
	struct Out { static void connect();
		static_assert(
			(peripheral == Peripheral::Comp4),
			"GpioB6::Out only connects to Comp4!");
	};
	template< Peripheral peripheral >
	struct Scin { static void connect();
		static_assert(
			(peripheral == Peripheral::Hrtim1),
			"GpioB6::Scin only connects to Hrtim1!");
	};
	template< Peripheral peripheral >
	struct Tx { static void connect();
		static_assert(
			(peripheral == Peripheral::Usart1) ||
			(peripheral == Peripheral::Fdcan2),
			"GpioB6::Tx only connects to Usart1 or Fdcan2!");
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
struct GpioB6::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioB6::Bkin2<Peripheral::Tim8>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Bkin2;
	static constexpr int af = 10;
	inline static void
	connect()
	{
		setAlternateFunction(10);
	}
};
template<>
struct GpioB6::Cc1<Peripheral::Ucpd1>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Cc1;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioB6::Ch1<Peripheral::Tim4>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1;
	static constexpr int af = 2;
	inline static void
	connect()
	{
		setAlternateFunction(2);
	}
};
template<>
struct GpioB6::Ch1<Peripheral::Tim8>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1;
	static constexpr int af = 5;
	inline static void
	connect()
	{
		setAlternateFunction(5);
	}
};
template<>
struct GpioB6::Ch1n<Peripheral::Tim16>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1n;
	static constexpr int af = 1;
	inline static void
	connect()
	{
		setAlternateFunction(1);
	}
};
template<>
struct GpioB6::Eev4<Peripheral::Hrtim1>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Eev4;
	static constexpr int af = 13;
	inline static void
	connect()
	{
		setAlternateFunction(13);
	}
};
template<>
struct GpioB6::Etr<Peripheral::Tim8>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Etr;
	static constexpr int af = 6;
	inline static void
	connect()
	{
		setAlternateFunction(6);
	}
};
template<>
struct GpioB6::Etr<Peripheral::Lptim1>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Etr;
	static constexpr int af = 11;
	inline static void
	connect()
	{
		setAlternateFunction(11);
	}
};
template<>
struct GpioB6::Fsb<Peripheral::Sai1>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Fsb;
	static constexpr int af = 14;
	inline static void
	connect()
	{
		setAlternateFunction(14);
	}
};
template<>
struct GpioB6::Out<Peripheral::Comp4>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Out;
	static constexpr int af = 8;
	inline static void
	connect()
	{
		setAlternateFunction(8);
	}
};
template<>
struct GpioB6::Scin<Peripheral::Hrtim1>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Scin;
	static constexpr int af = 12;
	inline static void
	connect()
	{
		setAlternateFunction(12);
	}
};
template<>
struct GpioB6::Tx<Peripheral::Usart1>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Tx;
	static constexpr int af = 7;
	inline static void
	connect()
	{
		setAlternateFunction(7);
	}
};
template<>
struct GpioB6::Tx<Peripheral::Fdcan2>
{
	using Gpio = GpioB6;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Tx;
	static constexpr int af = 9;
	inline static void
	connect()
	{
		setAlternateFunction(9);
	}
};
/// @endcond

} // namespace modm::platform

#endif // MODM_STM32_GPIO_PIN_B6_HPP