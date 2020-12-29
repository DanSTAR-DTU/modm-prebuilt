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

#ifndef MODM_STM32_GPIO_PIN_B15_HPP
#define MODM_STM32_GPIO_PIN_B15_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm::platform
{

/// @cond
class GpioB15;
using GpioOutputB15 = GpioB15;
using GpioInputB15  = GpioB15;
/// @endcond

/// IO class for Pin B15
/// @ingroup	modm_platform_gpio
class GpioB15 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioB15>;
	friend class Adc;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioB15;
	using Input = GpioB15;
	using IO = GpioB15;
	using Type = GpioB15;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::B; ///< Port name
	static constexpr uint8_t pin = 15; ///< Pin number
	static constexpr IRQn_Type ExternalInterruptIRQ = EXTI15_10_IRQn;

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
	/// Connect to Tim15
	using Ch1n = GpioSignal;
	/// Connect to Tim15
	using Ch2 = GpioSignal;
	/// Connect to Tim1
	using Ch3n = GpioSignal;
	/// Connect to Hrtim1
	using Chd2 = GpioSignal;
	/// Connect to Adc2
	using In15 = GpioSignal;
	/// Connect to Adc4
	using In5 = GpioSignal;
	/// Connect to Comp6
	using Inm = GpioSignal;
	/// Connect to Spi2
	using Mosi = GpioSignal;
	/// Connect to Comp3
	using Out = GpioSignal;
	/// Connect to Rtc
	using Refin = GpioSignal;
	/// Connect to I2s2
	using Sd = GpioSignal;
	/// Connect to Opamp5
	using Vinm = GpioSignal;
	/// Connect to Opamp5
	using Vinm0 = GpioSignal;
	/// Connect to Opamp5
	using Vinmsec = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioB15::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct Ch1n { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim15),
			"GpioB15::Ch1n only connects to Tim15!");
	};
	template< Peripheral peripheral >
	struct Ch2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim15),
			"GpioB15::Ch2 only connects to Tim15!");
	};
	template< Peripheral peripheral >
	struct Ch3n { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim1),
			"GpioB15::Ch3n only connects to Tim1!");
	};
	template< Peripheral peripheral >
	struct Chd2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Hrtim1),
			"GpioB15::Chd2 only connects to Hrtim1!");
	};
	template< Peripheral peripheral >
	struct In15 { static void connect();
		static_assert(
			(peripheral == Peripheral::Adc2),
			"GpioB15::In15 only connects to Adc2!");
	};
	template< Peripheral peripheral >
	struct In5 { static void connect();
		static_assert(
			(peripheral == Peripheral::Adc4),
			"GpioB15::In5 only connects to Adc4!");
	};
	template< Peripheral peripheral >
	struct Inm { static void connect();
		static_assert(
			(peripheral == Peripheral::Comp6),
			"GpioB15::Inm only connects to Comp6!");
	};
	template< Peripheral peripheral >
	struct Mosi { static void connect();
		static_assert(
			(peripheral == Peripheral::Spi2),
			"GpioB15::Mosi only connects to Spi2!");
	};
	template< Peripheral peripheral >
	struct Out { static void connect();
		static_assert(
			(peripheral == Peripheral::Comp3),
			"GpioB15::Out only connects to Comp3!");
	};
	template< Peripheral peripheral >
	struct Refin { static void connect();
		static_assert(
			(peripheral == Peripheral::Rtc),
			"GpioB15::Refin only connects to Rtc!");
	};
	template< Peripheral peripheral >
	struct Sd { static void connect();
		static_assert(
			(peripheral == Peripheral::I2s2),
			"GpioB15::Sd only connects to I2s2!");
	};
	template< Peripheral peripheral >
	struct Vinm { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp5),
			"GpioB15::Vinm only connects to Opamp5!");
	};
	template< Peripheral peripheral >
	struct Vinm0 { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp5),
			"GpioB15::Vinm0 only connects to Opamp5!");
	};
	template< Peripheral peripheral >
	struct Vinmsec { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp5),
			"GpioB15::Vinmsec only connects to Opamp5!");
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
struct GpioB15::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioB15::Ch1n<Peripheral::Tim15>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1n;
	static constexpr int af = 2;
	inline static void
	connect()
	{
		setAlternateFunction(2);
	}
};
template<>
struct GpioB15::Ch2<Peripheral::Tim15>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch2;
	static constexpr int af = 1;
	inline static void
	connect()
	{
		setAlternateFunction(1);
	}
};
template<>
struct GpioB15::Ch3n<Peripheral::Tim1>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch3n;
	static constexpr int af = 4;
	inline static void
	connect()
	{
		setAlternateFunction(4);
	}
};
template<>
struct GpioB15::Chd2<Peripheral::Hrtim1>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Chd2;
	static constexpr int af = 13;
	inline static void
	connect()
	{
		setAlternateFunction(13);
	}
};
template<>
struct GpioB15::In15<Peripheral::Adc2>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::In15;
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
GpioB15::AdcChannel<Peripheral::Adc2> = 15;
template<>
struct GpioB15::In5<Peripheral::Adc4>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::In5;
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
GpioB15::AdcChannel<Peripheral::Adc4> = 5;
template<>
struct GpioB15::Inm<Peripheral::Comp6>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Inm;
	static constexpr int af = -1;
	inline static void
	connect()
	{
		disconnect();
		setAnalogInput();
	}
};
template<>
struct GpioB15::Mosi<Peripheral::Spi2>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Mosi;
	static constexpr int af = 5;
	inline static void
	connect()
	{
		setAlternateFunction(5);
	}
};
template<>
struct GpioB15::Out<Peripheral::Comp3>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Out;
	static constexpr int af = 3;
	inline static void
	connect()
	{
		setAlternateFunction(3);
	}
};
template<>
struct GpioB15::Refin<Peripheral::Rtc>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Refin;
	static constexpr int af = 0;
	inline static void
	connect()
	{
		setAlternateFunction(0);
	}
};
template<>
struct GpioB15::Sd<Peripheral::I2s2>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Sd;
	static constexpr int af = 5;
	inline static void
	connect()
	{
		setAlternateFunction(5);
	}
};
template<>
struct GpioB15::Vinm<Peripheral::Opamp5>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioB15::Vinm0<Peripheral::Opamp5>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm0;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioB15::Vinmsec<Peripheral::Opamp5>
{
	using Gpio = GpioB15;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinmsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
/// @endcond

} // namespace modm::platform

#endif // MODM_STM32_GPIO_PIN_B15_HPP