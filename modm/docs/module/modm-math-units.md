!!! warning "These module docs are in beta and may be incomplete."

# modm:math:units: SI Units

modm uses a couple of common SI units for configuration of peripherals:

- `frequency_t` in Hertz: `Hz`, `kHz` and `MHz`.
- `baudrate_t` in Baud: `Bd`, `kBd`, `MBd`.
- `bitrate_t` in bit/s, `bps`.

These are integral units, so 1 Hz/Bd/bps cannot be split further, and are cast
directly to `uint32_t` type, so they can be used as a non-type template argument.

Conversion can be done via constexpr functions from *any* numerical type:

- `modm::Hz(T value)`, `modm::kHz(T value)`, `modm::MHz(T value)`.
- `modm::Bd(T value)`, `modm::kBd(T value)`, `modm::MBd(T value)`.
- `modm::bps(T value)`, `modm::kbps(T value)`, `modm::Mbps(T value)`.

In addition, user-defined literals are provided in the `modm::literals` namespace:

```cpp
using namespace modm::literals;

frequency_t frequency = modm::Mhz(10.5);
            frequency = 10.5_MHz;
baudrate_t baudrate = 115.2_kBd;
           baudrate = modm::kBd(115.2);
bitrate_t bitrate = modm::kbps(125);
          bitrate = 125_kbps;

frequency = 4295_MHz; // OVERFLOW at 2^32 units!
```

## Integral Percentages

Since `float` cannot be used as a non-type template argument, an integer type
for providing tolerances in `percent_t` is available.
Note that `percent_t` is implemented as an enum class, which prevents implicit
conversions, since the base for this is not 1.
You must therefore use the `modm::pct(T value)` or `_pct` constructors.

```cpp
using namespace modm::literals;

percent_t tolerance = modm::pct(10);
          tolerance = 10_pct;

// convert back to float. *internal use only*
float percent = modm::pct2f(tolerance);
```

!!! warning "This type is not guaranteed to hold more than 100 percent!"

## Content

```cpp
// Function
constexpr baudrate_t modm::Bd(T value);
constexpr baudrate_t modm::MBd(T value);
constexpr baudrate_t modm::kBd(T value);
constexpr bitrate_t modm::Mbps(T value);
constexpr bitrate_t modm::bps(T value);
constexpr bitrate_t modm::kbps(T value);
constexpr frequency_t modm::Hz(T value);
constexpr frequency_t modm::MHz(T value);
constexpr frequency_t modm::kHz(T value);
constexpr modm::literals::operator""_Bd(T value);
constexpr modm::literals::operator""_Hz(T value);
constexpr modm::literals::operator""_MBd(T value);
constexpr modm::literals::operator""_MHz(T value);
constexpr modm::literals::operator""_Mbps(T value);
constexpr modm::literals::operator""_bps(T value);
constexpr modm::literals::operator""_kBd(T value);
constexpr modm::literals::operator""_kHz(T value);
constexpr modm::literals::operator""_kbps(T value);
constexpr modm::literals::operator""_pct(T value);
constexpr percent_t modm::pct(T value);
// Typedef
using modm::baudrate_t = typedef uint32_t;
using modm::bitrate_t = typedef uint32_t;
using modm::frequency_t = typedef uint32_t;
using modm::percent_t = typedef uint16_t;
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:math:units Pages: 1 -->
<svg width="162pt" height="135pt"
 viewBox="0.00 0.00 162.00 135.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 131)">
<title>modm:math:units</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-131 158,-131 158,4 -4,4"/>
<!-- modm_math_units -->
<g id="node1" class="node">
<title>modm_math_units</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="111,-53 55,-53 55,0 111,0 111,-53"/>
<text text-anchor="middle" x="83" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="83" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="83" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">units</text>
</g>
<!-- modm_architecture -->
<g id="node2" class="node">
<title>modm_architecture</title>
<g id="a_node2"><a xlink:href="../modm-architecture" xlink:title="modm:&#10;architecture">
<polygon fill="#d3d3d3" stroke="#000000" points="80,-127 0,-127 0,-89 80,-89 80,-127"/>
<text text-anchor="middle" x="40" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="40" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture</text>
</a>
</g>
</g>
<!-- modm_math_units&#45;&gt;modm_architecture -->
<g id="edge1" class="edge">
<title>modm_math_units&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M62.541,-53.1861C57.6301,-61.6185 52.6676,-70.9667 48.5561,-79.5196"/>
<polygon fill="#000000" stroke="#000000" points="45.3116,-78.1982 44.3239,-88.7469 51.6743,-81.1166 45.3116,-78.1982"/>
</g>
<!-- modm_math -->
<g id="node3" class="node">
<title>modm_math</title>
<g id="a_node3"><a xlink:href="../modm-math" xlink:title="modm:&#10;math">
<polygon fill="#d3d3d3" stroke="#000000" points="154,-127 98,-127 98,-89 154,-89 154,-127"/>
<text text-anchor="middle" x="126" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="126" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">math</text>
</a>
</g>
</g>
<!-- modm_math_units&#45;&gt;modm_math -->
<g id="edge2" class="edge">
<title>modm_math_units&#45;&gt;modm_math</title>
<path fill="none" stroke="#000000" d="M90.7006,-53.1861C94.7735,-61.7999 99.8008,-71.3692 104.8202,-80.0703"/>
<polygon fill="#000000" stroke="#000000" points="101.8721,-81.96 110.0078,-88.7469 107.8802,-78.3679 101.8721,-81.96"/>
</g>
<!-- modm_architecture&#45;&gt;modm_math_units -->
<g id="edge3" class="edge">
<title>modm_architecture&#45;&gt;modm_math_units</title>
<path fill="none" stroke="#000000" d="M55.9922,-88.7469C60.8901,-80.8692 66.1301,-71.4896 70.7499,-62.432"/>
<polygon fill="#000000" stroke="#000000" points="74.0247,-63.704 75.2994,-53.1861 67.7439,-60.6135 74.0247,-63.704"/>
</g>
<!-- modm_math&#45;&gt;modm_math_units -->
<g id="edge4" class="edge">
<title>modm_math&#45;&gt;modm_math_units</title>
<path fill="none" stroke="#000000" d="M121.6761,-88.7469C118.2253,-80.7862 113.5055,-71.292 108.5099,-62.1461"/>
<polygon fill="#000000" stroke="#000000" points="111.4186,-60.1787 103.459,-53.1861 105.3208,-63.6161 111.4186,-60.1787"/>
</g>
</g>
</svg>
