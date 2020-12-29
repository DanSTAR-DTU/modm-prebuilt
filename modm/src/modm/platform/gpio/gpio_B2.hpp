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

#ifndef MODM_STM32_GPIO_PIN_B2_HPP
#define MODM_STM32_GPIO_PIN_B2_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm::platform
{

/// @cond
class GpioB2;
using GpioOutputB2 = GpioB2;
using GpioInputB2  = GpioB2;
/// @endcond

/// IO class for Pin B2
/// @ingroup	modm_platform_gpio
class GpioB2 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioB2>;
	friend class Adc;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioB2;
	using Input = GpioB2;
	using IO = GpioB2;
	using Type = GpioB2;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::B; ///< Port name
	static constexpr uint8_t pin = 2; ///< Pin number
	static constexpr IRQn_Type ExternalInterruptIRQ = EXTI2_IRQn;

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
	using Bk2io1 = GpioSignal;
	/// Connect to Tim5 or Tim20
	using Ch1 = GpioSignal;
	/// Connect to Adc2
	using In12 = GpioSignal;
	/// Connect to Comp4
	using Inm = GpioSignal;
	/// Connect to Lptim1
	using Out = GpioSignal;
	/// Connect to Rtc
	using Out2 = GpioSignal;
	/// Connect to Hrtim1
	using Scin = GpioSignal;
	/// Connect to I2c3
	using Smba = GpioSignal;
	/// Connect to Opamp3
	using Vinm = GpioSignal;
	/// Connect to Opamp3
	using Vinm0 = GpioSignal;
	/// Connect to Opamp3
	using Vinmsec = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioB2::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct Bk2io1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Quadspi1),
			"GpioB2::Bk2io1 only connects to Quadspi1!");
	};
	template< Peripheral peripheral >
	struct Ch1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim5) ||
			(peripheral == Peripheral::Tim20),
			"GpioB2::Ch1 only connects to Tim5 or Tim20!");
	};
	template< Peripheral peripheral >
	struct In12 { static void connect();
		static_assert(
			(peripheral == Peripheral::Adc2),
			"GpioB2::In12 only connects to Adc2!");
	};
	template< Peripheral peripheral >
	struct Inm { static void connect();
		static_assert(
			(peripheral == Peripheral::Comp4),
			"GpioB2::Inm only connects to Comp4!");
	};
	template< Peripheral peripheral >
	struct Out { static void connect();
		static_assert(
			(peripheral == Peripheral::Lptim1),
			"GpioB2::Out only connects to Lptim1!");
	};
	template< Peripheral peripheral >
	struct Out2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Rtc),
			"GpioB2::Out2 only connects to Rtc!");
	};
	template< Peripheral peripheral >
	struct Scin { static void connect();
		static_assert(
			(peripheral == Peripheral::Hrtim1),
			"GpioB2::Scin only connects to Hrtim1!");
	};
	template< Peripheral peripheral >
	struct Smba { static void connect();
		static_assert(
			(peripheral == Peripheral::I2c3),
			"GpioB2::Smba only connects to I2c3!");
	};
	template< Peripheral peripheral >
	struct Vinm { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp3),
			"GpioB2::Vinm only connects to Opamp3!");
	};
	template< Peripheral peripheral >
	struct Vinm0 { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp3),
			"GpioB2::Vinm0 only connects to Opamp3!");
	};
	template< Peripheral peripheral >
	struct Vinmsec { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp3),
			"GpioB2::Vinmsec only connects to Opamp3!");
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
struct GpioB2::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioB2::Bk2io1<Peripheral::Quadspi1>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Bk2io1;
	static constexpr int af = 10;
	inline static void
	connect()
	{
		setAlternateFunction(10);
	}
};
template<>
struct GpioB2::Ch1<Peripheral::Tim5>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1;
	static constexpr int af = 2;
	inline static void
	connect()
	{
		setAlternateFunction(2);
	}
};
template<>
struct GpioB2::Ch1<Peripheral::Tim20>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1;
	static constexpr int af = 3;
	inline static void
	connect()
	{
		setAlternateFunction(3);
	}
};
template<>
struct GpioB2::In12<Peripheral::Adc2>
{
	using Gpio = GpioB2;
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
GpioB2::AdcChannel<Peripheral::Adc2> = 12;
template<>
struct GpioB2::Inm<Peripheral::Comp4>
{
	using Gpio = GpioB2;
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
struct GpioB2::Out<Peripheral::Lptim1>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Out;
	static constexpr int af = 1;
	inline static void
	connect()
	{
		setAlternateFunction(1);
	}
};
template<>
struct GpioB2::Out2<Peripheral::Rtc>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Out2;
	static constexpr int af = 0;
	inline static void
	connect()
	{
		setAlternateFunction(0);
	}
};
template<>
struct GpioB2::Scin<Peripheral::Hrtim1>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Scin;
	static constexpr int af = 13;
	inline static void
	connect()
	{
		setAlternateFunction(13);
	}
};
template<>
struct GpioB2::Smba<Peripheral::I2c3>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Smba;
	static constexpr int af = 4;
	inline static void
	connect()
	{
		setAlternateFunction(4);
	}
};
template<>
struct GpioB2::Vinm<Peripheral::Opamp3>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioB2::Vinm0<Peripheral::Opamp3>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm0;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioB2::Vinmsec<Peripheral::Opamp3>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinmsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
/// @endcond

} // namespace modm::platform

#endif // MODM_STM32_GPIO_PIN_B2_HPP