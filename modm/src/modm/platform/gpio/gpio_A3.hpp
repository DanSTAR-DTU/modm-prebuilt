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

#ifndef MODM_STM32_GPIO_PIN_A3_HPP
#define MODM_STM32_GPIO_PIN_A3_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm::platform
{

/// @cond
class GpioA3;
using GpioOutputA3 = GpioA3;
using GpioInputA3  = GpioA3;
/// @endcond

/// IO class for Pin A3
/// @ingroup	modm_platform_gpio
class GpioA3 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioA3>;
	friend class Adc;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioA3;
	using Input = GpioA3;
	using IO = GpioA3;
	using Type = GpioA3;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::A; ///< Port name
	static constexpr uint8_t pin = 3; ///< Pin number
	static constexpr IRQn_Type ExternalInterruptIRQ = EXTI3_IRQn;

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
	/// Connect to Tim15
	using Ch2 = GpioSignal;
	/// Connect to Tim2 or Tim5
	using Ch4 = GpioSignal;
	/// Connect to Sai1
	using Ck1 = GpioSignal;
	/// Connect to Quadspi1
	using Clk = GpioSignal;
	/// Connect to Adc1
	using In4 = GpioSignal;
	/// Connect to Comp2
	using Inp = GpioSignal;
	/// Connect to Sai1
	using Mclka = GpioSignal;
	/// Connect to Usart2 or Lpuart1
	using Rx = GpioSignal;
	/// Connect to Opamp1 or Opamp5
	using Vinm = GpioSignal;
	/// Connect to Opamp1
	using Vinm0 = GpioSignal;
	/// Connect to Opamp5
	using Vinm1 = GpioSignal;
	/// Connect to Opamp1 or Opamp5
	using Vinmsec = GpioSignal;
	/// Connect to Opamp1
	using Vinp = GpioSignal;
	/// Connect to Opamp1
	using Vinpsec = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioA3::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct Ch2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim15),
			"GpioA3::Ch2 only connects to Tim15!");
	};
	template< Peripheral peripheral >
	struct Ch4 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim2) ||
			(peripheral == Peripheral::Tim5),
			"GpioA3::Ch4 only connects to Tim2 or Tim5!");
	};
	template< Peripheral peripheral >
	struct Ck1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Sai1),
			"GpioA3::Ck1 only connects to Sai1!");
	};
	template< Peripheral peripheral >
	struct Clk { static void connect();
		static_assert(
			(peripheral == Peripheral::Quadspi1),
			"GpioA3::Clk only connects to Quadspi1!");
	};
	template< Peripheral peripheral >
	struct In4 { static void connect();
		static_assert(
			(peripheral == Peripheral::Adc1),
			"GpioA3::In4 only connects to Adc1!");
	};
	template< Peripheral peripheral >
	struct Inp { static void connect();
		static_assert(
			(peripheral == Peripheral::Comp2),
			"GpioA3::Inp only connects to Comp2!");
	};
	template< Peripheral peripheral >
	struct Mclka { static void connect();
		static_assert(
			(peripheral == Peripheral::Sai1),
			"GpioA3::Mclka only connects to Sai1!");
	};
	template< Peripheral peripheral >
	struct Rx { static void connect();
		static_assert(
			(peripheral == Peripheral::Usart2) ||
			(peripheral == Peripheral::Lpuart1),
			"GpioA3::Rx only connects to Usart2 or Lpuart1!");
	};
	template< Peripheral peripheral >
	struct Vinm { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1) ||
			(peripheral == Peripheral::Opamp5),
			"GpioA3::Vinm only connects to Opamp1 or Opamp5!");
	};
	template< Peripheral peripheral >
	struct Vinm0 { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1),
			"GpioA3::Vinm0 only connects to Opamp1!");
	};
	template< Peripheral peripheral >
	struct Vinm1 { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp5),
			"GpioA3::Vinm1 only connects to Opamp5!");
	};
	template< Peripheral peripheral >
	struct Vinmsec { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1) ||
			(peripheral == Peripheral::Opamp5),
			"GpioA3::Vinmsec only connects to Opamp1 or Opamp5!");
	};
	template< Peripheral peripheral >
	struct Vinp { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1),
			"GpioA3::Vinp only connects to Opamp1!");
	};
	template< Peripheral peripheral >
	struct Vinpsec { static void connect();
		static_assert(
			(peripheral == Peripheral::Opamp1),
			"GpioA3::Vinpsec only connects to Opamp1!");
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
struct GpioA3::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioA3::Ch2<Peripheral::Tim15>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch2;
	static constexpr int af = 9;
	inline static void
	connect()
	{
		setAlternateFunction(9);
	}
};
template<>
struct GpioA3::Ch4<Peripheral::Tim2>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch4;
	static constexpr int af = 1;
	inline static void
	connect()
	{
		setAlternateFunction(1);
	}
};
template<>
struct GpioA3::Ch4<Peripheral::Tim5>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch4;
	static constexpr int af = 2;
	inline static void
	connect()
	{
		setAlternateFunction(2);
	}
};
template<>
struct GpioA3::Ck1<Peripheral::Sai1>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ck1;
	static constexpr int af = 3;
	inline static void
	connect()
	{
		setAlternateFunction(3);
	}
};
template<>
struct GpioA3::Clk<Peripheral::Quadspi1>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Clk;
	static constexpr int af = 10;
	inline static void
	connect()
	{
		setAlternateFunction(10);
	}
};
template<>
struct GpioA3::In4<Peripheral::Adc1>
{
	using Gpio = GpioA3;
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
GpioA3::AdcChannel<Peripheral::Adc1> = 4;
template<>
struct GpioA3::Inp<Peripheral::Comp2>
{
	using Gpio = GpioA3;
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
struct GpioA3::Mclka<Peripheral::Sai1>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Mclka;
	static constexpr int af = 13;
	inline static void
	connect()
	{
		setAlternateFunction(13);
	}
};
template<>
struct GpioA3::Rx<Peripheral::Usart2>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Rx;
	static constexpr int af = 7;
	inline static void
	connect()
	{
		setAlternateFunction(7);
	}
};
template<>
struct GpioA3::Rx<Peripheral::Lpuart1>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Rx;
	static constexpr int af = 12;
	inline static void
	connect()
	{
		setAlternateFunction(12);
	}
};
template<>
struct GpioA3::Vinm<Peripheral::Opamp1>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA3::Vinm<Peripheral::Opamp5>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA3::Vinm0<Peripheral::Opamp1>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm0;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA3::Vinm1<Peripheral::Opamp5>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm1;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA3::Vinmsec<Peripheral::Opamp1>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinmsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA3::Vinmsec<Peripheral::Opamp5>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinmsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA3::Vinp<Peripheral::Opamp1>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinp;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
template<>
struct GpioA3::Vinpsec<Peripheral::Opamp1>
{
	using Gpio = GpioA3;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Vinpsec;
	static constexpr int af = -1;
	inline static void
	connect()
	{
	}
};
/// @endcond

} // namespace modm::platform

#endif // MODM_STM32_GPIO_PIN_A3_HPP