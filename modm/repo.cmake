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
    -gdwarf-3 \
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
    -Wno-redundant-decls \
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
    -fno-unwind-tables \
    -fstrict-enums \
    -fuse-cxa-atexit \
    -std=c++20 \
    -Wno-volatile \
    -Woverloaded-virtual \
")

set(CXXFLAGS_RELEASE "\
")

set(CXXFLAGS_DEBUG "\
")


set(ASFLAGS "\
    -g3 \
    -gdwarf-3 \
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



SET(CMAKE_C_FLAGS "${ARCHFLAGS} ${CCFLAGS} ${CFLAGS}" CACHE INTERNAL "c compiler flags")
SET(CMAKE_C_FLAGS_RELEASE   "${CCFLAGS_RELEASE}"      CACHE INTERNAL "c compiler flags release")
SET(CMAKE_C_FLAGS_DEBUG     "${CCFLAGS_DEBUG}"        CACHE INTERNAL "c compiler flags debug")

SET(CMAKE_CXX_FLAGS "${ARCHFLAGS} ${CCFLAGS} ${CXXFLAGS}" CACHE INTERNAL "cxx compiler flags")
SET(CMAKE_CXX_FLAGS_RELEASE "${CCFLAGS_RELEASE}"          CACHE INTERNAL "cxx compiler flags release")
SET(CMAKE_CXX_FLAGS_DEBUG   "${CCFLAGS_DEBUG}"            CACHE INTERNAL "cxx compiler flags debug")

SET(CMAKE_ASM_FLAGS "${ARCHFLAGS} ${ASFLAGS}" CACHE INTERNAL "asm compiler flags")
SET(CMAKE_ASM_FLAGS_RELEASE "${ASFLAGS_RELEASE}" CACHE INTERNAL "asm compiler flags release")
SET(CMAKE_ASM_FLAGS_DEBUG   "${ASFLAGS_DEBUG}" CACHE INTERNAL "asm compiler flags debug")

SET(CMAKE_EXE_LINKER_FLAGS  "${ARCHFLAGS} ${LINKFLAGS}" CACHE INTERNAL "linker flags")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${LINKFLAGS_RELEASE}" CACHE INTERNAL "linker flags release")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG   "${LINKFLAGS_DEBUG}" CACHE INTERNAL "linker flags debug")

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
    ${CMAKE_CURRENT_LIST_DIR}/ext/crashcatcher/src/CrashCatcher_armv7m.sx
    ${CMAKE_CURRENT_LIST_DIR}/ext/gcc/cabi.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/gcc/cxxabi.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ext/gcc/new_delete.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ext/tlsf/tlsf.c
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/architecture/driver/atomic/flag.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/architecture/interface/can.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/container/smart_pointer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/driver/storage/i2c_eeprom.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/io/iostream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/io/iostream_printf.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/math/geometry/angle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/math/geometry/vector2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/math/utils/bit_operation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/math/utils/pc/operator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/clock/rcc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/clock/systick_timer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/assert.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/build_id.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/delay.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/heap_table.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/reset_handler.sx
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/startup.c
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/startup_platform.c
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/vectors.c
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/fault/crashcatcher_dump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/fault/crashcatcher_regions.c
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/fault/fault_storage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/gpio/enable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/heap/heap_tlsf.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/i2c/i2c_master_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/i2c/i2c_master_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/i2c/i2c_master_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/i2c/i2c_master_4.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/spi/spi_master_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/spi/spi_master_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/spi/spi_master_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_15.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_16.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_17.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_20.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_4.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_5.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_6.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_7.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/timer/timer_8.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_4.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/uart/uart_5.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/can/fd_can1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/can/fd_can2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/can/fd_can3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/utils/dummy.cpp
)
set_source_files_properties(
    ${CMAKE_CURRENT_LIST_DIR}/ext/crashcatcher/src/CrashCatcher_armv7m.sx
    ${CMAKE_CURRENT_LIST_DIR}/src/modm/platform/core/reset_handler.sx
    PROPERTIES LANGUAGE CXX
)
