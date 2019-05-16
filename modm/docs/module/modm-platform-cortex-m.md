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
<!-- Generated by graphviz version 2.40.1 (0)
 -->
<!-- Title: modm:platform:cortex&#45;m Pages: 1 -->
<svg width="1424pt" height="239pt"
 viewBox="0.00 0.00 1423.50 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:platform:cortex&#45;m</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-235 1419.5,-235 1419.5,4 -4,4"/>
<!-- modm_platform_cortex_m -->
<g id="node1" class="node">
<title>modm_platform_cortex_m</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="800.5,-142 717.5,-142 717.5,-89 800.5,-89 800.5,-142"/>
<text text-anchor="middle" x="759" y="-126.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="759" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="759" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">cortex&#45;m</text>
</g>
<!-- modm_architecture_accessor -->
<g id="node2" class="node">
<title>modm_architecture_accessor</title>
<g id="a_node2"><a xlink:href="../modm-architecture-accessor" xlink:title="modm:&#10;architecture:&#10;accessor">
<polygon fill="#d3d3d3" stroke="#000000" points="110,-231 0,-231 0,-178 110,-178 110,-231"/>
<text text-anchor="middle" x="55" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="55" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="55" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">accessor</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_accessor -->
<g id="edge1" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_accessor</title>
<path fill="none" stroke="#000000" d="M717.218,-117.9669C597.0762,-125.3083 251.4267,-148.2315 119.992,-178.1383"/>
<polygon fill="#000000" stroke="#000000" points="119.0746,-174.759 110.1635,-180.4898 120.7035,-181.5668 119.0746,-174.759"/>
</g>
<!-- modm_architecture_assert -->
<g id="node3" class="node">
<title>modm_architecture_assert</title>
<g id="a_node3"><a xlink:href="../modm-architecture-assert" xlink:title="modm:&#10;architecture:&#10;assert">
<polygon fill="#d3d3d3" stroke="#000000" points="238,-231 128,-231 128,-178 238,-178 238,-231"/>
<text text-anchor="middle" x="183" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="183" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="183" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">assert</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_assert -->
<g id="edge2" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_assert</title>
<path fill="none" stroke="#000000" d="M717.4722,-116.7699C630.9665,-120.2227 425.436,-132.6396 247.9526,-178.0362"/>
<polygon fill="#000000" stroke="#000000" points="246.983,-174.6719 238.1855,-180.5756 248.7445,-181.4466 246.983,-174.6719"/>
</g>
<!-- modm_architecture_atomic -->
<g id="node4" class="node">
<title>modm_architecture_atomic</title>
<g id="a_node4"><a xlink:href="../modm-architecture-atomic" xlink:title="modm:&#10;architecture:&#10;atomic">
<polygon fill="#d3d3d3" stroke="#000000" points="366,-231 256,-231 256,-178 366,-178 366,-231"/>
<text text-anchor="middle" x="311" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="311" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="311" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">atomic</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_atomic -->
<g id="edge3" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_atomic</title>
<path fill="none" stroke="#000000" d="M717.3984,-119.0919C647.9938,-125.7668 504.3533,-142.4931 376.2065,-177.9105"/>
<polygon fill="#000000" stroke="#000000" points="374.9037,-174.641 366.2242,-180.7168 376.7982,-181.3798 374.9037,-174.641"/>
</g>
<!-- modm_architecture_clock -->
<g id="node5" class="node">
<title>modm_architecture_clock</title>
<g id="a_node5"><a xlink:href="../modm-architecture-clock" xlink:title="modm:&#10;architecture:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="494,-231 384,-231 384,-178 494,-178 494,-231"/>
<text text-anchor="middle" x="439" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="439" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="439" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_clock -->
<g id="edge4" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_clock</title>
<path fill="none" stroke="#000000" d="M717.2176,-123.6458C668.077,-133.6759 584.0315,-152.2351 504.0403,-177.8345"/>
<polygon fill="#000000" stroke="#000000" points="502.7402,-174.5765 494.308,-180.9911 504.9,-181.2351 502.7402,-174.5765"/>
</g>
<!-- modm_architecture_delay -->
<g id="node6" class="node">
<title>modm_architecture_delay</title>
<g id="a_node6"><a xlink:href="../modm-architecture-delay" xlink:title="modm:&#10;architecture:&#10;delay">
<polygon fill="#d3d3d3" stroke="#000000" points="622,-231 512,-231 512,-178 622,-178 622,-231"/>
<text text-anchor="middle" x="567" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="567" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="567" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">delay</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_delay -->
<g id="edge5" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_delay</title>
<path fill="none" stroke="#000000" d="M717.3389,-134.8117C692.2693,-146.4325 659.8281,-161.4703 631.5017,-174.6008"/>
<polygon fill="#000000" stroke="#000000" points="629.6704,-171.5918 622.0697,-178.9729 632.6144,-177.9427 629.6704,-171.5918"/>
</g>
<!-- modm_architecture_heap -->
<g id="node7" class="node">
<title>modm_architecture_heap</title>
<g id="a_node7"><a xlink:href="../modm-architecture-heap" xlink:title="modm:&#10;architecture:&#10;heap">
<polygon fill="#d3d3d3" stroke="#000000" points="750,-231 640,-231 640,-178 750,-178 750,-231"/>
<text text-anchor="middle" x="695" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="695" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="695" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">heap</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_heap -->
<g id="edge6" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_heap</title>
<path fill="none" stroke="#000000" d="M739.7979,-142.2029C733.5961,-150.8273 726.6265,-160.5194 720.0654,-169.6434"/>
<polygon fill="#000000" stroke="#000000" points="717.1853,-167.6537 714.1886,-177.8159 722.8684,-171.7405 717.1853,-167.6537"/>
</g>
<!-- modm_architecture_interrupt -->
<g id="node8" class="node">
<title>modm_architecture_interrupt</title>
<g id="a_node8"><a xlink:href="../modm-architecture-interrupt" xlink:title="modm:&#10;architecture:&#10;interrupt">
<polygon fill="#d3d3d3" stroke="#000000" points="878,-231 768,-231 768,-178 878,-178 878,-231"/>
<text text-anchor="middle" x="823" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="823" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="823" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">interrupt</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_interrupt -->
<g id="edge7" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_interrupt</title>
<path fill="none" stroke="#000000" d="M778.2021,-142.2029C784.4039,-150.8273 791.3735,-160.5194 797.9346,-169.6434"/>
<polygon fill="#000000" stroke="#000000" points="795.1316,-171.7405 803.8114,-177.8159 800.8147,-167.6537 795.1316,-171.7405"/>
</g>
<!-- modm_architecture_memory -->
<g id="node9" class="node">
<title>modm_architecture_memory</title>
<g id="a_node9"><a xlink:href="../modm-architecture-memory" xlink:title="modm:&#10;architecture:&#10;memory">
<polygon fill="#d3d3d3" stroke="#000000" points="1006,-231 896,-231 896,-178 1006,-178 1006,-231"/>
<text text-anchor="middle" x="951" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="951" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="951" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">memory</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_memory -->
<g id="edge8" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_memory</title>
<path fill="none" stroke="#000000" d="M800.6611,-134.8117C825.7307,-146.4325 858.1719,-161.4703 886.4983,-174.6008"/>
<polygon fill="#000000" stroke="#000000" points="885.3856,-177.9427 895.9303,-178.9729 888.3296,-171.5918 885.3856,-177.9427"/>
</g>
<!-- modm_architecture_unaligned -->
<g id="node10" class="node">
<title>modm_architecture_unaligned</title>
<g id="a_node10"><a xlink:href="../modm-architecture-unaligned" xlink:title="modm:&#10;architecture:&#10;unaligned">
<polygon fill="#d3d3d3" stroke="#000000" points="1134,-231 1024,-231 1024,-178 1134,-178 1134,-231"/>
<text text-anchor="middle" x="1079" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1079" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1079" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">unaligned</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_unaligned -->
<g id="edge9" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_unaligned</title>
<path fill="none" stroke="#000000" d="M800.7824,-123.6458C849.923,-133.6759 933.9685,-152.2351 1013.9597,-177.8345"/>
<polygon fill="#000000" stroke="#000000" points="1013.1,-181.2351 1023.692,-180.9911 1015.2598,-174.5765 1013.1,-181.2351"/>
</g>
<!-- modm_cmsis_device -->
<g id="node11" class="node">
<title>modm_cmsis_device</title>
<g id="a_node11"><a xlink:href="../modm-cmsis-device" xlink:title="modm:&#10;cmsis:&#10;device">
<polygon fill="#d3d3d3" stroke="#000000" points="1217.5,-231 1152.5,-231 1152.5,-178 1217.5,-178 1217.5,-231"/>
<text text-anchor="middle" x="1185" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1185" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">cmsis:</text>
<text text-anchor="middle" x="1185" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">device</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_cmsis_device -->
<g id="edge10" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_cmsis_device</title>
<path fill="none" stroke="#000000" d="M800.7523,-116.8651C872.8213,-120.3961 1024.2519,-133.017 1143,-178 1143.0957,-178.0363 1143.1915,-178.0728 1143.2873,-178.1095"/>
<polygon fill="#000000" stroke="#000000" points="1141.6111,-181.1933 1152.1668,-182.1032 1144.4824,-174.8093 1141.6111,-181.1933"/>
</g>
<!-- modm_platform -->
<g id="node12" class="node">
<title>modm_platform</title>
<g id="a_node12"><a xlink:href="../modm-platform" xlink:title="modm:&#10;platform">
<polygon fill="#d3d3d3" stroke="#000000" points="1314,-223.5 1236,-223.5 1236,-185.5 1314,-185.5 1314,-223.5"/>
<text text-anchor="middle" x="1275" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1275" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">platform</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_platform -->
<g id="edge11" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M800.7123,-119.0762C900.0052,-127.9088 1147.9862,-151.9383 1227,-178 1229.7544,-178.9085 1232.533,-179.9761 1235.2912,-181.1521"/>
<polygon fill="#000000" stroke="#000000" points="1233.9859,-184.4059 1244.5247,-185.4943 1236.9648,-178.0714 1233.9859,-184.4059"/>
</g>
<!-- modm_platform_clock -->
<g id="node13" class="node">
<title>modm_platform_clock</title>
<g id="a_node13"><a xlink:href="../modm-platform-clock" xlink:title="modm:&#10;platform:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="1415.5,-231 1332.5,-231 1332.5,-178 1415.5,-178 1415.5,-231"/>
<text text-anchor="middle" x="1374" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1374" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1374" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_platform_clock -->
<g id="edge12" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_platform_clock</title>
<path fill="none" stroke="#000000" d="M800.709,-118.1283C913.908,-125.5774 1224.9629,-148.2325 1323,-178 1323.0985,-178.0299 1323.197,-178.06 1323.2955,-178.0903"/>
<polygon fill="#000000" stroke="#000000" points="1321.9017,-181.3087 1332.4963,-181.3853 1324.2619,-174.7186 1321.9017,-181.3087"/>
</g>
<!-- modm_platform_core -->
<g id="node14" class="node">
<title>modm_platform_core</title>
<g id="a_node14"><a xlink:href="../modm-platform-core" xlink:title="modm:&#10;platform:&#10;core">
<polygon fill="#d3d3d3" stroke="#000000" points="800.5,-53 717.5,-53 717.5,0 800.5,0 800.5,-53"/>
<text text-anchor="middle" x="759" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="759" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="759" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">core</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_platform_cortex_m -->
<g id="edge13" class="edge">
<title>modm_platform_core&#45;&gt;modm_platform_cortex_m</title>
<path fill="none" stroke="#000000" d="M759,-53.2029C759,-61.2113 759,-70.1403 759,-78.6802"/>
<polygon fill="#000000" stroke="#000000" points="755.5001,-78.8159 759,-88.8159 762.5001,-78.8159 755.5001,-78.8159"/>
</g>
</g>
</svg>

