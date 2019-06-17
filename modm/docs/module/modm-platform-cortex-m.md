!!! warning "These module docs are in beta and may be incomplete."

# modm:platform:cortex-m: ARM Cortex-M Core

This module generates the startup code, the vector table, the linkerscript and
provides runtime support for dynamic memory allocations on the heap and
assertion handling.

## Linkerscript

The linkerscript is generated for the devices memory map.

### Placement of `.fastcode` section

From the Cortex-M3 Technical Reference Manual:

> 14.5 System Interface:
>
> The system interface is a 32-bit AHB-Lite bus. Instruction and vector fetches,
> and data and debug accesses to the System memory space, 0x20000000 - 0xDFFFFFFF,
> 0xE0100000 - 0xFFFFFFFF, are performed over this bus.
>
> 14.5.6 Pipelined instruction fetches:
>
> To provide a clean timing interface on the System bus, instruction and vector
> fetch requests to this bus are registered. This results in an additional cycle
> of latency because instructions fetched from the System bus take two cycles.
> This also means that back-to-back instruction fetches from the System bus are
> not possible.
>
> Note:
>   Instruction fetch requests to the ICode bus are not registered.
>   Performance critical code must run from the ICode interface.

So for STM32s where the CCM is _not_ connected to the I-Bus, we execute
`.fastcode` from Flash.

## Options
#### stack_execution_guard

Fill the stack with relative jumps to fault handler to prevent accidental execution.: `False` ∈ `{ True, False }`


#### allocator

Dynamic memory allocation strategy: `newlib` ∈ `{ block, newlib, tlsf }`

By default, the arm-none-eabi toolchain ships with the `newlib` libc, which uses
`dlmalloc` as the underlying allocator algorithm and only requires the
implementation of the `void * sbrk(ptrdiff_t size)` hook. However, this limits
the allocator to use just _one_ memory region, which must then also be of
_continuous_ extend, since `sbrk` can only grow and shrink, but not jump.
Therefore, when using the `newlib` strategy, only the largest memory region is
used as heap! Depending on the device memory architecture this can leave large
memory regions unused.

For devices with very small memories, we recommend using the block allocator
strategy, which uses a very light-weight and simple algorithm. This also only
operates on one continuous memory region as heap.

!!! note
    Memories can have different traits, such as DMA-ability or access time. The
    default memory allocator functions (malloc, new, etc) only return DMA-able
    memories, ordered by fastest access time. Similarly the search for the
    largest memory region only considers DMA-able memory.

!!! warning
    For devices which contain separate memories laid out in a continuous way
    (often called SRAM1, SRAM2, etc.) the `newlib` and `block` strategies choose
    the largest continuous memory region, *even though* unaligned accesses
    across memory regions may not be supported in hardware and lead to a bus
    fault! Consider using the TLSF implementation, which does not suffer from
    this issue.

To use all non-statically allocated memory for heap, use the TLSF strategy,
which natively supports multiple memory regions. Our implementation treats
all internal memories as separate regions, so unaligned accesses across memory
boundaries are not an issue. To request heap memory of different traits, see
`modm::MemoryTraits`.

!!! note
    The TLSF implementation has a static overhead of about 1kB per memory trait
    group, however, these can then contain multiple non-continuous memory
    regions. The upside of this large static allocation is very fast allocation
    times of O(1), but we recommend using TLSF only for devices with multiple
    memory regions.
#### main_stack_size

Minimum size of the application main stack: `3040` ∈ `{ 256 .. 3040 .. 65536 }`

The ARM Cortex-M uses a descending stack mechanism which is placed so that it
grows towards the beginning of RAM. In case of a stack overflow the hardware
then attempts to stack into invalid memory which triggers a HardFault. A stack
overflow will therefore never overwrite any static or heap memory and this
protection works without the MPU and therefore also on ARM Cortex-M0 devices.

If you enable either the LED or the logging HardFault option, a smaller stack
is added above the main stack. This stack is only used by the HardFault handler
when not enough memory remains in the main stack to preserve GDB backtrace
behavior. This memory also acts as a small safety buffer against main stack
underflow, which is not detected however.

