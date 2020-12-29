# Copyright (c) 2018, Sergiy Yevtushenko
# Copyright (c) 2018-2019, Niklas Hauser
#
# This file is part of the modm project.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
find_package(PkgConfig)

if(NOT TOOL_EXECUTABLE_PREFIX)
    set(TOOL_EXECUTABLE_PREFIX "arm-none-eabi-")
endif()

if(WIN32)
    set(TOOL_EXECUTABLE_SUFFIX ".exe")
else()
    set(TOOL_EXECUTABLE_SUFFIX "")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER "${TOOL_EXECUTABLE_PREFIX}gcc${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "c")
set(CMAKE_CXX_COMPILER "${TOOL_EXECUTABLE_PREFIX}g++${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "c++")
set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}"     CACHE INTERNAL "asm")

set(CMAKE_AR           "${TOOL_EXECUTABLE_PREFIX}ar"      CACHE INTERNAL "ar")
set(CMAKE_RANLIB       "${TOOL_EXECUTABLE_PREFIX}ranlib"  CACHE INTERNAL "ranlib")
if(TOOL_EXECUTABLE_SUFFIX STREQUAL "")
    set(CMAKE_AS       "${TOOL_EXECUTABLE_PREFIX}as"      CACHE INTERNAL "as")
    set(CMAKE_NM       "${TOOL_EXECUTABLE_PREFIX}nm"      CACHE INTERNAL "nm")
else()
    set(CMAKE_AS       "${CMAKE_CXX_COMPILER}"            CACHE INTERNAL "as")
    set(CMAKE_NM       "${TOOL_EXECUTABLE_PREFIX}gcc-nm${TOOL_EXECUTABLE_SUFFIX}"     CACHE INTERNAL "nm")
    set(CMAKE_AR       "${TOOL_EXECUTABLE_PREFIX}gcc-ar${TOOL_EXECUTABLE_SUFFIX}"     CACHE INTERNAL "ar")
    set(CMAKE_RANLIB   "${TOOL_EXECUTABLE_PREFIX}gcc-ranlib${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "ranlib")
endif()

set(CMAKE_OBJCOPY      "${TOOL_EXECUTABLE_PREFIX}objcopy" CACHE INTERNAL "objcopy")
set(CMAKE_OBJDUMP      "${TOOL_EXECUTABLE_PREFIX}objdump" CACHE INTERNAL "objdump")
set(CMAKE_SIZE         "${TOOL_EXECUTABLE_PREFIX}size"    CACHE INTERNAL "size")
set(CMAKE_STRIP        "${TOOL_EXECUTABLE_PREFIX}strip"   CACHE INTERNAL "strip")
set(CMAKE_LINKER       "${CMAKE_CXX_COMPILER}"            CACHE INTERNAL "linker")
set(CMAKE_DEBUGGER     "${TOOL_EXECUTABLE_PREFIX}gdb"     CACHE INTERNAL "debugger")
set(CMAKE_CPPFILT      "${TOOL_EXECUTABLE_PREFIX}c++filt" CACHE INTERNAL "C++filt")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

if(APPLE)
    # CMake adds system search paths on macOS for some reason
    if(CMAKE_C_LINK_FLAGS)
        string(REPLACE "-Wl,-search_paths_first" "" CMAKE_C_LINK_FLAGS ${CMAKE_C_LINK_FLAGS})
    endif()
    if (CMAKE_CXX_LINK_FLAGS)
        string(REPLACE "-Wl,-search_paths_first" "" CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS})
    endif()
endif()