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

#ifndef MODM_STM32_GPIO_PIN_B1_HPP
#define MODM_STM32_GPIO_PIN_B1_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm::platform
{

/// @cond
class GpioB1;
using GpioOutputB1 = GpioB1;
using GpioInputB1  = GpioB1;
/// @endcond

/// IO class for Pin B1
/// @ingroup	modm_platform_gpio
class GpioB1 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioB1>;
	friend class Adc;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioB1;
	using Input = GpioB1;
	using IO = GpioB1;
	using Type = GpioB1;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::B; ///< Port name
	static constexpr uint8_t pin = 1; ///< Pin number
	static constexpr IRQn_Type ExternalInterruptIRQ = EXTI1_IRQn;

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
	/// Connect to Quadspi1
	using Bk1io0 = GpioSignal;
	/// Connect to Tim8 or Tim1
	using Ch3n = GpioSignal;
	/// Connect to Tim3
	using Ch4 = GpioSignal;
	/// Connect to Lpuart1
	using De = GpioSignal;
	/// Connect to Adc3
	using In1 = GpioSignal;
	/// Connect to Adc1
	using In12 = GpioSignal;
	/// Connect to Comp1
	using Inp = GpioSignal;
	/// Connect to Comp4
	using Out = GpioSignal;
	/// Connect to Lpuart1
	using Rts = GpioSignal;
	/// Connect to Hrtim1
	using Scout = GpioSignal;
	/// Connect to Opamp6
	using Vinm = GpioSignal;
	/// Connect to Opamp6
	using Vinm1 = GpioSignal;
	/// Connect to Opamp6
	using Vinmsec = GpioSignal;
	/// Connect to Opamp3
	using Vout = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioB1::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct Bk1io0 { static void connect();
		static_assert(
			(peripheral == Peripheral::Quadspi1),
			"GpioB1::Bk1io0 only connects to Quadspi1!");
	};
	template< Peripheral peripheral >
	struct Ch3n { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim8) ||
			(peripheral == Peripheral::Tim1),
			"GpioB1::Ch3n only connects to Tim8 or Tim1!");
	};
	template< Peripheral peripheral >
	struct Ch4 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim3),
			"GpioB1::Ch4 only connects to Tim3!");
	};
	template< Peripheral peripheral >
	struct De { static void connect();
		static_assert(
			(peripheral == Peripheral::Lpuart1),
			"GpioB1::De only connects to Lpuart1!");
	};
	template< Peripheral peripheral >
	struct In1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Adc3),
			"GpioB1::In1 only connects to Adc3!");
	};
	template< Peripheral peripheral >
	struct In12 { static void connect();
		static_assert(
			(peripheral == Peripheral::Adc1),
			"GpioB1::In12 only connects to Adc1!");
	};
	template< Peripheral peripheral >
	struct Inp { static void connect();
		static_assert(
			(peripheral == Peripheral::Comp1),
			"GpioB1::Inp only connects to Comp1!");
	};
	template< Peripheral peripheral >
	struct Out { static void connect();
		static_assert(
			(peripheral == Peripheral::Comp4),
			"GpioB1::Out only connects to Comp4!");
	};
	template< Peripheral peripheral >
	struct Rts { static void connect();
		static_assert(
			(peripheral == Peripheral::Lpuart1),
			"GpioB1::Rts only connects to Lpuart1!");
	};
	template< Peripheral peripheral >
	struct Scout { static void connect();
		static_assert(
			(peripheral == Peripheral::Hrtim1),
			"GpioB1::Scout only connects to Hrtim1!");
	};
	template< Peripheral peripheral >
	struct Vinm { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp6),
			"GpioB1::Vinm only connects to Opamp6!");
	};
	template< Peripheral peripheral >
	struct Vinm1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp6),
			"GpioB1::Vinm1 only connects to Opamp6!");
	};
	template< Peripheral peripheral >
	struct Vinmsec { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp6),
			"GpioB1::Vinmsec only connects to Opamp6!");
	};
	template< Peripheral peripheral >
	struct Vout { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp3),
			"GpioB1::Vout only connects to Opamp3!");
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
struct GpioB1::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioB1::Bk1io0<Peripheral::Quadspi1>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Bk1io0;
	static constexpr int af = 10;
	inline static void
	connect()
	{
		setAlternateFunction(10);
	}
};
template<>
struct GpioB1::Ch3n<Peripheral::Tim8>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch3n;
	static constexpr int af = 4;
	inline static void
	connect()
	{
		setAlternateFunction(4);
	}
};
template<>
struct GpioB1::Ch3n<Peripheral::Tim1>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch3n;
	static constexpr int af = 6;
	inline static void
	connect()
	{
		setAlternateFunction(6);
	}
};
template<>
struct GpioB1::Ch4<Peripheral::Tim3>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch4;
	static constexpr int af = 2;
	inline static void
	connect()
	{
		setAlternateFunction(2);
	}
};
template<>
struct GpioB1::De<Peripheral::Lpuart1>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::De;
	static constexpr int af = 12;
	inline static void
	connect()
	{
		setAlternateFunction(12);
	}
};
template<>
struct GpioB1::In1<Peripheral::Adc3>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::In1;
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
GpioB1::AdcChannel<Peripheral::Adc3> = 1;
template<>
struct GpioB1::In12<Peripheral::Adc1>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::In12;
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
GpioB1::AdcChannel<Peripheral::Adc1> = 12;
template<>
struct GpioB1::Inp<Peripheral::Comp1>
{
	using Gpio = GpioB1;
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
struct GpioB1::Out<Peripheral::Comp4>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Out;
	static constexpr int af = 8;
	inline static void
	connect()
	{
		setAlternateFunction(8);
	}
};
template<>
struct GpioB1::Rts<Peripheral::Lpuart1>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Rts;
	static constexpr int af = 12;
	inline static void
	connect()
	{
		setAlternateFunction(12);
	}
};
template<>
struct GpioB1::Scout<Peripheral::Hrtim1>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Scout;
	static constexpr int af = 13;
	inline static void
	connect()
	{
		setAlternateFunction(13);
	}
};
template<>
struct GpioB1::Vinm<Peripheral::Opamp6>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioB1::Vinm1<Peripheral::Opamp6>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm1;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioB1::Vinmsec<Peripheral::Opamp6>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinmsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioB1::Vout<Peripheral::Opamp3>
{
	using Gpio = GpioB1;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vout;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
/// @endcond

} // namespace modm::platform

#endif // MODM_STM32_GPIO_PIN_B1_HPP