If the vector table is relocated into RAM, the start address needs to be aligned
to the next highest power-of-two word depending on the total number of device
interrupts. On devices where the table is relocated into the same memory as the
main stack, an alignment buffer up to 1kB is added to the main stack.

```
|              ...                |
|---------------------------------|
|    Interrupt Vectors (in RAM)   |
|        (if re-mapped)           | <-- vector table origin
|---------------------------------| <-- HardFault stack top
|        HardFault Stack          |
|       (grows downwards)         |
|               |                 |
|               v                 |
|---------------------------------| <-- main stack top
|           Main Stack            |
|       (grows downwards)         |
|               |                 |
|               v                 |
|---------------------------------|
|  Alignment buffer for vectors   |
|   (overwritten by main stack!)  |
'---------------------------------' <-- RAM origin
```

!!! warning
    The main stack size you provide is a minimum and may be enlarged to satisfy
    alignment requirements. Be aware that these requirements operate on the sum
    of HardFault and main stack. Disabling HardFault options may therefore
    decrease the alignment buffer added to the main stack size, which may make
    your application overflow stack. You need to increase your minimum main
    stack size in that case.

!!! note
    The main stack is watermarked and you can get the maximum stack usage using
    the `uint32_t modm::cortex::getMaximumStackUsage()` function.
#### vector_table_location

Vector table location in ROM or RAM: `rom` ∈ `{ ram, rom }`

The vector table is always stored in ROM and copied to RAM by the startup script
if required. You can modify the RAM vector table using the CMSIS NVIC functions:

- `void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector)`
- `uint32_t NVIC_GetVector(IRQn_Type IRQn)`

For applications that do not modify the vector table at runtime, relocation to
RAM is not necessary and can save a few hundred bytes of static memory.

By default, the fastest option is chosen depending on the target memory
architecture. This does not always mean the table is copied into RAM, and
therefore may not be modifiable with this option!

From the ARM Cortex-M4 Technical Reference Manual on exception handling:

> - Processor state is automatically stored to the stack on an exception, and
>   automatically restored from the stack at the end of the Interrupt Service
>   Routine.
> - The vector is fetched in parallel to the state saving, enabling efficient
>   interrupt entry.

!!! warning
    Placing main stack and vector table into the same memory can significantly
    slow down interrupt latency, since both I-Code and D-Code memory interface
    need to fetch from the same access port.
#### linkerscript.flash_offset

Add an offset to the default start address of the flash memory. This might be required for bootloaders located there. WARNING: Not all offsets are compatible with the vector table relocation.: `0` ∈ `{ 0 ... 524288 }`


## Content

```cpp
// Function
void _delay_ms(uint16_t ms);
void _delay_ns(uint16_t ns);
void _delay_us(uint16_t us);
```
## Collectors
#### linkerscript.memory

Additions to the linkerscript's 'MEMORY' ∈ `String`


#### linkerscript.process_stack_size

Maximum required size of the process stack ∈ `-Inf ... +Inf`


#### linkerscript.sections

Additions to the linkerscript's 'SECTIONS' ∈ `String`


#### linkerscript.table_extern.copy

Additions to the linkerscript's '.table.copy.extern' section ∈ `String`


#### linkerscript.table_extern.heap

Additions to the linkerscript's '.table.heap' section ∈ `String`


#### linkerscript.table_extern.zero

Additions to the linkerscript's '.table.zero.extern' section ∈ `String`


## Queries
#### linkerscript

Computes linkerscript properties *post-build*:
  - process_stack_size: largest requested process stack size by any module
  - vector_table_location: ram or rom

Stripped and newline-joined collector values of:
  - linkerscript_memory
  - linkerscript_sections
  - linkerscript_extern_zero
  - linkerscript_extern_copy
  - linkerscript_extern_heap

Additional memory properties:
  - memories: unfiltered memory regions
  - regions: memory region names
  - ram_origin: Lowest SRAM origin address
  - ram_origin: Total size of all SRAM regions

:returns: dictionary of linkerscript properties
#### vector_table

Computes vector table properties:
  - vector_table: [position] = Full vector name (ie. *with* _Handler or _IRQHandler suffix)
  - vector_table_location: rom or ram
  - highest_irq: highest IRQ number + 1
  - core: cortex-m{0,3,4,7}{,+,f,fd}

