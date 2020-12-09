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

#ifndef MODM_STM32_GPIO_PIN_UNUSED_HPP
#define MODM_STM32_GPIO_PIN_UNUSED_HPP

#include "base.hpp"
#include <modm/architecture/interface/gpio.hpp>

namespace modm
{

namespace platform
{

/**
 * Dummy implementation of an I/O pin.
 *
 * This class can be used when a pin is not required. All functions
 * are dummy functions which do nothing. `read()` will always
 * return `false`.
 *
 * For example when creating a software SPI with the modm::SoftwareSimpleSpi
 * class and the return channel (MISO - Master In Slave Out) is not needed,
 * a good way is to use this class as a parameter when defining the
 * SPI class.
 *
 * Example:
 * @code
 * #include <modm/architecture/platform.hpp>
 *
 * namespace pin
 * {
 *     typedef GpioOutputD7 Clk;
 *     typedef GpioOutputD5 Mosi;
 * }
 *
 * modm::SoftwareSpiMaster< pin::Clk, pin::Mosi, GpioUnused > Spi;
 *
 * ...
 * Spi::write(0xaa);
 * @endcode
 *
 * @author	Fabian Greif
 * @author	Niklas Hauser
 * @ingroup	modm_platform_gpio
 */
class GpioUnused : public Gpio, public ::modm::GpioIO
{
public:
	using Output = GpioUnused;
	using Input = GpioUnused;
	using IO = GpioUnused;
	using Type = GpioUnused;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port(-1);
	static constexpr uint8_t pin = uint8_t(-1);
	static constexpr uint16_t mask = 0;

protected:
	/// @cond
	static void setAlternateFunction(uint8_t) {}
	static void setAnalogInput() {}
	/// @endcond

public:
	// GpioOutput
	// start documentation inherited
	static void setOutput() {}
	static void setOutput(bool) {}
	static void set() {}
	static void set(bool) {}
	static void reset() {}
	static void toggle() {}
	static bool isSet() { return false; }
	// stop documentation inherited
	static void configure(OutputType, OutputSpeed = OutputSpeed::MHz50) {}
	static void setOutput(OutputType, OutputSpeed = OutputSpeed::MHz50) {}

	// GpioInput
	// start documentation inherited
	static void setInput() {}
	static bool read() { return false; }
	// end documentation inherited
	static void configure(InputType) {}
	static void setInput(InputType) {}
	// External Interrupts
	static void enableExternalInterrupt() {}
	static void disableExternalInterrupt() {}
	static void enableExternalInterruptVector(const uint32_t) {}
	static void disableExternalInterruptVector() {}
	static void setInputTrigger(const InputTrigger) {}
	static bool getExternalInterruptFlag() { return false; }
	/// Reset the interrupt flag in the interrupt routine.
	static void acknowledgeExternalInterruptFlag() {}

	// GpioIO
	// start documentation inherited
	static Direction getDirection() { return Direction::Special; }
	// end documentation inherited
	static void lock() {}
	static void disconnect() {}

public:
	/// @cond
	template< Peripheral _ >
	struct Bk1io0
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bk1io0;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bk1io1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bk1io1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bk1io2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bk1io2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bk1io3
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bk1io3;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bk1ncs
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bk1ncs;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bk2io0
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bk2io0;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bk2io1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bk2io1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bk2io2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bk2io2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bk2io3
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bk2io3;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bkin
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bkin;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Bkin2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Bkin2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Cc1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Cc1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Cc2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Cc2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ch1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ch1n
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ch1n;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ch2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ch2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ch2n
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ch2n;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ch3
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ch3;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ch3n
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ch3n;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ch4
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ch4;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ch4n
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ch4n;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Cha1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Cha1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Cha2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Cha2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Chb1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Chb1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Chb2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Chb2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Chc1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Chc1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Chc2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Chc2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Chd1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Chd1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Chd2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Chd2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Che1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Che1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Che2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Che2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Chf1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Chf1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Chf2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Chf2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ck
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ck;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ck1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ck1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ck2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ck2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ckin
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ckin;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Clk
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Clk;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Crssync
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Crssync;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Cts
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Cts;
		static void connect() {}
	};
	template< Peripheral _ >
	struct D1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::D1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct D2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::D2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct D3
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::D3;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Dbcc1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Dbcc1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Dbcc2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Dbcc2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct De
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::De;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Dm
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Dm;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Dp
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Dp;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev10
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev10;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev3
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev3;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev4
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev4;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev5
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev5;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev6
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev6;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev7
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev7;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev8
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev8;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Eev9
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Eev9;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Etr
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Etr;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Flt1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Flt1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Flt2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Flt2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Flt3
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Flt3;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Flt4
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Flt4;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Flt5
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Flt5;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Flt6
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Flt6;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Frstx1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Frstx1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Frstx2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Frstx2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Fsa
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Fsa;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Fsb
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Fsb;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In10
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In10;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In11
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In11;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In12
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In12;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In13
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In13;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In14
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In14;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In15
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In15;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In17
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In17;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In3
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In3;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In4
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In4;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In5
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In5;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In6
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In6;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In7
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In7;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In8
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In8;
		static void connect() {}
	};
	template< Peripheral _ >
	struct In9
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::In9;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Inm
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Inm;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Inp
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Inp;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Jtck
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Jtck;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Jtdi
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Jtdi;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Jtdo
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Jtdo;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Jtms
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Jtms;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Jtrst
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Jtrst;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Lsco
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Lsco;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Mck
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Mck;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Mclka
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Mclka;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Mclkb
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Mclkb;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Mco
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Mco;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Miso
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Miso;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Mosi
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Mosi;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Nss
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Nss;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Osc32in
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Osc32in;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Osc32out
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Osc32out;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Oscin
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Oscin;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Oscout
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Oscout;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Out
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Out;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Out1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Out1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Out2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Out2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Pvdin
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Pvdin;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Refin
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Refin;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Rts
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Rts;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Rx
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Rx;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Scin
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Scin;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Sck
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Sck;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Scka
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Scka;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Sckb
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Sckb;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Scl
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Scl;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Scout
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Scout;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Sd
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Sd;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Sda
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Sda;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Sdb
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Sdb;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Smba
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Smba;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Swclk
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Swclk;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Swdio
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Swdio;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Swo
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Swo;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Tamp1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Tamp1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Tamp2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Tamp2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ts
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ts;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Tx
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Tx;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Vinm
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Vinm0
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm0;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Vinm1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Vinm1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Vinmsec
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Vinmsec;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Vinp
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Vinp;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Vinpsec
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Vinpsec;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Vout
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Vout;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Wkup1
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Wkup1;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Wkup2
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Wkup2;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Wkup4
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Wkup4;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Wkup5
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Wkup5;
		static void connect() {}
	};
	template< Peripheral _ >
	struct Ws
	{
		using Gpio = GpioUnused;
		static constexpr Gpio::Signal Signal = Gpio::Signal::Ws;
		static void connect() {}
	};
	/// @endcond
};

} // namespace platform

} // namespace modm

#endif // MODM_STM32_GPIO_PIN_UNUSED_HPP