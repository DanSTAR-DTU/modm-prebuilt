!!! warning "These module docs are in beta and may be incomplete."

# modm:architecture:register: General Purpose Registers

Data structures to provide a native register abstraction.

These data structures are used to describe the relationship of single bits,
bit groups and bit configurations in registers with type-safe access.

Registers can be made up of three things:

- Bits: a single bit (position N),
- Configurations: a combination of bits where the meaning does not correspond to its numeric value (position [N, M])
- Values: a numeric value (position [N, M])

Example of an 8-bit register called Control

```
   7    6    5      4      3      2      1    0
| EN | FS | PRE1 | PRE0 | DEL2 | DEL1 | DEL0 | |
```

- Bit 7: Enable
- Bit 6: Full Scale
- Configuration [5, 4]: Prescaler
    - 00: Divide by 1
    - 01: Divide by 2
    - 10: Divide by 4
    - 11: Divide by 8
- Value [3, 1]: Start-Up Delay in ms

## Register Bits

The bits can be modelled using strongly-typed enums and the Flags template class as follows:

```cpp
enum class
Control : uint8_t
{
    EN = Bit7,	///< bit documentation
    FS = Bit6,

    PRE1 = Bit5,
    PRE0 = Bit4,

    DEL2 = Bit3,
    DEL1 = Bit2,
    DEL0 = Bit1,
};
MODM_FLAGS8(Control);
// expands to
// typedef modm::Flags8< Control >  Control_t;
// and some enum operator overloading magic
```

You can handle all its register bits as you would expect:

```cpp
Control_t control = Control::EN;
control = Control::EN | Control::FS;
control &= ~Control::FS;
control |= Control::FS;
control ^= Control::PRE1;
bool isSet = control & Control::FS;

control.reset(Control::PRE1 | Control::PRE0);
control.set(Control::DEL0);

bool noneSet = control.none(Control::PRE1 | Control::PRE0);
bool allSet = control.all(Control::EN | Control::FS);
```

You still get raw access if you really need it:

```cpp
uint8_t raw = control.value; // the underlying type
control.value = 0x24;
```

The access is type-safe, you cannot use bits from two different registers:

```cpp
enum class Control2 : uint8_t
{
    DIS = Bit4,
    HS = Bit3,
};
MODM_FLAGS8(Control2);

auto control = Control::EN | Control2::HS; // compile error
```

You can even overload functions on argument type now:

```cpp
void write(Control_t control);
void write(Control2_t control);

write(Control::EN | Control::FS);  // calls #1
write(Control2::DIS);              // calls #2
```


## Register Configurations

Configurations are also described as a strongly-typed enum and then wrapped into the Configuration template class.

```cpp
enum class
Prescaler : uint8_t
{
    Div1 = 0,				///< configuration documentation
    Div2 = int(Control::PRE0),
    Div4 = int(Control::PRE1),
    Div8 = int(Control::PRE1) | int(Control::PRE0),
};
typedef Configuration< Control_t, Prescaler, (Bit5 | Bit4) >  Prescaler_t;
```

The `Prescaler` enum values are already shifted in this example (hence the `(Bit5 | Bit4)` mask), however you can also declare the prescaler values non-shifted and let the wrapper shift it:

```cpp
enum class Prescaler : uint8_t
{
    Div1 = 0,
    Div2 = 1,
    Div4 = 2,
    Div8 = 3,
};
typedef Configuration<Control_t, Prescaler, 0b11, 4> Prescaler_t;
```

Why? If you have two or more configurations with the same selections in the same register,  you can simply add another one:

```cpp
typedef Configuration< Control_t, Prescaler, 0b11, 6 >  Prescaler2_t;
```

Configurations can be used inline:

```cpp
Control_t control = Control::EN | Prescaler_t(Prescaler::Div2);
Control_t control &= ~Prescaler_t::mask();
```

But do not have to:

```cpp
Prescaler_t::set(control, Prescaler::Div2);
Prescaler_t::reset(control);
Prescaler prescaler = Prescaler_t::get(control);
```


## Register Values

Values are described using the Value template class which masks and shifts the value as required.
In our example the value has a width of 3 bits and needs to be shifted 1 bit:

```cpp
typedef Value< Control_t, 3, 1 >  Delay_t;
```

This can be used the same way as the Configuration:

```cpp
Control_t control = Control::EN | Prescaler_t(Prescaler::Div2) | Delay_t(4);
Control_t control &= ~Delay_t::mask();

Delay_t::set(control, 7);
Delay_t::reset(control);
uint8_t delay = Delay_t::get(control);
```

See [Typesafe Register Access in C++](http://blog.modm.io/2015/02/25/typesafe-register-access-in-c++)
for a more detailed background on this implementation.

## Content

```cpp
// Struct
struct modm::Configuration< typename Parent , typename Enum , typename Parent::UnderlyingType Mask, typename Parent::UnderlyingType Position=0 >;
struct modm::Flags< typename Enum , typename T  >;
struct modm::FlagsGroup< T... >;
struct modm::FlagsOperators< typename Enum, typename T >;
struct modm::Register< typename T >;
struct modm::Value< typename Parent , typename Parent::UnderlyingType Width, typename Parent::UnderlyingType Position=0 >;
// Typedef
using modm::Flags16 = typedef Flags<Enum, uint16_t>;
using modm::Flags32 = typedef Flags<Enum, uint32_t>;
using modm::Flags8 = typedef Flags<Enum, uint8_t>;
using modm::Register16 = typedef Register<uint16_t>;
using modm::Register32 = typedef Register<uint32_t>;
using modm::Register8 = typedef Register<uint8_t>;
// Define
#define MODM_FLAGS16(Enum)
#define MODM_FLAGS32(Enum)
#define MODM_FLAGS8(Enum)
#define MODM_TYPE_FLAGS(Parent)
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:architecture:register Pages: 1 -->
<svg width="746pt" height="239pt"
 viewBox="0.00 0.00 746.00 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:architecture:register</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-235 742,-235 742,4 -4,4"/>
<!-- modm_architecture_register -->
<g id="node1" class="node">
<title>modm_architecture_register</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="446,-142 362,-142 362,-89 446,-89 446,-142"/>
<text text-anchor="middle" x="404" y="-126.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="404" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="404" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">register</text>
</g>
<!-- modm_architecture -->
<g id="node2" class="node">
<title>modm_architecture</title>
<g id="a_node2"><a xlink:href="../modm-architecture" xlink:title="modm:&#10;architecture">
<polygon fill="#d3d3d3" stroke="#000000" points="358,-223.5 278,-223.5 278,-185.5 358,-185.5 358,-223.5"/>
<text text-anchor="middle" x="318" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="318" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">architecture</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_architecture -->
<g id="edge1" class="edge">
<title>modm_architecture_register&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M378.1972,-142.2029C367.2629,-153.5186 354.5524,-166.6725 343.6363,-177.9694"/>
<polygon fill="#000000" stroke="#000000" points="340.8415,-175.8249 336.4096,-185.4482 345.8754,-180.6891 340.8415,-175.8249"/>
</g>
<!-- modm_io -->
<g id="node3" class="node">
<title>modm_io</title>
<g id="a_node3"><a xlink:href="../modm-io" xlink:title="modm:&#10;io">
<polygon fill="#d3d3d3" stroke="#000000" points="432,-223.5 376,-223.5 376,-185.5 432,-185.5 432,-223.5"/>
<text text-anchor="middle" x="404" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="404" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">io</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_io -->
<g id="edge2" class="edge">
<title>modm_architecture_register&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M404,-142.2029C404,-152.6482 404,-164.6597 404,-175.3311"/>
<polygon fill="#000000" stroke="#000000" points="400.5001,-175.4482 404,-185.4482 407.5001,-175.4483 400.5001,-175.4482"/>
</g>
<!-- modm_math_utils -->
<g id="node4" class="node">
<title>modm_math_utils</title>
<g id="a_node4"><a xlink:href="../modm-math-utils" xlink:title="modm:&#10;math:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="506,-231 450,-231 450,-178 506,-178 506,-231"/>
<text text-anchor="middle" x="478" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="478" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="478" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_math_utils -->
<g id="edge3" class="edge">
<title>modm_architecture_register&#45;&gt;modm_math_utils</title>
<path fill="none" stroke="#000000" d="M426.2024,-142.2029C433.4464,-150.9153 441.5964,-160.7174 449.2502,-169.9225"/>
<polygon fill="#000000" stroke="#000000" points="446.7286,-172.3643 455.8132,-177.8159 452.1111,-167.8889 446.7286,-172.3643"/>
</g>
<!-- modm_architecture_gpio_expander -->
<g id="node5" class="node">
<title>modm_architecture_gpio_expander</title>
<g id="a_node5"><a xlink:href="../modm-architecture-gpio-expander" xlink:title="modm:&#10;architecture:&#10;gpio.expander">
<polygon fill="#d3d3d3" stroke="#000000" points="94,-53 0,-53 0,0 94,0 94,-53"/>
<text text-anchor="middle" x="47" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="47" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="47" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio.expander</text>
</a>
</g>
</g>
<!-- modm_architecture_gpio_expander&#45;&gt;modm_architecture_register -->
<g id="edge4" class="edge">
<title>modm_architecture_gpio_expander&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M94.0307,-49.7189C97.0364,-50.9022 100.0425,-52.0089 103,-53 187.9124,-81.4562 290.8252,-99.4599 351.6634,-108.4882"/>
<polygon fill="#000000" stroke="#000000" points="351.3874,-111.9849 361.788,-109.9652 352.398,-105.0582 351.3874,-111.9849"/>
</g>
<!-- modm_architecture_i2c_multiplexer -->
<g id="node6" class="node">
<title>modm_architecture_i2c_multiplexer</title>
<g id="a_node6"><a xlink:href="../modm-architecture-i2c-multiplexer" xlink:title="modm:&#10;architecture:&#10;i2c.multiplexer">
<polygon fill="#d3d3d3" stroke="#000000" points="212,-53 112,-53 112,0 212,0 212,-53"/>
<text text-anchor="middle" x="162" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="162" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="162" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.multiplexer</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_multiplexer&#45;&gt;modm_architecture_register -->
<g id="edge5" class="edge">
<title>modm_architecture_i2c_multiplexer&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M212.0769,-49.3736C215.0843,-50.6284 218.0751,-51.8466 221,-53 264.7216,-70.2418 315.3486,-87.3161 352.2547,-99.2502"/>
<polygon fill="#000000" stroke="#000000" points="351.2138,-102.592 361.8052,-102.3229 353.3577,-95.9284 351.2138,-102.592"/>
</g>
<!-- modm_architecture_memory -->
<g id="node7" class="node">
<title>modm_architecture_memory</title>
<g id="a_node7"><a xlink:href="../modm-architecture-memory" xlink:title="modm:&#10;architecture:&#10;memory">
<polygon fill="#d3d3d3" stroke="#000000" points="314,-53 230,-53 230,0 314,0 314,-53"/>
<text text-anchor="middle" x="272" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="272" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="272" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">memory</text>
</a>
</g>
</g>
<!-- modm_architecture_memory&#45;&gt;modm_architecture_register -->
<g id="edge6" class="edge">
<title>modm_architecture_memory&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M311.6044,-53.2029C325.5702,-62.6193 341.424,-73.3086 356.0076,-83.1415"/>
<polygon fill="#000000" stroke="#000000" points="354.1755,-86.1275 364.4236,-88.8159 358.0888,-80.3235 354.1755,-86.1275"/>
</g>
<!-- modm_driver_bme280 -->
<g id="node8" class="node">
<title>modm_driver_bme280</title>
<g id="a_node8"><a xlink:href="../modm-driver-bme280" xlink:title="modm:&#10;driver:&#10;bme280">
<polygon fill="#d3d3d3" stroke="#000000" points="393.5,-53 332.5,-53 332.5,0 393.5,0 393.5,-53"/>
<text text-anchor="middle" x="363" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="363" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">driver:</text>
<text text-anchor="middle" x="363" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">bme280</text>
</a>
</g>
</g>
<!-- modm_driver_bme280&#45;&gt;modm_architecture_register -->
<g id="edge7" class="edge">
<title>modm_driver_bme280&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M375.3014,-53.2029C379.1122,-61.4753 383.3756,-70.73 387.4265,-79.5235"/>
<polygon fill="#000000" stroke="#000000" points="384.3443,-81.1978 391.7073,-88.8159 390.7021,-78.2689 384.3443,-81.1978"/>
</g>
<!-- modm_platform_adc -->
<g id="node9" class="node">
<title>modm_platform_adc</title>
<g id="a_node9"><a xlink:href="../modm-platform-adc" xlink:title="modm:&#10;platform:&#10;adc">
<polygon fill="#d3d3d3" stroke="#000000" points="480,-53 412,-53 412,0 480,0 480,-53"/>
<text text-anchor="middle" x="446" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="446" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="446" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">adc</text>
</a>
</g>
</g>
<!-- modm_platform_adc&#45;&gt;modm_architecture_register -->
<g id="edge8" class="edge">
<title>modm_platform_adc&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M433.3986,-53.2029C429.4948,-61.4753 425.1274,-70.73 420.9777,-79.5235"/>
<polygon fill="#000000" stroke="#000000" points="417.695,-78.2786 416.5925,-88.8159 424.0256,-81.2661 417.695,-78.2786"/>
</g>
<!-- modm_platform_spi -->
<g id="node10" class="node">
<title>modm_platform_spi</title>
<g id="a_node10"><a xlink:href="../modm-platform-spi" xlink:title="modm:&#10;platform:&#10;spi">
<polygon fill="#d3d3d3" stroke="#000000" points="566,-53 498,-53 498,0 566,0 566,-53"/>
<text text-anchor="middle" x="532" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="532" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="532" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi</text>
</a>
</g>
</g>
<!-- modm_platform_spi&#45;&gt;modm_architecture_register -->
<g id="edge9" class="edge">
<title>modm_platform_spi&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M497.6983,-50.3504C483.3274,-60.3426 466.3896,-72.1198 450.8952,-82.8932"/>
<polygon fill="#000000" stroke="#000000" points="448.6734,-80.175 442.4612,-88.7575 452.6696,-85.9223 448.6734,-80.175"/>
</g>
<!-- modm_platform_timer -->
<g id="node11" class="node">
<title>modm_platform_timer</title>
<g id="a_node11"><a xlink:href="../modm-platform-timer" xlink:title="modm:&#10;platform:&#10;timer">
<polygon fill="#d3d3d3" stroke="#000000" points="652,-53 584,-53 584,0 652,0 652,-53"/>
<text text-anchor="middle" x="618" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="618" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="618" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">timer</text>
</a>
</g>
</g>
<!-- modm_platform_timer&#45;&gt;modm_architecture_register -->
<g id="edge10" class="edge">
<title>modm_platform_timer&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M583.6057,-48.5244C580.7328,-50.1093 577.8407,-51.623 575,-53 536.2982,-71.76 490.4285,-88.2344 455.9442,-99.5529"/>
<polygon fill="#000000" stroke="#000000" points="454.5152,-96.3368 446.0828,-102.7513 456.6748,-102.9954 454.5152,-96.3368"/>
</g>
<!-- modm_platform_uart -->
<g id="node12" class="node">
<title>modm_platform_uart</title>
<g id="a_node12"><a xlink:href="../modm-platform-uart" xlink:title="modm:&#10;platform:&#10;uart">
<polygon fill="#d3d3d3" stroke="#000000" points="738,-53 670,-53 670,0 738,0 738,-53"/>
<text text-anchor="middle" x="704" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="704" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="704" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">uart</text>
</a>
</g>
</g>
<!-- modm_platform_uart&#45;&gt;modm_architecture_register -->
<g id="edge11" class="edge">
<title>modm_platform_uart&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M669.7664,-48.881C666.8575,-50.3859 663.9133,-51.784 661,-53 593.0676,-81.3562 509.4334,-98.7075 456.4028,-107.7169"/>
<polygon fill="#000000" stroke="#000000" points="455.5099,-104.3171 446.2185,-109.4083 456.6568,-111.2225 455.5099,-104.3171"/>
</g>
</g>
</svg>