The system vectors start at -16, so you must add 16 to `highest_irq` to get
the total number of vectors in the table!

:returns: a dictionary of vector table properties
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:platform:cortex&#45;m Pages: 1 -->
<svg width="1252pt" height="239pt"
 viewBox="0.00 0.00 1252.00 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:platform:cortex&#45;m</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-235 1248,-235 1248,4 -4,4"/>
<!-- modm_platform_cortex_m -->
<g id="node1" class="node">
<title>modm_platform_cortex_m</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="688,-142 620,-142 620,-89 688,-89 688,-142"/>
<text text-anchor="middle" x="654" y="-126.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="654" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="654" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">cortex&#45;m</text>
</g>
<!-- modm_architecture_accessor -->
<g id="node2" class="node">
<title>modm_architecture_accessor</title>
<g id="a_node2"><a xlink:href="../modm-architecture-accessor" xlink:title="modm:&#10;architecture:&#10;accessor">
<polygon fill="#d3d3d3" stroke="#000000" points="84,-231 0,-231 0,-178 84,-178 84,-231"/>
<text text-anchor="middle" x="42" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="42" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="42" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">accessor</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_accessor -->
<g id="edge1" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_accessor</title>
<path fill="none" stroke="#000000" d="M619.9139,-117.653C517.152,-124.3941 210.7724,-146.3885 93.8132,-178.3375"/>
<polygon fill="#000000" stroke="#000000" points="92.7109,-175.0129 84.0791,-181.1563 94.658,-181.7366 92.7109,-175.0129"/>
</g>
<!-- modm_architecture_assert -->
<g id="node3" class="node">
<title>modm_architecture_assert</title>
<g id="a_node3"><a xlink:href="../modm-architecture-assert" xlink:title="modm:&#10;architecture:&#10;assert">
<polygon fill="#d3d3d3" stroke="#000000" points="186,-231 102,-231 102,-178 186,-178 186,-231"/>
<text text-anchor="middle" x="144" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="144" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="144" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">assert</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_assert -->
<g id="edge2" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_assert</title>
<path fill="none" stroke="#000000" d="M619.7263,-116.1799C543.8148,-118.52 356.2166,-128.912 196.1233,-178.099"/>
<polygon fill="#000000" stroke="#000000" points="194.6,-174.9085 186.105,-181.2398 196.6941,-181.5879 194.6,-174.9085"/>
</g>
<!-- modm_architecture_atomic -->
<g id="node4" class="node">
<title>modm_architecture_atomic</title>
<g id="a_node4"><a xlink:href="../modm-architecture-atomic" xlink:title="modm:&#10;architecture:&#10;atomic">
<polygon fill="#d3d3d3" stroke="#000000" points="288,-231 204,-231 204,-178 288,-178 288,-231"/>
<text text-anchor="middle" x="246" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="246" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="246" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">atomic</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_atomic -->
<g id="edge3" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_atomic</title>
<path fill="none" stroke="#000000" d="M619.9278,-117.9843C557.1062,-123.2987 419.1149,-138.4041 297.9523,-178.0905"/>
<polygon fill="#000000" stroke="#000000" points="296.5213,-174.8784 288.1474,-181.3691 298.7412,-181.5171 296.5213,-174.8784"/>
</g>
<!-- modm_architecture_build_id -->
<g id="node5" class="node">
<title>modm_architecture_build_id</title>
<g id="a_node5"><a xlink:href="../modm-architecture-build-id" xlink:title="modm:&#10;architecture:&#10;build_id">
<polygon fill="#d3d3d3" stroke="#000000" points="390,-231 306,-231 306,-178 390,-178 390,-231"/>
<text text-anchor="middle" x="348" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="348" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="348" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">build_id</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_build_id -->
<g id="edge4" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_build_id</title>
<path fill="none" stroke="#000000" d="M619.8114,-121.1908C571.9804,-129.7273 482.5357,-147.7878 399.7655,-178.0429"/>
<polygon fill="#000000" stroke="#000000" points="398.3775,-174.8249 390.2282,-181.5954 400.821,-181.3846 398.3775,-174.8249"/>
</g>
<!-- modm_architecture_clock -->
<g id="node6" class="node">
<title>modm_architecture_clock</title>
<g id="a_node6"><a xlink:href="../modm-architecture-clock" xlink:title="modm:&#10;architecture:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="492,-231 408,-231 408,-178 492,-178 492,-231"/>
<text text-anchor="middle" x="450" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="450" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="450" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_clock -->
<g id="edge5" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_clock</title>
<path fill="none" stroke="#000000" d="M619.89,-128.4021C590.5826,-139.715 547.0921,-157.0414 501.3319,-177.9834"/>
<polygon fill="#000000" stroke="#000000" points="499.7663,-174.8512 492.1531,-182.2193 502.6995,-181.207 499.7663,-174.8512"/>
</g>
<!-- modm_architecture_delay -->
<g id="node7" class="node">
<title>modm_architecture_delay</title>
<g id="a_node7"><a xlink:href="../modm-architecture-delay" xlink:title="modm:&#10;architecture:&#10;delay">
<polygon fill="#d3d3d3" stroke="#000000" points="594,-231 510,-231 510,-178 594,-178 594,-231"/>
<text text-anchor="middle" x="552" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="552" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="552" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">delay</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_delay -->
<g id="edge6" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_delay</title>
<path fill="none" stroke="#000000" d="M623.3966,-142.2029C613.0082,-151.2673 601.268,-161.5112 590.3528,-171.0353"/>
<polygon fill="#000000" stroke="#000000" points="587.8156,-168.6041 582.5818,-177.8159 592.4178,-173.8785 587.8156,-168.6041"/>
</g>
<!-- modm_architecture_heap -->
<g id="node8" class="node">
<title>modm_architecture_heap</title>
<g id="a_node8"><a xlink:href="../modm-architecture-heap" xlink:title="modm:&#10;architecture:&#10;heap">
<polygon fill="#d3d3d3" stroke="#000000" points="696,-231 612,-231 612,-178 696,-178 696,-231"/>
<text text-anchor="middle" x="654" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="654" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="654" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">heap</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_heap -->
<g id="edge7" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_heap</title>
<path fill="none" stroke="#000000" d="M654,-142.2029C654,-150.2113 654,-159.1403 654,-167.6802"/>
<polygon fill="#000000" stroke="#000000" points="650.5001,-167.8159 654,-177.8159 657.5001,-167.8159 650.5001,-167.8159"/>
</g>
<!-- modm_architecture_interrupt -->
<g id="node9" class="node">
<title>modm_architecture_interrupt</title>
<g id="a_node9"><a xlink:href="../modm-architecture-interrupt" xlink:title="modm:&#10;architecture:&#10;interrupt">
<polygon fill="#d3d3d3" stroke="#000000" points="798,-231 714,-231 714,-178 798,-178 798,-231"/>
<text text-anchor="middle" x="756" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="756" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="756" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">interrupt</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_interrupt -->
<g id="edge8" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_interrupt</title>
<path fill="none" stroke="#000000" d="M684.6034,-142.2029C694.9918,-151.2673 706.732,-161.5112 717.6472,-171.0353"/>
<polygon fill="#000000" stroke="#000000" points="715.5822,-173.8785 725.4182,-177.8159 720.1844,-168.6041 715.5822,-173.8785"/>
</g>
<!-- modm_architecture_memory -->
<g id="node10" class="node">
<title>modm_architecture_memory</title>
<g id="a_node10"><a xlink:href="../modm-architecture-memory" xlink:title="modm:&#10;architecture:&#10;memory">
<polygon fill="#d3d3d3" stroke="#000000" points="900,-231 816,-231 816,-178 900,-178 900,-231"/>
<text text-anchor="middle" x="858" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="858" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="858" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">memory</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_memory -->
<g id="edge9" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_memory</title>
<path fill="none" stroke="#000000" d="M688.11,-128.4021C717.4174,-139.715 760.9079,-157.0414 806.6681,-177.9834"/>
<polygon fill="#000000" stroke="#000000" points="805.3005,-181.207 815.8469,-182.2193 808.2337,-174.8512 805.3005,-181.207"/>
</g>
<!-- modm_architecture_unaligned -->
<g id="node11" class="node">
<title>modm_architecture_unaligned</title>
<g id="a_node11"><a xlink:href="../modm-architecture-unaligned" xlink:title="modm:&#10;architecture:&#10;unaligned">
<polygon fill="#d3d3d3" stroke="#000000" points="1002,-231 918,-231 918,-178 1002,-178 1002,-231"/>
<text text-anchor="middle" x="960" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="960" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="960" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">unaligned</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_unaligned -->
<g id="edge10" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_unaligned</title>
<path fill="none" stroke="#000000" d="M688.1886,-121.1908C736.0196,-129.7273 825.4643,-147.7878 908.2345,-178.0429"/>
<polygon fill="#000000" stroke="#000000" points="907.179,-181.3846 917.7718,-181.5954 909.6225,-174.8249 907.179,-181.3846"/>
</g>
<!-- modm_cmsis_device -->
<g id="node12" class="node">
<title>modm_cmsis_device</title>
<g id="a_node12"><a xlink:href="../modm-cmsis-device" xlink:title="modm:&#10;cmsis:&#10;device">
<polygon fill="#d3d3d3" stroke="#000000" points="1076,-231 1020,-231 1020,-178 1076,-178 1076,-231"/>
<text text-anchor="middle" x="1048" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1048" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">cmsis:</text>
<text text-anchor="middle" x="1048" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">device</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_cmsis_device -->
<g id="edge11" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_cmsis_device</title>
<path fill="none" stroke="#000000" d="M688.1298,-116.4641C751.6363,-119.2964 891.5372,-130.4488 1010.0704,-178.376"/>
<polygon fill="#000000" stroke="#000000" points="1009.0737,-181.7517 1019.6516,-182.3513 1011.7564,-175.2861 1009.0737,-181.7517"/>
</g>
<!-- modm_platform -->
<g id="node13" class="node">
<title>modm_platform</title>
<g id="a_node13"><a xlink:href="../modm-platform" xlink:title="modm:&#10;platform">
<polygon fill="#d3d3d3" stroke="#000000" points="1158,-223.5 1094,-223.5 1094,-185.5 1158,-185.5 1158,-223.5"/>
<text text-anchor="middle" x="1126" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1126" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">platform</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_platform -->
<g id="edge12" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M688.2342,-118.5608C776.8658,-126.7869 1011.9963,-150.6126 1085,-178 1086.9045,-178.7145 1088.8123,-179.5279 1090.7059,-180.4154"/>
<polygon fill="#000000" stroke="#000000" points="1089.5053,-183.7416 1099.9765,-185.3558 1092.7974,-177.564 1089.5053,-183.7416"/>
</g>
<!-- modm_platform_clock -->
<g id="node14" class="node">
<title>modm_platform_clock</title>
<g id="a_node14"><a xlink:href="../modm-platform-clock" xlink:title="modm:&#10;platform:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="1244,-231 1176,-231 1176,-178 1244,-178 1244,-231"/>
<text text-anchor="middle" x="1210" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1210" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1210" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_platform_clock -->
<g id="edge13" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_platform_clock</title>
<path fill="none" stroke="#000000" d="M688.0536,-117.7396C784.664,-124.3835 1059.8289,-145.3889 1166.1617,-178.4945"/>
<polygon fill="#000000" stroke="#000000" points="1165.3197,-181.9029 1175.9133,-181.7359 1167.5278,-175.2602 1165.3197,-181.9029"/>
</g>
<!-- modm_platform_core -->
<g id="node15" class="node">
<title>modm_platform_core</title>
<g id="a_node15"><a xlink:href="../modm-platform-core" xlink:title="modm:&#10;platform:&#10;core">
<polygon fill="#d3d3d3" stroke="#000000" points="688,-53 620,-53 620,0 688,0 688,-53"/>
<text text-anchor="middle" x="654" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="654" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="654" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">core</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_platform_cortex_m -->
<g id="edge14" class="edge">
<title>modm_platform_core&#45;&gt;modm_platform_cortex_m</title>
<path fill="none" stroke="#000000" d="M654,-53.2029C654,-61.2113 654,-70.1403 654,-78.6802"/>
<polygon fill="#000000" stroke="#000000" points="650.5001,-78.8159 654,-88.8159 657.5001,-78.8159 650.5001,-78.8159"/>
</g>
</g>
</svg>

