!!! warning "These module docs are in beta and may be incomplete."

# modm:debug: Logging

The `modm::log::Logger` uses a `modm::IOStream` to read messages and write them
to a given output device.

Write a log message with streams:

```cpp
MODM_LOG_DEBUG << "i=" << i << ", y=" << y << modm::flush;
```

The name of the stream represents the log level of the message. Available are:

- `MODM_LOG_DEBUG`
- `MODM_LOG_INFO`
- `MODM_LOG_WARNING`
- `MODM_LOG_ERROR`

A log message can also be generated separately:

```cpp
int a[3] = {1,2,3};
MODM_LOG_DEBUG << "a = ";
for(int i = 0; i < sizeof(a); ++i) {
	MODM_LOG_DEBUG << a[i] << ", ";
}
MODM_LOG_DEBUG << modm::flush;
```

### Flow of a call

This is to give an estimation how many resources a call of the logger use.
All considerations are for a PC program.

The given call is:

```cpp
MODM_LOG_DEBUG << 123456;
```

The macro resolves to:

```cpp
if (MODM_LOG_LEVEL <= modm::log::DEBUG)
    modm::log::debug
```

- `modm::log::debug` is an instance of `modm::Logger`:
   Call of `modm::Logger::operator << (T)` (with `T = int32_t`) which is inline
   - `IOStream::operator << (T)` (with `T = int32_t`) is inline
   - `IOStream::writeInteger(int32_t)`
- `IOStream::writeInteger(int32_t)` will create the formatted string
- virtual call of `IODevice::write(const char*)`
- redirect to `std::cout`

In sum there are two nested method calls with one of them being virtual.

## Content

```cpp
// Class
class modm::ErrorReport;
class modm::log::DefaultStyle;
class modm::log::Logger;
class modm::log::Prefix< typename T , typename STYLE =DefaultStyle >;
class modm::log::StdColour< Colour TEXT, Colour BACKGROUND, typename STYLE =DefaultStyle >;
class modm::log::Style< typename STYLE =DefaultStyle >;
class modm::log::StyleWrapper< typename STYLE >;
// Enum
enum Colour;
enum Level;
// Variable
Logger modm::log::debug ;
Logger modm::log::error ;
Logger modm::log::info ;
Logger modm::log::warning ;
// Define
#define FILENAME
#define MODM_FILE_INFO
#define MODM_LOG_DEBUG
#define MODM_LOG_ERROR
#define MODM_LOG_INFO
#define MODM_LOG_LEVEL
#define MODM_LOG_OFF
#define MODM_LOG_WARNING
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:debug Pages: 1 -->
<svg width="236pt" height="209pt"
 viewBox="0.00 0.00 236.00 209.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 205)">
<title>modm:debug</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-205 232,-205 232,4 -4,4"/>
<!-- modm_debug -->
<g id="node1" class="node">
<title>modm_debug</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="154,-127 98,-127 98,-89 154,-89 154,-127"/>
<text text-anchor="middle" x="126" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="126" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">debug</text>
</g>
<!-- modm_architecture -->
<g id="node2" class="node">
<title>modm_architecture</title>
<g id="a_node2"><a xlink:href="../modm-architecture" xlink:title="modm:&#10;architecture">
<polygon fill="#d3d3d3" stroke="#000000" points="80,-201 0,-201 0,-163 80,-163 80,-201"/>
<text text-anchor="middle" x="40" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="40" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture</text>
</a>
</g>
</g>
<!-- modm_debug&#45;&gt;modm_architecture -->
<g id="edge1" class="edge">
<title>modm_debug&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M103.8529,-127.0568C93.6007,-135.8785 81.2618,-146.4957 70.1749,-156.0355"/>
<polygon fill="#000000" stroke="#000000" points="67.6683,-153.575 62.371,-162.7505 72.234,-158.8811 67.6683,-153.575"/>
</g>
<!-- modm_io -->
<g id="node3" class="node">
<title>modm_io</title>
<g id="a_node3"><a xlink:href="../modm-io" xlink:title="modm:&#10;io">
<polygon fill="#d3d3d3" stroke="#000000" points="154,-201 98,-201 98,-163 154,-163 154,-201"/>
<text text-anchor="middle" x="126" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="126" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">io</text>
</a>
</g>
</g>
<!-- modm_debug&#45;&gt;modm_io -->
<g id="edge2" class="edge">
<title>modm_debug&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M126,-127.0568C126,-134.8507 126,-144.0462 126,-152.6619"/>
<polygon fill="#000000" stroke="#000000" points="122.5001,-152.7505 126,-162.7505 129.5001,-152.7506 122.5001,-152.7505"/>
</g>
<!-- modm_utils -->
<g id="node4" class="node">
<title>modm_utils</title>
<g id="a_node4"><a xlink:href="../modm-utils" xlink:title="modm:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="228,-201 172,-201 172,-163 228,-163 228,-201"/>
<text text-anchor="middle" x="200" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="200" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_debug&#45;&gt;modm_utils -->
<g id="edge3" class="edge">
<title>modm_debug&#45;&gt;modm_utils</title>
<path fill="none" stroke="#000000" d="M145.0568,-127.0568C153.7072,-135.7072 164.0841,-146.0841 173.4786,-155.4786"/>
<polygon fill="#000000" stroke="#000000" points="171.2046,-158.1543 180.7505,-162.7505 176.1543,-153.2046 171.2046,-158.1543"/>
</g>
<!-- modm_driver_bme280 -->
<g id="node5" class="node">
<title>modm_driver_bme280</title>
<g id="a_node5"><a xlink:href="../modm-driver-bme280" xlink:title="modm:&#10;driver:&#10;bme280">
<polygon fill="#d3d3d3" stroke="#000000" points="115.5,-53 54.5,-53 54.5,0 115.5,0 115.5,-53"/>
<text text-anchor="middle" x="85" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="85" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">driver:</text>
<text text-anchor="middle" x="85" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">bme280</text>
</a>
</g>
</g>
<!-- modm_driver_bme280&#45;&gt;modm_debug -->
<g id="edge4" class="edge">
<title>modm_driver_bme280&#45;&gt;modm_debug</title>
<path fill="none" stroke="#000000" d="M98.4249,-53.1861C102.7126,-61.7092 107.4709,-71.1679 111.8111,-79.7952"/>
<polygon fill="#000000" stroke="#000000" points="108.6937,-81.3866 116.3144,-88.7469 114.947,-78.2407 108.6937,-81.3866"/>
</g>
<!-- modm_platform_can -->
<g id="node6" class="node">
<title>modm_platform_can</title>
<g id="a_node6"><a xlink:href="../modm-platform-can" xlink:title="modm:&#10;platform:&#10;can">
<polygon fill="#d3d3d3" stroke="#000000" points="202,-53 134,-53 134,0 202,0 202,-53"/>
<text text-anchor="middle" x="168" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="168" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="168" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">can</text>
</a>
</g>
</g>
<!-- modm_platform_can&#45;&gt;modm_debug -->
<g id="edge5" class="edge">
<title>modm_platform_can&#45;&gt;modm_debug</title>
<path fill="none" stroke="#000000" d="M154.2476,-53.1861C149.8554,-61.7092 144.981,-71.1679 140.535,-79.7952"/>
<polygon fill="#000000" stroke="#000000" points="137.3916,-78.2545 135.9218,-88.7469 143.6139,-81.4612 137.3916,-78.2545"/>
</g>
</g>
</svg>
