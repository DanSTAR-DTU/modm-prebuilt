/*
 * Copyright (c) 2019, Niklas Hauser
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include <CrashCatcher.h>

extern uint32_t __stack_start[];
extern uint32_t __stack_end[];
extern uint32_t __data_start[];
extern uint32_t __data_end[];
extern uint32_t __bss_start[];
extern uint32_t __bss_end[];
extern uint32_t __fastdata_start[];
extern uint32_t __fastdata_end[];
static const CrashCatcherMemoryRegion regions[] = {
	{ (uint32_t)__stack_start, (uint32_t)__stack_end, CRASH_CATCHER_WORD },
	{ (uint32_t)__data_start, (uint32_t)__data_end, CRASH_CATCHER_WORD },
	{ (uint32_t)__bss_start, (uint32_t)__bss_end, CRASH_CATCHER_WORD },
	{ (uint32_t)__fastdata_start, (uint32_t)__fastdata_end, CRASH_CATCHER_WORD },
	{ 0xFFFFFFFF, 0xFFFFFFFF, CRASH_CATCHER_BYTE }
};

const CrashCatcherMemoryRegion*
CrashCatcher_GetMemoryRegions(void)
{
	return regions;
}