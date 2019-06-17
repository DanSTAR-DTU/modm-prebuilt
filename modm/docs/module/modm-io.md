!!! warning "These module docs are in beta and may be incomplete."

# modm:io: Input/Output Streams

The `modm::IOStream` class contains efficient formatting that supports both
C++ `std::basic_ostream`-like formatting via `operator <<` as well as
implementing a reasonably large subset of `printf`.

```cpp
modm::IOStream stream(device);
stream << "format number 8: " << uint8_t(8) << " or as signed -100: " << int8_t(-100);
stream << modm::endl;

stream.printf("format number 8: %u or as signed -100: %d", 8, -100);
```

!!! warning "AVR supported features"
    All expensive features incl. printf are disabled by default! Check the options.

!!! note "`modm::endl` does implicitly flush!"
    Flushing is *extremely expensive* on embedded systems, therefore `modm::endl`
    does not implicitly flush the stream. Please call `modm::flush` explicitly.


## Using printf

This module uses the printf implementation from [`mpaland/printf`](https://github.com/mpaland/printf).

### Format Specifiers

A format specifier follows this prototype: `%[flags][width][.precision][length]type`
The following format specifiers are supported:


#### Supported Types

| Type   | Output |
|--------|--------|
| d or i | Signed decimal integer |
| u      | Unsigned decimal integer |
| b      | Unsigned binary |
| o      | Unsigned octal |
| x      | Unsigned hexadecimal integer (lowercase) |
| X      | Unsigned hexadecimal integer (uppercase) |
| f or F | Decimal floating point (`with_float` option) |
| e or E | Scientific-notation (exponential) floating point (`with_float` option) |
| g or G | Scientific or decimal floating point (`with_float` option) |
| c      | Single character |
| s      | String of characters |
| p      | Pointer address |
| %      | A % followed by another % character will write a single % |


#### Supported Flags

| Flags | Description |
|-------|-------------|
| -     | Left-justify within the given field width; Right justification is the default. |
| +     | Forces to precede the result with a plus or minus sign (+ or -) even for positive numbers.<br>By default, only negative numbers are preceded with a - sign. |
| (space) | If no sign is going to be written, a blank space is inserted before the value. |
| #     | Used with o, b, x or X specifiers the value is preceded with 0, 0b, 0x or 0X respectively for values different than zero.<br>Used with f, F it forces the written output to contain a decimal point even if no more digits follow. By default, if no digits follow, no decimal point is written. |
| 0     | Left-pads the number with zeros (0) instead of spaces when padding is specified (see width sub-specifier). |


#### Supported Width

| Width    | Description |
|----------|-------------|
| (number) | Minimum number of characters to be printed. If the value to be printed is shorter than this number, the result is padded with blank spaces. The value is not truncated even if the result is larger. |
| *        | The width is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted. |


#### Supported Precision

| Precision | Description |
|-----------|-------------|
| .number   | For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. If the value to be written is shorter than this number, the result is padded with leading zeros. The value is not truncated even if the result is longer. A precision of 0 means that no character is written for the value 0.<br>For f and F specifiers: this is the number of digits to be printed after the decimal point. **By default, this is 6, maximum is 9**.<br>For s: this is the maximum number of characters to be printed. By default all characters are printed until the ending null character is encountered.<br>If the period is specified without an explicit value for precision, 0 is assumed. |
| .*        | The precision is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted. |


#### Supported Length

The length sub-specifier modifies the length of the data type.

| Length | d i  | u o x X |
|--------|------|---------|
| (none) | int  | unsigned int |
| hh     | char | unsigned char |
| h      | short int | unsigned short int |
| l      | long int | unsigned long int |
| ll     | long long int | unsigned long long int (`with_long_long` option) |
| j      | intmax_t | uintmax_t |
| z      | size_t | size_t |
| t      | ptrdiff_t | ptrdiff_t (`with_ptrdiff` option) |


## Redirecting IOStreams

The `modm::IODeviceWrapper` transforms any peripheral device that provides static
`write()` and `read()` functions into an `IODevice`.

You have to decide what happens when the device buffer is full and you cannot
write to it at the moment. There are two options:

1. busy wait until the buffer is free, or
2. discard the bytes that cannot be written.

Option 1 has the advantage, that none of your data will be lost,
however, busy-waiting can take a long time and can mess up your
program timings.
There is also a **high risk of deadlock**, when writing to a
IODevice inside of an interrupt and then busy-waiting forever
because the IODevice requires interrupts itself to send out
the data.

It is therefore highly recommended to use option 2, where surplus
data will be discarded.
You should increase the IODevice buffer size, if you experience
missing data from your connection.
This behavior is also deadlock safe when called from inside another
interrupt, and your program timing is minimally affected (essentially
only coping data into the buffer).

There is no default template argument, so that you hopefully make
a conscious decision and be aware of this behavior.

Example:

```cpp
// configure a UART
using Uart = Uart0;

// wrap it into an IODevice
modm::IODeviceWrapper<Uart, modm::IOBuffer::DiscardIfFull> device;

// use this device to print a message
device.write("Hello");

// or create a IOStream and use the stream to print something
modm::IOStream stream(device);
stream << " World!";
```

## Options
#### with_long_long

Support for 64-bit integer formatting: `True` ∈ `{ True, False }`


#### with_float

Support for floating point formatting: `True` ∈ `{ True, False }`

On AVRs floating point values can be printed, however, the formatting cannot be
specified and all values are printed as *scientific-notation exponential floating point*
#### with_ptrdiff

Support for pointer difference formatting: `True` ∈ `{ True, False }`


#### with_printf

Support for printf-style formatting: `True` ∈ `{ True, False }`


## Content

```cpp
// Class
class modm::IODevice;
class modm::IODeviceWrapper< class Device, IOBuffer behavior >;
class modm::IOStream;
// Function
IOStream& modm::ascii(IOStream &ios);
IOStream& modm::bin(IOStream &ios);
IOStream& modm::black(IOStream &ios);
IOStream& modm::blue(IOStream &ios);
IOStream& modm::cyan(IOStream &ios);
IOStream& modm::endl(IOStream &ios);
IOStream& modm::flush(IOStream &ios);
IOStream& modm::green(IOStream &ios);
IOStream& modm::hex(IOStream &ios);
IOStream& modm::magenta(IOStream &ios);
IOStream& modm::red(IOStream &ios);
IOStream& modm::white(IOStream &ios);
IOStream& modm::yellow(IOStream &ios);
// Enum
enum IOBuffer;
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:io Pages: 1 -->
<svg width="712pt" height="224pt"
 viewBox="0.00 0.00 712.00 224.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 220)">
<title>modm:io</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-220 708,-220 708,4 -4,4"/>
<!-- modm_io -->
<g id="node1" class="node">
<title>modm_io</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="408,-127 352,-127 352,-89 408,-89 408,-127"/>
<text text-anchor="middle" x="380" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="380" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">io</text>
</g>
<!-- modm_architecture_accessor -->
<g id="node2" class="node">
<title>modm_architecture_accessor</title>
<g id="a_node2"><a xlink:href="../modm-architecture-accessor" xlink:title="modm:&#10;architecture:&#10;accessor">
<polygon fill="#d3d3d3" stroke="#000000" points="422,-216 338,-216 338,-163 422,-163 422,-216"/>
<text text-anchor="middle" x="380" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="380" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="380" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">accessor</text>
</a>
</g>
</g>
<!-- modm_io&#45;&gt;modm_architecture_accessor -->
<g id="edge1" class="edge">
<title>modm_io&#45;&gt;modm_architecture_accessor</title>
<path fill="none" stroke="#000000" d="M374.1601,-127.3144C373.4434,-134.9606 373.15,-144.0143 373.2799,-152.8129"/>
<polygon fill="#000000" stroke="#000000" points="369.7859,-153.0508 373.6269,-162.9249 376.7818,-152.8107 369.7859,-153.0508"/>
</g>
<!-- modm_math_utils -->
<g id="node3" class="node">
<title>modm_math_utils</title>
<g id="a_node3"><a xlink:href="../modm-math-utils" xlink:title="modm:&#10;math:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="496,-216 440,-216 440,-163 496,-163 496,-216"/>
<text text-anchor="middle" x="468" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="468" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="468" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_io&#45;&gt;modm_math_utils -->
<g id="edge2" class="edge">
<title>modm_io&#45;&gt;modm_math_utils</title>
<path fill="none" stroke="#000000" d="M400.8548,-127.3144C410.2615,-136.0263 421.6413,-146.5655 432.3385,-156.4726"/>
<polygon fill="#000000" stroke="#000000" points="430.2541,-159.3126 439.9692,-163.5396 435.0105,-154.1768 430.2541,-159.3126"/>
</g>
<!-- modm_architecture_accessor&#45;&gt;modm_io -->
<g id="edge3" class="edge">
<title>modm_architecture_accessor&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M386.3731,-162.9249C386.8243,-154.762 386.8704,-145.7322 386.5114,-137.3922"/>
<polygon fill="#000000" stroke="#000000" points="389.997,-137.0595 385.8399,-127.3144 383.0125,-137.5249 389.997,-137.0595"/>
</g>
<!-- modm_architecture_can -->
<g id="node4" class="node">
<title>modm_architecture_can</title>
<g id="a_node4"><a xlink:href="../modm-architecture-can" xlink:title="modm:&#10;architecture:&#10;can">
<polygon fill="#d3d3d3" stroke="#000000" points="84,-53 0,-53 0,0 84,0 84,-53"/>
<text text-anchor="middle" x="42" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="42" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="42" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">can</text>
</a>
</g>
</g>
<!-- modm_architecture_can&#45;&gt;modm_io -->
<g id="edge4" class="edge">
<title>modm_architecture_can&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M84.1661,-49.5765C87.1158,-50.8275 90.078,-51.9852 93,-53 178.9943,-82.8667 285.4383,-97.8859 341.4167,-104.1983"/>
<polygon fill="#000000" stroke="#000000" points="341.2707,-107.7032 351.5921,-105.3119 342.0323,-100.7447 341.2707,-107.7032"/>
</g>
<!-- modm_architecture_i2c -->
<g id="node5" class="node">
<title>modm_architecture_i2c</title>
<g id="a_node5"><a xlink:href="../modm-architecture-i2c" xlink:title="modm:&#10;architecture:&#10;i2c">
<polygon fill="#d3d3d3" stroke="#000000" points="186,-53 102,-53 102,0 186,0 186,-53"/>
<text text-anchor="middle" x="144" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="144" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="144" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c&#45;&gt;modm_io -->
<g id="edge5" class="edge">
<title>modm_architecture_i2c&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M186.009,-49.1132C189.0225,-50.4921 192.0397,-51.8035 195,-53 244.4095,-72.9697 303.8323,-89.3551 341.7549,-98.9019"/>
<polygon fill="#000000" stroke="#000000" points="341.3059,-102.3967 351.8548,-101.4106 342.9934,-95.6031 341.3059,-102.3967"/>
</g>
<!-- modm_architecture_register -->
<g id="node6" class="node">
<title>modm_architecture_register</title>
<g id="a_node6"><a xlink:href="../modm-architecture-register" xlink:title="modm:&#10;architecture:&#10;register">
<polygon fill="#d3d3d3" stroke="#000000" points="288,-53 204,-53 204,0 288,0 288,-53"/>
<text text-anchor="middle" x="246" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="246" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="246" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">register</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_io -->
<g id="edge6" class="edge">
<title>modm_architecture_register&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M288.0308,-52.0635C305.6781,-62.7968 326.0223,-75.1703 343.0825,-85.5465"/>
<polygon fill="#000000" stroke="#000000" points="341.3103,-88.5651 351.6729,-90.7712 344.9478,-82.5844 341.3103,-88.5651"/>
</g>
<!-- modm_container -->
<g id="node7" class="node">
<title>modm_container</title>
<g id="a_node7"><a xlink:href="../modm-container" xlink:title="modm:&#10;container">
<polygon fill="#d3d3d3" stroke="#000000" points="373.5,-45.5 306.5,-45.5 306.5,-7.5 373.5,-7.5 373.5,-45.5"/>
<text text-anchor="middle" x="340" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="340" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">container</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_io -->
<g id="edge7" class="edge">
<title>modm_container&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M349.4794,-45.8144C354.4609,-55.9642 360.6598,-68.5943 366.167,-79.8153"/>
<polygon fill="#000000" stroke="#000000" points="363.0406,-81.3893 370.5886,-88.8243 369.3246,-78.3051 363.0406,-81.3893"/>
</g>
<!-- modm_debug -->
<g id="node8" class="node">
<title>modm_debug</title>
<g id="a_node8"><a xlink:href="../modm-debug" xlink:title="modm:&#10;debug">
<polygon fill="#d3d3d3" stroke="#000000" points="448,-45.5 392,-45.5 392,-7.5 448,-7.5 448,-45.5"/>
<text text-anchor="middle" x="420" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="420" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">debug</text>
</a>
</g>
</g>
<!-- modm_debug&#45;&gt;modm_io -->
<g id="edge8" class="edge">
<title>modm_debug&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M410.5206,-45.8144C405.5391,-55.9642 399.3402,-68.5943 393.833,-79.8153"/>
<polygon fill="#000000" stroke="#000000" points="390.6754,-78.3051 389.4114,-88.8243 396.9594,-81.3893 390.6754,-78.3051"/>
</g>
<!-- modm_math_geometry -->
<g id="node9" class="node">
<title>modm_math_geometry</title>
<g id="a_node9"><a xlink:href="../modm-math-geometry" xlink:title="modm:&#10;math:&#10;geometry">
<polygon fill="#d3d3d3" stroke="#000000" points="534,-53 466,-53 466,0 534,0 534,-53"/>
<text text-anchor="middle" x="500" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="500" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="500" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">geometry</text>
</a>
</g>
</g>
<!-- modm_math_geometry&#45;&gt;modm_io -->
<g id="edge9" class="edge">
<title>modm_math_geometry&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M465.9338,-49.6366C450.4991,-60.1194 432.2292,-72.5277 416.5125,-83.2019"/>
<polygon fill="#000000" stroke="#000000" points="414.3839,-80.4167 408.0778,-88.9305 418.3168,-86.2074 414.3839,-80.4167"/>
</g>
<!-- modm_math_matrix -->
<g id="node10" class="node">
<title>modm_math_matrix</title>
<g id="a_node10"><a xlink:href="../modm-math-matrix" xlink:title="modm:&#10;math:&#10;matrix">
<polygon fill="#d3d3d3" stroke="#000000" points="608,-53 552,-53 552,0 608,0 608,-53"/>
<text text-anchor="middle" x="580" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="580" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="580" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">matrix</text>
</a>
</g>
</g>
<!-- modm_math_matrix&#45;&gt;modm_io -->
<g id="edge10" class="edge">
<title>modm_math_matrix&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M551.8525,-47.922C548.9337,-49.7569 545.9534,-51.4868 543,-53 502.2903,-73.8575 451.9496,-89.4441 418.0728,-98.6012"/>
<polygon fill="#000000" stroke="#000000" points="416.8888,-95.2939 408.1167,-101.2354 418.6793,-102.0611 416.8888,-95.2939"/>
</g>
<!-- modm_processing_timer -->
<g id="node11" class="node">
<title>modm_processing_timer</title>
<g id="a_node11"><a xlink:href="../modm-processing-timer" xlink:title="modm:&#10;processing:&#10;timer">
<polygon fill="#d3d3d3" stroke="#000000" points="704,-53 626,-53 626,0 704,0 704,-53"/>
<text text-anchor="middle" x="665" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="665" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="665" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">timer</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_io -->
<g id="edge11" class="edge">
<title>modm_processing_timer&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M625.8638,-49.2318C622.9073,-50.5999 619.9329,-51.8751 617,-53 549.2109,-78.9998 465.608,-94.8807 417.9883,-102.5096"/>
<polygon fill="#000000" stroke="#000000" points="417.446,-99.0518 408.1077,-104.0564 418.5287,-105.9676 417.446,-99.0518"/>
</g>
</g>
</svg>

