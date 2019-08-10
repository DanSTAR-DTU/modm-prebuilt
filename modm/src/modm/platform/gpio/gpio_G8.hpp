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

#ifndef MODM_STM32_GPIO_PIN_G8_HPP
#define MODM_STM32_GPIO_PIN_G8_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm::platform
{

/// @cond
class GpioG8;
using GpioOutputG8 = GpioG8;
using GpioInputG8  = GpioG8;
/// @endcond

/// IO class for Pin G8
/// @ingroup	modm_platform_gpio
class GpioG8 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioG8>;
	friend class Adc;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioG8;
	using Input = GpioG8;
	using IO = GpioG8;
	using Type = GpioG8;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::G; ///< Port name
	static constexpr uint8_t pin = 8; ///< Pin number

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
	/// ExternalInterruptIRQ
	static constexpr IRQn_Type ExternalInterruptIRQ = EXTI9_5_IRQn;

public:
	/// @cond
	inline static void setAlternateFunction(uint8_t af) {
		GPIOG->AFR[af_id] = (GPIOG->AFR[af_id] & ~af_mask) | ((af & 0xf) << af_offset);
		GPIOG->MODER = (GPIOG->MODER & ~mask2) | (i(Mode::AlternateFunction) << (pin * 2));
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
	inline static void toggle() {
		if (isSet()) { reset(); }
		else         { set();   }
	}
	inline static bool isSet() { return (GPIOG->ODR & mask); }
	// stop documentation inherited
	inline static void configure(OutputType type, OutputSpeed speed = OutputSpeed::MHz50) { PinSet::configure(type, speed); }
	inline static void setOutput(OutputType type, OutputSpeed speed = OutputSpeed::MHz50) { PinSet::setOutput(type, speed); }
	// GpioInput
	// start documentation inherited
	inline static void setInput() { PinSet::setInput(); }
	inline static bool read() { return (GPIOG->IDR & mask); }
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
		EXTI->IMR |= mask;
	}
	inline static void disableExternalInterrupt() { EXTI->IMR &= ~mask; }
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
			EXTI->RTSR |=  mask;
			EXTI->FTSR &= ~mask;
			break;
		case InputTrigger::FallingEdge:
			EXTI->RTSR &= ~mask;
			EXTI->FTSR |=  mask;
			break;
		case InputTrigger::BothEdges:
			EXTI->RTSR |=  mask;
			EXTI->FTSR |=  mask;
			break;
		}
	}
	inline static bool getExternalInterruptFlag() { return (EXTI->PR & mask); }
	inline static void acknowledgeExternalInterruptFlag() { EXTI->PR = mask; }
	// GpioIO
	// start documentation inherited
	inline static Direction getDirection() {
		uint32_t mode = (GPIOG->MODER & mask2);
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
		GPIOG->AFR[af_id] &= ~af_mask;
	}

public:
#ifdef __DOXYGEN__
	/// @{
	/// Connect to any software peripheral
	using BitBang = GpioSignal;
	/// Connect to Usart6
	using De = GpioSignal;
	/// Connect to Spdifrx
	using In2 = GpioSignal;
	/// Connect to Spi6
	using Nss = GpioSignal;
	/// Connect to Eth
	using PpsOut = GpioSignal;
	/// Connect to Usart6
	using Rts = GpioSignal;
	/// Connect to Fmc
	using Sdclk = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioG8::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct De { static void connect();
		static_assert(
			(peripheral == Peripheral::Usart6),
			"GpioG8::De only connects to Usart6!");
	};
	template< Peripheral peripheral >
	struct In2 { static void connect();
		static_assert(
			(peripheral == Peripheral::Spdifrx),
			"GpioG8::In2 only connects to Spdifrx!");
	};
	template< Peripheral peripheral >
	struct Nss { static void connect();
		static_assert(
			(peripheral == Peripheral::Spi6),
			"GpioG8::Nss only connects to Spi6!");
	};
	template< Peripheral peripheral >
	struct PpsOut { static void connect();
		static_assert(
			(peripheral == Peripheral::Eth),
			"GpioG8::PpsOut only connects to Eth!");
	};
	template< Peripheral peripheral >
	struct Rts { static void connect();
		static_assert(
			(peripheral == Peripheral::Usart6),
			"GpioG8::Rts only connects to Usart6!");
	};
	template< Peripheral peripheral >
	struct Sdclk { static void connect();
		static_assert(
			(peripheral == Peripheral::Fmc),
			"GpioG8::Sdclk only connects to Fmc!");
	};
	/// @endcond
private:
	template< Peripheral peripheral >
	static constexpr int8_t AdcChannel = -1;
};

/// @cond
template<>
struct GpioG8::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioG8;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioG8::De<Peripheral::Usart6>
{
	using Gpio = GpioG8;
	static constexpr Gpio::Signal Signal = Gpio::Signal::De;
	static constexpr int af = 8;
	inline static void
	connect()
	{
		setAlternateFunction(8);
	}
};
template<>
struct GpioG8::In2<Peripheral::Spdifrx>
{
	using Gpio = GpioG8;
	static constexpr Gpio::Signal Signal = Gpio::Signal::In2;
	static constexpr int af = 7;
	inline static void
	connect()
	{
		setAlternateFunction(7);
	}
};
template<>
struct GpioG8::Nss<Peripheral::Spi6>
{
	using Gpio = GpioG8;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Nss;
	static constexpr int af = 5;
	inline static void
	connect()
	{
		setAlternateFunction(5);
	}
};
template<>
struct GpioG8::PpsOut<Peripheral::Eth>
{
	using Gpio = GpioG8;
	static constexpr Gpio::Signal Signal = Gpio::Signal::PpsOut;
	static constexpr int af = 11;
	inline static void
	connect()
	{
		setAlternateFunction(11);
	}
};
template<>
struct GpioG8::Rts<Peripheral::Usart6>
{
	using Gpio = GpioG8;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Rts;
	static constexpr int af = 8;
	inline static void
	connect()
	{
		setAlternateFunction(8);
	}
};
template<>
struct GpioG8::Sdclk<Peripheral::Fmc>
{
	using Gpio = GpioG8;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Sdclk;
	static constexpr int af = 12;
	inline static void
	connect()
	{
		setAlternateFunction(12);
	}
};
/// @endcond

} // namespace modm::platform

#endif // MODM_STM32_GPIO_PIN_G8_HPP