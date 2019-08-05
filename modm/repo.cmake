# Copyright (c) 2018, Sergiy Yevtushenko
# Copyright (c) 2018-2019, Niklas Hauser
#
# This file is part of the modm project.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# This file was autogenerated by the modm cmake builder. Do not modify!

include(${CMAKE_CURRENT_LIST_DIR}/cmake/configure-gcc.cmake)
# Toolchain configuration
set(CPPDEFINES "\
")

set(CPPDEFINES_RELEASE "\
")

set(CPPDEFINES_DEBUG "\
    -DMODM_DEBUG_BUILD \
")


set(CCFLAGS "\
    -fdata-sections \
    -ffunction-sections \
    -finline-limit=10000 \
    -fshort-wchar \
    -fsingle-precision-constant \
    -funsigned-bitfields \
    -funsigned-char \
    -fwrapv \
    -g3 \
    -gdwarf \
    -W \
    -Wall \
    -Wdouble-promotion \
    -Wduplicated-cond \
    -Werror=format \
    -Werror=maybe-uninitialized \
    -Werror=overflow \
    -Werror=sign-compare \
    -Wextra \
    -Wlogical-op \
    -Wpointer-arith \
    -Wundef \
")

set(CCFLAGS_RELEASE "\
    -Os \
")

set(CCFLAGS_DEBUG "\
    -fno-move-loop-invariants \
    -fno-split-wide-types \
    -fno-tree-loop-optimize \
    -Og \
")


set(CFLAGS "\
    -std=gnu11 \
    -Wbad-function-cast \
    -Wimplicit \
    -Wredundant-decls \
    -Wstrict-prototypes \
")

set(CFLAGS_RELEASE "\
")

set(CFLAGS_DEBUG "\
")


set(CXXFLAGS "\
    -fno-exceptions \
    -fno-rtti \
    -fno-threadsafe-statics \
    -fno-unwind-tables \
    -fstrict-enums \
    -fuse-cxa-atexit \
    -std=c++17 \
    -Woverloaded-virtual \
")

set(CXXFLAGS_RELEASE "\
")

set(CXXFLAGS_DEBUG "\
")


set(ASFLAGS "\
    -g3 \
    -gdwarf \
")

set(ASFLAGS_RELEASE "\
")

set(ASFLAGS_DEBUG "\
")


set(ARCHFLAGS "\
    -mcpu=cortex-m4 \
    -mfloat-abi=hard \
    -mfpu=fpv4-sp-d16 \
    -mthumb \
")

set(ARCHFLAGS_RELEASE "\
")

set(ARCHFLAGS_DEBUG "\
")


set(LINKFLAGS "\
    --specs=nano.specs \
    --specs=nosys.specs \
    -L${CMAKE_CURRENT_LIST_DIR}/link \
    -nostartfiles \
    -Tlinkerscript.ld \
    -Wl,--build-id=sha1 \
    -Wl,--fatal-warnings \
    -Wl,--gc-sections \
    -Wl,--no-wchar-size-warning \
    -Wl,--relax \
    -Wl,-wrap,_calloc_r \
    -Wl,-wrap,_free_r \
    -Wl,-wrap,_malloc_r \
    -Wl,-wrap,_realloc_r \
")

set(LINKFLAGS_RELEASE "\
")

set(LINKFLAGS_DEBUG "\
")



string(APPEND CCFLAGS "${ARCHFLAGS}")
string(APPEND ASFLAGS "${ARCHFLAGS}")
string(APPEND LINKFLAGS "${ARCHFLAGS}")
string(APPEND CMAKE_C_FLAGS         "${CCFLAGS} ${CFLAGS}")
string(APPEND CMAKE_C_FLAGS_RELEASE "${CCFLAGS_RELEASE}")
string(APPEND CMAKE_C_FLAGS_DEBUG   "${CCFLAGS_DEBUG}")

string(APPEND CMAKE_CXX_FLAGS         "${CCFLAGS} ${CXXFLAGS}")
string(APPEND CMAKE_CXX_FLAGS_RELEASE "${CCFLAGS_RELEASE}")
string(APPEND CMAKE_CXX_FLAGS_DEBUG   "${CCFLAGS_DEBUG}")

string(APPEND CMAKE_ASM_FLAGS         "${ASFLAGS}")
string(APPEND CMAKE_ASM_FLAGS_RELEASE "${ASFLAGS_RELEASE}")
string(APPEND CMAKE_ASM_FLAGS_DEBUG   "${ASFLAGS_DEBUG}")

string(APPEND CMAKE_EXE_LINKER_FLAGS         "${LINKFLAGS}")
string(APPEND CMAKE_EXE_LINKER_FLAGS_RELEASE "${LINKFLAGS_RELEASE}")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG   "${LINKFLAGS_DEBUG}")

add_definitions("${CPPDEFINES}")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions("${CPPDEFINES_RELEASE}")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("${CPPDEFINES_DEBUG}")
endif()

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/ext
    ${CMAKE_CURRENT_LIST_DIR}/ext/cmsis/core
    ${CMAKE_CURRENT_LIST_DIR}/ext/cmsis/device
    ${CMAKE_CURRENT_LIST_DIR}/ext/crashcatcher/include
    ${CMAKE_CURRENT_LIST_DIR}/src
)
list(APPEND SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/ext/crashcatcher/src/CrashCatcher.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/crashcatcher/src/CrashCatcher_armv7m.S
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/architecture/driver/atomic/flag.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/architecture/interface/can.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/architecture/interface/can_message.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/architecture/interface/i2c.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/container/smart_pointer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/debug/error_report.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/io/iostream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/io/iostream_printf.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/math/geometry/angle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/math/geometry/vector2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/math/utils/bit_operation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/math/utils/pc/operator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/adc/adc_interrupt_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/adc/adc_interrupt_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/adc/adc_interrupt_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/can/can_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/can/can_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/can/can_filter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/clock/rcc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/clock/systick_timer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/assert.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/build_id.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/clock.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/cxxabi.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/delay.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/heap_newlib.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/heap_table.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/reset_handler.sx
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/startup.c
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/startup_platform.c
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/vectors.c
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/fault/crashcatcher_dump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/fault/crashcatcher_regions.c
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/fault/fault_storage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/gpio/enable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/i2c/i2c_master_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/i2c/i2c_master_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/i2c/i2c_master_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/spi/spi_master_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/spi/spi_master_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/spi/spi_master_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_10.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_11.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_12.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_13.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_14.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_4.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_5.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_6.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_7.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_8.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_9.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_4.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_5.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_6.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/utils/dummy.cpp
)
set_source_files_properties(
    ${CMAKE_CURRENT_LIST_DIR}/ext/crashcatcher/src/CrashCatcher_armv7m.S
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/reset_handler.sx
    PROPERTIES LANGUAGE CXX
)
