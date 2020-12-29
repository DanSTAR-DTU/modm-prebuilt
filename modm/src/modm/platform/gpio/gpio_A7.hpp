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

#ifndef MODM_STM32_GPIO_PIN_A7_HPP
#define MODM_STM32_GPIO_PIN_A7_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm::platform
{

/// @cond
class GpioA7;
using GpioOutputA7 = GpioA7;
using GpioInputA7  = GpioA7;
/// @endcond

/// IO class for Pin A7
/// @ingroup	modm_platform_gpio
class GpioA7 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioA7>;
	friend class Adc;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioA7;
	using Input = GpioA7;
	using IO = GpioA7;
	using Type = GpioA7;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::A; ///< Port name
	static constexpr uint8_t pin = 7; ///< Pin number
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
		GPIOA->AFR[af_id] = (GPIOA->AFR[af_id] & ~af_mask) | ((af & 0xf) << af_offset);
		GPIOA->MODER = (GPIOA->MODER & ~mask2) | (i(Mode::AlternateFunction) << (pin * 2));
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
	inline static bool isSet() { return (GPIOA->ODR & mask); }
	// stop documentation inherited
	inline static void configure(OutputType type, OutputSpeed speed = OutputSpeed::MHz50) { PinSet::configure(type, speed); }
	inline static void setOutput(OutputType type, OutputSpeed speed = OutputSpeed::MHz50) { PinSet::setOutput(type, speed); }
	// GpioInput
	// start documentation inherited
	inline static void setInput() { PinSet::setInput(); }
	inline static bool read() { return (GPIOA->IDR & mask); }
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
		uint32_t mode = (GPIOA->MODER & mask2);
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
		GPIOA->AFR[af_id] &= ~af_mask;
	}

public:
#ifdef __DOXYGEN__
	/// @{
	/// Connect to any software peripheral
	using BitBang = GpioSignal;
	/// Connect to Quadspi1
	using Bk1io2 = GpioSignal;
	/// Connect to Tim17
	using Ch1 = GpioSignal;
	/// Connect to Tim8 or Tim1
	using Ch1n = GpioSignal;
	/// Connect to Tim3
	using Ch2 = GpioSignal;
	/// Connect to Ucpd1
	using Frstx1 = GpioSignal;
	/// Connect to Ucpd1
	using Frstx2 = GpioSignal;
	/// Connect to Adc2
	using In4 = GpioSignal;
	/// Connect to Comp2
	using Inp = GpioSignal;
	/// Connect to Spi1
	using Mosi = GpioSignal;
	/// Connect to Comp2
	using Out = GpioSignal;
	/// Connect to Opamp1 or Opamp2
	using Vinp = GpioSignal;
	/// Connect to Opamp1 or Opamp2
	using Vinpsec = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioA7::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct Bk1io2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Quadspi1),
			"GpioA7::Bk1io2 only connects to Quadspi1!");
	};
	template< Peripheral peripheral >
	struct Ch1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim17),
			"GpioA7::Ch1 only connects to Tim17!");
	};
	template< Peripheral peripheral >
	struct Ch1n { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim8) ||
			(peripheral == Peripheral::Tim1),
			"GpioA7::Ch1n only connects to Tim8 or Tim1!");
	};
	template< Peripheral peripheral >
	struct Ch2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim3),
			"GpioA7::Ch2 only connects to Tim3!");
	};
	template< Peripheral peripheral >
	struct Frstx1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Ucpd1),
			"GpioA7::Frstx1 only connects to Ucpd1!");
	};
	template< Peripheral peripheral >
	struct Frstx2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Ucpd1),
			"GpioA7::Frstx2 only connects to Ucpd1!");
	};
	template< Peripheral peripheral >
	struct In4 { static void connect();
		static_assert(
			(peripheral == Peripheral::Adc2),
			"GpioA7::In4 only connects to Adc2!");
	};
	template< Peripheral peripheral >
	struct Inp { static void connect();
		static_assert(
			(peripheral == Peripheral::Comp2),
			"GpioA7::Inp only connects to Comp2!");
	};
	template< Peripheral peripheral >
	struct Mosi { static void connect();
		static_assert(
			(peripheral == Peripheral::Spi1),
			"GpioA7::Mosi only connects to Spi1!");
	};
	template< Peripheral peripheral >
	struct Out { static void connect();
		static_assert(
			(peripheral == Peripheral::Comp2),
			"GpioA7::Out only connects to Comp2!");
	};
	template< Peripheral peripheral >
	struct Vinp { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1) ||
			(peripheral == Peripheral::Opamp2),
			"GpioA7::Vinp only connects to Opamp1 or Opamp2!");
	};
	template< Peripheral peripheral >
	struct Vinpsec { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1) ||
			(peripheral == Peripheral::Opamp2),
			"GpioA7::Vinpsec only connects to Opamp1 or Opamp2!");
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
struct GpioA7::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioA7::Bk1io2<Peripheral::Quadspi1>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Bk1io2;
	static constexpr int af = 10;
	inline static void
	connect()
	{
		setAlternateFunction(10);
	}
};
template<>
struct GpioA7::Ch1<Peripheral::Tim17>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1;
	static constexpr int af = 1;
	inline static void
	connect()
	{
		setAlternateFunction(1);
	}
};
template<>
struct GpioA7::Ch1n<Peripheral::Tim8>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1n;
	static constexpr int af = 4;
	inline static void
	connect()
	{
		setAlternateFunction(4);
	}
};
template<>
struct GpioA7::Ch1n<Peripheral::Tim1>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1n;
	static constexpr int af = 6;
	inline static void
	connect()
	{
		setAlternateFunction(6);
	}
};
template<>
struct GpioA7::Ch2<Peripheral::Tim3>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch2;
	static constexpr int af = 2;
	inline static void
	connect()
	{
		setAlternateFunction(2);
	}
};
template<>
struct GpioA7::Frstx1<Peripheral::Ucpd1>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Frstx1;
	static constexpr int af = 14;
	inline static void
	connect()
	{
		setAlternateFunction(14);
	}
};
template<>
struct GpioA7::Frstx2<Peripheral::Ucpd1>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Frstx2;
	static constexpr int af = 14;
	inline static void
	connect()
	{
		setAlternateFunction(14);
	}
};
template<>
struct GpioA7::In4<Peripheral::Adc2>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::In4;
	static constexpr int af = -1;
	inline static void
	connect()
	{
		disconnect();
		setAnalogInput();
	}
};
template<>
constexpr int8_t
GpioA7::AdcChannel<Peripheral::Adc2> = 4;
template<>
struct GpioA7::Inp<Peripheral::Comp2>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Inp;
	static constexpr int af = -1;
	inline static void
	connect()
	{
		disconnect();
		setAnalogInput();
	}
};
template<>
struct GpioA7::Mosi<Peripheral::Spi1>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Mosi;
	static constexpr int af = 5;
	inline static void
	connect()
	{
		setAlternateFunction(5);
	}
};
template<>
struct GpioA7::Out<Peripheral::Comp2>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Out;
	static constexpr int af = 8;
	inline static void
	connect()
	{
		setAlternateFunction(8);
	}
};
template<>
struct GpioA7::Vinp<Peripheral::Opamp1>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinp;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA7::Vinp<Peripheral::Opamp2>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinp;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA7::Vinpsec<Peripheral::Opamp1>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinpsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA7::Vinpsec<Peripheral::Opamp2>
{
	using Gpio = GpioA7;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinpsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
/// @endcond

} // namespace modm::platform

#endif // MODM_STM32_GPIO_PIN_A7_HPP