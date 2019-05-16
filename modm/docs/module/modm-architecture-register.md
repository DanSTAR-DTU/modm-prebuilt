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
<!-- Generated by graphviz version 2.40.1 (0)
 -->
<!-- Title: modm:architecture:register Pages: 1 -->
<svg width="895pt" height="239pt"
 viewBox="0.00 0.00 895.00 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:architecture:register</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-235 891,-235 891,4 -4,4"/>
<!-- modm_architecture_register -->
<g id="node1" class="node">
<title>modm_architecture_register</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="548.5,-142 438.5,-142 438.5,-89 548.5,-89 548.5,-142"/>
<text text-anchor="middle" x="493.5" y="-126.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="493.5" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="493.5" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">register</text>
</g>
<!-- modm_architecture -->
<g id="node2" class="node">
<title>modm_architecture</title>
<g id="a_node2"><a xlink:href="../modm-architecture" xlink:title="modm:&#10;architecture">
<polygon fill="#d3d3d3" stroke="#000000" points="442.5,-223.5 336.5,-223.5 336.5,-185.5 442.5,-185.5 442.5,-223.5"/>
<text text-anchor="middle" x="389.5" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="389.5" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">architecture</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_architecture -->
<g id="edge1" class="edge">
<title>modm_architecture_register&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M462.2966,-142.2029C448.6923,-153.845 432.8144,-167.4329 419.3652,-178.9423"/>
<polygon fill="#000000" stroke="#000000" points="417.0848,-176.2871 411.7627,-185.4482 421.6362,-181.6055 417.0848,-176.2871"/>
</g>
<!-- modm_io -->
<g id="node3" class="node">
<title>modm_io</title>
<g id="a_node3"><a xlink:href="../modm-io" xlink:title="modm:&#10;io">
<polygon fill="#d3d3d3" stroke="#000000" points="526,-223.5 461,-223.5 461,-185.5 526,-185.5 526,-223.5"/>
<text text-anchor="middle" x="493.5" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="493.5" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">io</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_io -->
<g id="edge2" class="edge">
<title>modm_architecture_register&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M493.5,-142.2029C493.5,-152.6482 493.5,-164.6597 493.5,-175.3311"/>
<polygon fill="#000000" stroke="#000000" points="490.0001,-175.4482 493.5,-185.4482 497.0001,-175.4483 490.0001,-175.4482"/>
</g>
<!-- modm_math_utils -->
<g id="node4" class="node">
<title>modm_math_utils</title>
<g id="a_node4"><a xlink:href="../modm-math-utils" xlink:title="modm:&#10;math:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="609,-231 544,-231 544,-178 609,-178 609,-231"/>
<text text-anchor="middle" x="576.5" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="576.5" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="576.5" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_math_utils -->
<g id="edge3" class="edge">
<title>modm_architecture_register&#45;&gt;modm_math_utils</title>
<path fill="none" stroke="#000000" d="M518.4027,-142.2029C526.6919,-151.0913 536.0387,-161.1138 544.7732,-170.4797"/>
<polygon fill="#000000" stroke="#000000" points="542.2349,-172.8897 551.6148,-177.8159 547.3542,-168.1155 542.2349,-172.8897"/>
</g>
<!-- modm_architecture_gpio_expander -->
<g id="node5" class="node">
<title>modm_architecture_gpio_expander</title>
<g id="a_node5"><a xlink:href="../modm-architecture-gpio-expander" xlink:title="modm:&#10;architecture:&#10;gpio.expander">
<polygon fill="#d3d3d3" stroke="#000000" points="119,-53 0,-53 0,0 119,0 119,-53"/>
<text text-anchor="middle" x="59.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="59.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="59.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio.expander</text>
</a>
</g>
</g>
<!-- modm_architecture_gpio_expander&#45;&gt;modm_architecture_register -->
<g id="edge4" class="edge">
<title>modm_architecture_gpio_expander&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M119.3466,-50.5804C122.0871,-51.4433 124.8138,-52.2555 127.5,-53 230.8812,-81.6525 354.5253,-99.4651 428.3968,-108.4226"/>
<polygon fill="#000000" stroke="#000000" points="428.138,-111.9165 438.4827,-109.628 428.9687,-104.966 428.138,-111.9165"/>
</g>
<!-- modm_architecture_i2c_multiplexer -->
<g id="node6" class="node">
<title>modm_architecture_i2c_multiplexer</title>
<g id="a_node6"><a xlink:href="../modm-architecture-i2c-multiplexer" xlink:title="modm:&#10;architecture:&#10;i2c.multiplexer">
<polygon fill="#d3d3d3" stroke="#000000" points="262,-53 137,-53 137,0 262,0 262,-53"/>
<text text-anchor="middle" x="199.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="199.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="199.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.multiplexer</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_multiplexer&#45;&gt;modm_architecture_register -->
<g id="edge5" class="edge">
<title>modm_architecture_i2c_multiplexer&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M262.2279,-49.9156C265.3556,-50.98 268.4591,-52.014 271.5,-53 323.9064,-69.9925 384.1099,-86.7739 428.5897,-98.6623"/>
<polygon fill="#000000" stroke="#000000" points="427.838,-102.084 438.4018,-101.2737 429.6384,-95.3195 427.838,-102.084"/>
</g>
<!-- modm_architecture_memory -->
<g id="node7" class="node">
<title>modm_architecture_memory</title>
<g id="a_node7"><a xlink:href="../modm-architecture-memory" xlink:title="modm:&#10;architecture:&#10;memory">
<polygon fill="#d3d3d3" stroke="#000000" points="390.5,-53 280.5,-53 280.5,0 390.5,0 390.5,-53"/>
<text text-anchor="middle" x="335.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="335.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="335.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">memory</text>
</a>
</g>
</g>
<!-- modm_architecture_memory&#45;&gt;modm_architecture_register -->
<g id="edge6" class="edge">
<title>modm_architecture_memory&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M382.9052,-53.2029C400.0118,-62.8389 419.4848,-73.8079 437.2734,-83.8281"/>
<polygon fill="#000000" stroke="#000000" points="435.9701,-87.111 446.4007,-88.9694 439.4056,-81.012 435.9701,-87.111"/>
</g>
<!-- modm_driver_bme280 -->
<g id="node8" class="node">
<title>modm_driver_bme280</title>
<g id="a_node8"><a xlink:href="../modm-driver-bme280" xlink:title="modm:&#10;driver:&#10;bme280">
<polygon fill="#d3d3d3" stroke="#000000" points="482.5,-53 408.5,-53 408.5,0 482.5,0 482.5,-53"/>
<text text-anchor="middle" x="445.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="445.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">driver:</text>
<text text-anchor="middle" x="445.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">bme280</text>
</a>
</g>
</g>
<!-- modm_driver_bme280&#45;&gt;modm_architecture_register -->
<g id="edge7" class="edge">
<title>modm_driver_bme280&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M459.9016,-53.2029C464.4105,-61.5633 469.4607,-70.927 474.2482,-79.8039"/>
<polygon fill="#000000" stroke="#000000" points="471.2811,-81.6758 479.1086,-88.8159 477.4422,-78.3529 471.2811,-81.6758"/>
</g>
<!-- modm_platform_adc -->
<g id="node9" class="node">
<title>modm_platform_adc</title>
<g id="a_node9"><a xlink:href="../modm-platform-adc" xlink:title="modm:&#10;platform:&#10;adc">
<polygon fill="#d3d3d3" stroke="#000000" points="584,-53 501,-53 501,0 584,0 584,-53"/>
<text text-anchor="middle" x="542.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="542.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="542.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">adc</text>
</a>
</g>
</g>
<!-- modm_platform_adc&#45;&gt;modm_architecture_register -->
<g id="edge8" class="edge">
<title>modm_platform_adc&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M527.7984,-53.2029C523.1955,-61.5633 518.0402,-70.927 513.1529,-79.8039"/>
<polygon fill="#000000" stroke="#000000" points="509.9482,-78.3678 508.1912,-88.8159 516.0803,-81.7439 509.9482,-78.3678"/>
</g>
<!-- modm_platform_spi -->
<g id="node10" class="node">
<title>modm_platform_spi</title>
<g id="a_node10"><a xlink:href="../modm-platform-spi" xlink:title="modm:&#10;platform:&#10;spi">
<polygon fill="#d3d3d3" stroke="#000000" points="685,-53 602,-53 602,0 685,0 685,-53"/>
<text text-anchor="middle" x="643.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="643.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="643.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi</text>
</a>
</g>
</g>
<!-- modm_platform_spi&#45;&gt;modm_architecture_register -->
<g id="edge9" class="edge">
<title>modm_platform_spi&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M601.7166,-51.2915C584.7314,-61.3694 564.8978,-73.1373 546.8834,-83.8258"/>
<polygon fill="#000000" stroke="#000000" points="545.0208,-80.8612 538.2066,-88.9741 548.5927,-86.8813 545.0208,-80.8612"/>
</g>
<!-- modm_platform_timer -->
<g id="node11" class="node">
<title>modm_platform_timer</title>
<g id="a_node11"><a xlink:href="../modm-platform-timer" xlink:title="modm:&#10;platform:&#10;timer">
<polygon fill="#d3d3d3" stroke="#000000" points="786,-53 703,-53 703,0 786,0 786,-53"/>
<text text-anchor="middle" x="744.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="744.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="744.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">timer</text>
</a>
</g>
</g>
<!-- modm_platform_timer&#45;&gt;modm_architecture_register -->
<g id="edge10" class="edge">
<title>modm_platform_timer&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M702.761,-49.3455C699.991,-50.6364 697.221,-51.8674 694.5,-53 650.1792,-71.4474 598.4165,-87.42 558.5205,-98.6"/>
<polygon fill="#000000" stroke="#000000" points="557.3464,-95.2934 548.6449,-101.3377 559.2165,-102.039 557.3464,-95.2934"/>
</g>
<!-- modm_platform_uart -->
<g id="node12" class="node">
<title>modm_platform_uart</title>
<g id="a_node12"><a xlink:href="../modm-platform-uart" xlink:title="modm:&#10;platform:&#10;uart">
<polygon fill="#d3d3d3" stroke="#000000" points="887,-53 804,-53 804,0 887,0 887,-53"/>
<text text-anchor="middle" x="845.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="845.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="845.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">uart</text>
</a>
</g>
</g>
<!-- modm_platform_uart&#45;&gt;modm_architecture_register -->
<g id="edge11" class="edge">
<title>modm_platform_uart&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M803.8823,-49.6584C801.0826,-50.8726 798.2729,-52.0012 795.5,-53 716.4086,-81.4872 620.7337,-98.5409 558.568,-107.4701"/>
<polygon fill="#000000" stroke="#000000" points="557.9204,-104.0267 548.5059,-108.8864 558.8962,-110.9584 557.9204,-104.0267"/>
</g>
</g>
</svg>

