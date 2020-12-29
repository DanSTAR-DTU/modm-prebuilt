/*
 * Copyright (c) 2016, Sascha Schade
 * Copyright (c) 2017, Niklas Hauser
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include <modm/architecture.hpp>

#include "platform/1-wire/bitbang_master.hpp"
#include "platform/adc/adc_1.hpp"
#include "platform/adc/adc_2.hpp"
#include "platform/adc/adc_3.hpp"
#include "platform/adc/adc_4.hpp"
#include "platform/adc/adc_5.hpp"
#include "platform/can/can_bit_timings.hpp"
#include "platform/clock/rcc.hpp"
#include "platform/clock/systick_timer.hpp"
#include "platform/core/hardware_init.hpp"
#include "platform/core/heap_table.hpp"
#include "platform/core/peripherals.hpp"
#include "platform/fault/fault.hpp"
#include "platform/fault/fault_storage.hpp"
#include "platform/gpio/base.hpp"
#include "platform/gpio/connector.hpp"
#include "platform/gpio/connector_detail.hpp"
#include "platform/gpio/gpio_A0.hpp"
#include "platform/gpio/gpio_A1.hpp"
#include "platform/gpio/gpio_A10.hpp"
#include "platform/gpio/gpio_A11.hpp"
#include "platform/gpio/gpio_A12.hpp"
#include "platform/gpio/gpio_A13.hpp"
#include "platform/gpio/gpio_A14.hpp"
#include "platform/gpio/gpio_A15.hpp"
#include "platform/gpio/gpio_A2.hpp"
#include "platform/gpio/gpio_A3.hpp"
#include "platform/gpio/gpio_A4.hpp"
#include "platform/gpio/gpio_A5.hpp"
#include "platform/gpio/gpio_A6.hpp"
#include "platform/gpio/gpio_A7.hpp"
#include "platform/gpio/gpio_A8.hpp"
#include "platform/gpio/gpio_A9.hpp"
#include "platform/gpio/gpio_B0.hpp"
#include "platform/gpio/gpio_B1.hpp"
#include "platform/gpio/gpio_B10.hpp"
#include "platform/gpio/gpio_B11.hpp"
#include "platform/gpio/gpio_B12.hpp"
#include "platform/gpio/gpio_B13.hpp"
#include "platform/gpio/gpio_B14.hpp"
#include "platform/gpio/gpio_B15.hpp"
#include "platform/gpio/gpio_B2.hpp"
#include "platform/gpio/gpio_B3.hpp"
#include "platform/gpio/gpio_B4.hpp"
#include "platform/gpio/gpio_B5.hpp"
#include "platform/gpio/gpio_B6.hpp"
#include "platform/gpio/gpio_B7.hpp"
#include "platform/gpio/gpio_B8.hpp"
#include "platform/gpio/gpio_B9.hpp"
#include "platform/gpio/gpio_C0.hpp"
#include "platform/gpio/gpio_C1.hpp"
#include "platform/gpio/gpio_C10.hpp"
#include "platform/gpio/gpio_C11.hpp"
#include "platform/gpio/gpio_C12.hpp"
#include "platform/gpio/gpio_C13.hpp"
#include "platform/gpio/gpio_C14.hpp"
#include "platform/gpio/gpio_C15.hpp"
#include "platform/gpio/gpio_C2.hpp"
#include "platform/gpio/gpio_C3.hpp"
#include "platform/gpio/gpio_C4.hpp"
#include "platform/gpio/gpio_C5.hpp"
#include "platform/gpio/gpio_C6.hpp"
#include "platform/gpio/gpio_C7.hpp"
#include "platform/gpio/gpio_C8.hpp"
#include "platform/gpio/gpio_C9.hpp"
#include "platform/gpio/gpio_D2.hpp"
#include "platform/gpio/gpio_F0.hpp"
#include "platform/gpio/gpio_F1.hpp"
#include "platform/gpio/gpio_G10.hpp"
#include "platform/gpio/inverted.hpp"
#include "platform/gpio/port.hpp"
#include "platform/gpio/set.hpp"
#include "platform/gpio/software_port.hpp"
#include "platform/gpio/unused.hpp"
#include "platform/i2c/i2c_master_1.hpp"
#include "platform/i2c/i2c_master_2.hpp"
#include "platform/i2c/i2c_master_3.hpp"
#include "platform/i2c/i2c_master_4.hpp"
#include "platform/i2c/i2c_timing_calculator.hpp"
#include "platform/spi/bitbang_spi_master.hpp"
#include "platform/spi/spi_base.hpp"
#include "platform/spi/spi_hal_1.hpp"
#include "platform/spi/spi_hal_2.hpp"
#include "platform/spi/spi_hal_3.hpp"
#include "platform/spi/spi_master_1.hpp"
#include "platform/spi/spi_master_2.hpp"
#include "platform/spi/spi_master_3.hpp"
#include "platform/timer/advanced_base.hpp"
#include "platform/timer/basic_base.hpp"
#include "platform/timer/general_purpose_base.hpp"
#include "platform/timer/timer_1.hpp"
#include "platform/timer/timer_15.hpp"
#include "platform/timer/timer_16.hpp"
#include "platform/timer/timer_17.hpp"
#include "platform/timer/timer_2.hpp"
#include "platform/timer/timer_20.hpp"
#include "platform/timer/timer_3.hpp"
#include "platform/timer/timer_4.hpp"
#include "platform/timer/timer_5.hpp"
#include "platform/timer/timer_6.hpp"
#include "platform/timer/timer_7.hpp"
#include "platform/timer/timer_8.hpp"
#include "platform/uart/uart_1.hpp"
#include "platform/uart/uart_2.hpp"
#include "platform/uart/uart_3.hpp"
#include "platform/uart/uart_4.hpp"
#include "platform/uart/uart_5.hpp"
#include "platform/uart/uart_base.hpp"
#include "platform/uart/uart_baudrate.hpp"
#include "platform/uart/uart_hal_1.hpp"
#include "platform/uart/uart_hal_2.hpp"
#include "platform/uart/uart_hal_3.hpp"
#include "platform/uart/uart_hal_4.hpp"
#include "platform/uart/uart_hal_5.hpp"
#include "platform/can/fd_can1.hpp"
