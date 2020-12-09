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

#ifndef MODM_STM32_GPIO_PIN_C5_HPP
#define MODM_STM32_GPIO_PIN_C5_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm::platform
{

/// @cond
class GpioC5;
using GpioOutputC5 = GpioC5;
using GpioInputC5  = GpioC5;
/// @endcond

/// IO class for Pin C5
/// @ingroup	modm_platform_gpio
class GpioC5 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioC5>;
	friend class Adc;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioC5;
	using Input = GpioC5;
	using IO = GpioC5;
	using Type = GpioC5;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::C; ///< Port name
	static constexpr uint8_t pin = 5; ///< Pin number
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
		GPIOC->AFR[af_id] = (GPIOC->AFR[af_id] & ~af_mask) | ((af & 0xf) << af_offset);
		GPIOC->MODER = (GPIOC->MODER & ~mask2) | (i(Mode::AlternateFunction) << (pin * 2));
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
	inline static bool isSet() { return (GPIOC->ODR & mask); }
	// stop documentation inherited
	inline static void configure(OutputType type, OutputSpeed speed = OutputSpeed::MHz50) { PinSet::configure(type, speed); }
	inline static void setOutput(OutputType type, OutputSpeed speed = OutputSpeed::MHz50) { PinSet::setOutput(type, speed); }
	// GpioInput
	// start documentation inherited
	inline static void setInput() { PinSet::setInput(); }
	inline static bool read() { return (GPIOC->IDR & mask); }
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
		uint32_t mode = (GPIOC->MODER & mask2);
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
		GPIOC->AFR[af_id] &= ~af_mask;
	}

public:
#ifdef __DOXYGEN__
	/// @{
	/// Connect to any software peripheral
	using BitBang = GpioSignal;
	/// Connect to Tim15
	using Bkin = GpioSignal;
	/// Connect to Tim1
	using Ch4n = GpioSignal;
	/// Connect to Sai1
	using D3 = GpioSignal;
	/// Connect to Hrtim1
	using Eev10 = GpioSignal;
	/// Connect to Adc2
	using In11 = GpioSignal;
	/// Connect to Usart1
	using Rx = GpioSignal;
	/// Connect to Opamp1 or Opamp2
	using Vinm = GpioSignal;
	/// Connect to Opamp1 or Opamp2
	using Vinm1 = GpioSignal;
	/// Connect to Opamp1 or Opamp2
	using Vinmsec = GpioSignal;
	/// Connect to Sys
	using Wkup5 = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioC5::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct Bkin { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim15),
			"GpioC5::Bkin only connects to Tim15!");
	};
	template< Peripheral peripheral >
	struct Ch4n { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim1),
			"GpioC5::Ch4n only connects to Tim1!");
	};
	template< Peripheral peripheral >
	struct D3 { static void connect();
		static_assert(
			(peripheral == Peripheral::Sai1),
			"GpioC5::D3 only connects to Sai1!");
	};
	template< Peripheral peripheral >
	struct Eev10 { static void connect();
		static_assert(
			(peripheral == Peripheral::Hrtim1),
			"GpioC5::Eev10 only connects to Hrtim1!");
	};
	template< Peripheral peripheral >
	struct In11 { static void connect();
		static_assert(
			(peripheral == Peripheral::Adc2),
			"GpioC5::In11 only connects to Adc2!");
	};
	template< Peripheral peripheral >
	struct Rx { static void connect();
		static_assert(
			(peripheral == Peripheral::Usart1),
			"GpioC5::Rx only connects to Usart1!");
	};
	template< Peripheral peripheral >
	struct Vinm { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1) ||
			(peripheral == Peripheral::Opamp2),
			"GpioC5::Vinm only connects to Opamp1 or Opamp2!");
	};
	template< Peripheral peripheral >
	struct Vinm1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1) ||
			(peripheral == Peripheral::Opamp2),
			"GpioC5::Vinm1 only connects to Opamp1 or Opamp2!");
	};
	template< Peripheral peripheral >
	struct Vinmsec { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1) ||
			(peripheral == Peripheral::Opamp2),
			"GpioC5::Vinmsec only connects to Opamp1 or Opamp2!");
	};
	template< Peripheral peripheral >
	struct Wkup5 { static void connect();
		static_assert(
			(peripheral == Peripheral::Sys),
			"GpioC5::Wkup5 only connects to Sys!");
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
struct GpioC5::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioC5::Bkin<Peripheral::Tim15>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Bkin;
	static constexpr int af = 2;
	inline static void
	connect()
	{
		setAlternateFunction(2);
	}
};
template<>
struct GpioC5::Ch4n<Peripheral::Tim1>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch4n;
	static constexpr int af = 6;
	inline static void
	connect()
	{
		setAlternateFunction(6);
	}
};
template<>
struct GpioC5::D3<Peripheral::Sai1>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::D3;
	static constexpr int af = 3;
	inline static void
	connect()
	{
		setAlternateFunction(3);
	}
};
template<>
struct GpioC5::Eev10<Peripheral::Hrtim1>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Eev10;
	static constexpr int af = 13;
	inline static void
	connect()
	{
		setAlternateFunction(13);
	}
};
template<>
struct GpioC5::In11<Peripheral::Adc2>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::In11;
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
GpioC5::AdcChannel<Peripheral::Adc2> = 11;
template<>
struct GpioC5::Rx<Peripheral::Usart1>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Rx;
	static constexpr int af = 7;
	inline static void
	connect()
	{
		setAlternateFunction(7);
	}
};
template<>
struct GpioC5::Vinm<Peripheral::Opamp1>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioC5::Vinm<Peripheral::Opamp2>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioC5::Vinm1<Peripheral::Opamp1>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm1;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioC5::Vinm1<Peripheral::Opamp2>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm1;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioC5::Vinmsec<Peripheral::Opamp1>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinmsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioC5::Vinmsec<Peripheral::Opamp2>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinmsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioC5::Wkup5<Peripheral::Sys>
{
	using Gpio = GpioC5;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Wkup5;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
/// @endcond

} // namespace modm::platform

#endif // MODM_STM32_GPIO_PIN_C5_HPP