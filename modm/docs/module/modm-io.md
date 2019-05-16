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
enum class IOBuffer;
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (0)
 -->
<!-- Title: modm:io Pages: 1 -->
<svg width="864pt" height="224pt"
 viewBox="0.00 0.00 863.50 224.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 220)">
<title>modm:io</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-220 859.5,-220 859.5,4 -4,4"/>
<!-- modm_io -->
<g id="node1" class="node">
<title>modm_io</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="505.5,-127 440.5,-127 440.5,-89 505.5,-89 505.5,-127"/>
<text text-anchor="middle" x="473" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="473" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">io</text>
</g>
<!-- modm_architecture_accessor -->
<g id="node2" class="node">
<title>modm_architecture_accessor</title>
<g id="a_node2"><a xlink:href="../modm-architecture-accessor" xlink:title="modm:&#10;architecture:&#10;accessor">
<polygon fill="#d3d3d3" stroke="#000000" points="528,-216 418,-216 418,-163 528,-163 528,-216"/>
<text text-anchor="middle" x="473" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="473" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="473" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">accessor</text>
</a>
</g>
</g>
<!-- modm_io&#45;&gt;modm_architecture_accessor -->
<g id="edge1" class="edge">
<title>modm_io&#45;&gt;modm_architecture_accessor</title>
<path fill="none" stroke="#000000" d="M467.1601,-127.3144C466.4434,-134.9606 466.15,-144.0143 466.2799,-152.8129"/>
<polygon fill="#000000" stroke="#000000" points="462.7859,-153.0508 466.6269,-162.9249 469.7818,-152.8107 462.7859,-153.0508"/>
</g>
<!-- modm_math_utils -->
<g id="node3" class="node">
<title>modm_math_utils</title>
<g id="a_node3"><a xlink:href="../modm-math-utils" xlink:title="modm:&#10;math:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="611.5,-216 546.5,-216 546.5,-163 611.5,-163 611.5,-216"/>
<text text-anchor="middle" x="579" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="579" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="579" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_io&#45;&gt;modm_math_utils -->
<g id="edge2" class="edge">
<title>modm_io&#45;&gt;modm_math_utils</title>
<path fill="none" stroke="#000000" d="M497.8523,-127.1081C509.8115,-136.3032 524.4644,-147.5694 538.012,-157.9857"/>
<polygon fill="#000000" stroke="#000000" points="536.2095,-161.0147 546.2705,-164.3354 540.4763,-155.4654 536.2095,-161.0147"/>
</g>
<!-- modm_architecture_accessor&#45;&gt;modm_io -->
<g id="edge3" class="edge">
<title>modm_architecture_accessor&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M479.3731,-162.9249C479.8243,-154.762 479.8704,-145.7322 479.5114,-137.3922"/>
<polygon fill="#000000" stroke="#000000" points="482.997,-137.0595 478.8399,-127.3144 476.0125,-137.5249 482.997,-137.0595"/>
</g>
<!-- modm_architecture_can -->
<g id="node4" class="node">
<title>modm_architecture_can</title>
<g id="a_node4"><a xlink:href="../modm-architecture-can" xlink:title="modm:&#10;architecture:&#10;can">
<polygon fill="#d3d3d3" stroke="#000000" points="110,-53 0,-53 0,0 110,0 110,-53"/>
<text text-anchor="middle" x="55" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="55" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="55" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">can</text>
</a>
</g>
</g>
<!-- modm_architecture_can&#45;&gt;modm_io -->
<g id="edge4" class="edge">
<title>modm_architecture_can&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M110.217,-50.3086C113.1653,-51.2825 116.1051,-52.1884 119,-53 228.7115,-83.7599 362.6877,-98.6612 430.015,-104.6488"/>
<polygon fill="#000000" stroke="#000000" points="429.9696,-108.1579 440.2345,-105.5343 430.574,-101.184 429.9696,-108.1579"/>
</g>
<!-- modm_architecture_i2c -->
<g id="node5" class="node">
<title>modm_architecture_i2c</title>
<g id="a_node5"><a xlink:href="../modm-architecture-i2c" xlink:title="modm:&#10;architecture:&#10;i2c">
<polygon fill="#d3d3d3" stroke="#000000" points="238,-53 128,-53 128,0 238,0 238,-53"/>
<text text-anchor="middle" x="183" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="183" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="183" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c&#45;&gt;modm_io -->
<g id="edge5" class="edge">
<title>modm_architecture_i2c&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M238.3071,-50.0114C241.2319,-51.063 244.142,-52.0667 247,-53 309.8096,-73.5114 384.6552,-90.2716 430.4916,-99.6963"/>
<polygon fill="#000000" stroke="#000000" points="429.8789,-103.1433 440.3759,-101.7073 431.2745,-96.2838 429.8789,-103.1433"/>
</g>
<!-- modm_architecture_register -->
<g id="node6" class="node">
<title>modm_architecture_register</title>
<g id="a_node6"><a xlink:href="../modm-architecture-register" xlink:title="modm:&#10;architecture:&#10;register">
<polygon fill="#d3d3d3" stroke="#000000" points="366,-53 256,-53 256,0 366,0 366,-53"/>
<text text-anchor="middle" x="311" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="311" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="311" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">register</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_io -->
<g id="edge6" class="edge">
<title>modm_architecture_register&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M364.0449,-53.1861C385.8108,-64.1363 410.6463,-76.6307 431.1036,-86.9225"/>
<polygon fill="#000000" stroke="#000000" points="429.6619,-90.1151 440.1681,-91.4827 432.8079,-83.8619 429.6619,-90.1151"/>
</g>
<!-- modm_container -->
<g id="node7" class="node">
<title>modm_container</title>
<g id="a_node7"><a xlink:href="../modm-container" xlink:title="modm:&#10;container">
<polygon fill="#d3d3d3" stroke="#000000" points="469.5,-45.5 384.5,-45.5 384.5,-7.5 469.5,-7.5 469.5,-45.5"/>
<text text-anchor="middle" x="427" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="427" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">container</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_io -->
<g id="edge7" class="edge">
<title>modm_container&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M437.9014,-45.8144C443.6301,-55.9642 450.7587,-68.5943 457.0921,-79.8153"/>
<polygon fill="#000000" stroke="#000000" points="454.2135,-81.836 462.1769,-88.8243 460.3096,-78.3953 454.2135,-81.836"/>
</g>
<!-- modm_debug -->
<g id="node8" class="node">
<title>modm_debug</title>
<g id="a_node8"><a xlink:href="../modm-debug" xlink:title="modm:&#10;debug">
<polygon fill="#d3d3d3" stroke="#000000" points="552.5,-45.5 487.5,-45.5 487.5,-7.5 552.5,-7.5 552.5,-45.5"/>
<text text-anchor="middle" x="520" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="520" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">debug</text>
</a>
</g>
</g>
<!-- modm_debug&#45;&gt;modm_io -->
<g id="edge8" class="edge">
<title>modm_debug&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M508.8617,-45.8144C502.9499,-56.0657 495.579,-68.8471 489.0598,-80.1515"/>
<polygon fill="#000000" stroke="#000000" points="486.0222,-78.413 484.0584,-88.8243 492.0861,-81.91 486.0222,-78.413"/>
</g>
<!-- modm_math_geometry -->
<g id="node9" class="node">
<title>modm_math_geometry</title>
<g id="a_node9"><a xlink:href="../modm-math-geometry" xlink:title="modm:&#10;math:&#10;geometry">
<polygon fill="#d3d3d3" stroke="#000000" points="655.5,-53 570.5,-53 570.5,0 655.5,0 655.5,-53"/>
<text text-anchor="middle" x="613" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="613" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="613" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">geometry</text>
</a>
</g>
</g>
<!-- modm_math_geometry&#45;&gt;modm_io -->
<g id="edge9" class="edge">
<title>modm_math_geometry&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M570.2347,-51.3955C552.5677,-61.6802 532.1586,-73.5612 514.6139,-83.7748"/>
<polygon fill="#000000" stroke="#000000" points="512.6113,-80.8907 505.7299,-88.9465 516.133,-86.9403 512.6113,-80.8907"/>
</g>
<!-- modm_math_matrix -->
<g id="node10" class="node">
<title>modm_math_matrix</title>
<g id="a_node10"><a xlink:href="../modm-math-matrix" xlink:title="modm:&#10;math:&#10;matrix">
<polygon fill="#d3d3d3" stroke="#000000" points="738.5,-53 673.5,-53 673.5,0 738.5,0 738.5,-53"/>
<text text-anchor="middle" x="706" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="706" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="706" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">matrix</text>
</a>
</g>
</g>
<!-- modm_math_matrix&#45;&gt;modm_io -->
<g id="edge10" class="edge">
<title>modm_math_matrix&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M673.3591,-48.7793C670.5854,-50.307 667.778,-51.7381 665,-53 615.7748,-75.3612 555.2847,-90.8869 515.5144,-99.6082"/>
<polygon fill="#000000" stroke="#000000" points="514.6638,-96.2109 505.6206,-101.731 516.1324,-103.0552 514.6638,-96.2109"/>
</g>
<!-- modm_processing_timer -->
<g id="node11" class="node">
<title>modm_processing_timer</title>
<g id="a_node11"><a xlink:href="../modm-processing-timer" xlink:title="modm:&#10;processing:&#10;timer">
<polygon fill="#d3d3d3" stroke="#000000" points="855.5,-53 756.5,-53 756.5,0 855.5,0 855.5,-53"/>
<text text-anchor="middle" x="806" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="806" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="806" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">timer</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_io -->
<g id="edge11" class="edge">
<title>modm_processing_timer&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M756.3049,-50.0894C753.5191,-51.1342 750.7385,-52.1137 748,-53 667.926,-78.9137 570.606,-94.9085 515.839,-102.5644"/>
<polygon fill="#000000" stroke="#000000" points="515.2281,-99.1154 505.7962,-103.9412 516.1789,-106.0505 515.2281,-99.1154"/>
</g>
</g>
</svg>

