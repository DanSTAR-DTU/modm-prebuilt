!!! warning "These module docs are in beta and may be incomplete."

# modm:platform: Platform HAL

Target-specific implementation of the Hardware Abstraction Layer.

## Content

```cpp
// Class
class modm::CanBitTiming< int32_t Clk, int32_t Bitrate >;
class modm::platform::Adc1;
class modm::platform::Adc2;
class modm::platform::Adc3;
class modm::platform::AdcInterrupt1;
class modm::platform::AdcInterrupt2;
class modm::platform::AdcInterrupt3;
class modm::platform::AdvancedControlTimer;
class modm::platform::BasicTimer;
class modm::platform::BitBangSpiMaster< typename Sck , typename Mosi , typename Miso =GpioUnused >;
class modm::platform::Can1;
class modm::platform::Can2;
class modm::platform::CanFilter;
class modm::platform::GeneralPurposeTimer;
class modm::platform::GpioA0;
class modm::platform::GpioA10;
class modm::platform::GpioA11;
class modm::platform::GpioA12;
class modm::platform::GpioA13;
class modm::platform::GpioA14;
class modm::platform::GpioA15;
class modm::platform::GpioA1;
class modm::platform::GpioA2;
class modm::platform::GpioA3;
class modm::platform::GpioA4;
class modm::platform::GpioA5;
class modm::platform::GpioA6;
class modm::platform::GpioA7;
class modm::platform::GpioA8;
class modm::platform::GpioA9;
class modm::platform::GpioB0;
class modm::platform::GpioB10;
class modm::platform::GpioB12;
class modm::platform::GpioB13;
class modm::platform::GpioB14;
class modm::platform::GpioB15;
class modm::platform::GpioB1;
class modm::platform::GpioB2;
class modm::platform::GpioB3;
class modm::platform::GpioB4;
class modm::platform::GpioB5;
class modm::platform::GpioB6;
class modm::platform::GpioB7;
class modm::platform::GpioB8;
class modm::platform::GpioB9;
class modm::platform::GpioC0;
class modm::platform::GpioC10;
class modm::platform::GpioC11;
class modm::platform::GpioC12;
class modm::platform::GpioC13;
class modm::platform::GpioC14;
class modm::platform::GpioC15;
class modm::platform::GpioC1;
class modm::platform::GpioC2;
class modm::platform::GpioC3;
class modm::platform::GpioC4;
class modm::platform::GpioC5;
class modm::platform::GpioC6;
class modm::platform::GpioC7;
class modm::platform::GpioC8;
class modm::platform::GpioC9;
class modm::platform::GpioD2;
class modm::platform::GpioH0;
class modm::platform::GpioH1;
class modm::platform::GpioInverted< class Pin  >;
class modm::platform::GpioPort< class StartGpio , int8_t Width >;
class modm::platform::GpioSet< class... Gpios >;
class modm::platform::GpioUnused;
class modm::platform::I2cMaster1;
class modm::platform::I2cMaster2;
class modm::platform::I2cMaster3;
class modm::platform::Rcc;
class modm::platform::SoftwareGpioPort< class... Gpios >;
class modm::platform::SpiBase;
class modm::platform::SpiHal1;
class modm::platform::SpiHal2;
class modm::platform::SpiHal3;
class modm::platform::SpiMaster1;
class modm::platform::SpiMaster2;
class modm::platform::SpiMaster3;
class modm::platform::SysTickTimer;
class modm::platform::Timer10;
class modm::platform::Timer11;
class modm::platform::Timer12;
class modm::platform::Timer13;
class modm::platform::Timer14;
class modm::platform::Timer1;
class modm::platform::Timer2;
class modm::platform::Timer3;
class modm::platform::Timer4;
class modm::platform::Timer5;
class modm::platform::Timer6;
class modm::platform::Timer7;
class modm::platform::Timer8;
class modm::platform::Timer9;
class modm::platform::Uart4;
class modm::platform::Uart5;
class modm::platform::UartBase;
class modm::platform::UartBaudrate;
class modm::platform::UartHal4;
class modm::platform::UartHal5;
class modm::platform::Usart1;
class modm::platform::Usart2;
class modm::platform::Usart3;
class modm::platform::Usart6;
class modm::platform::UsartHal1;
class modm::platform::UsartHal2;
class modm::platform::UsartHal3;
class modm::platform::UsartHal6;
// Struct
struct modm::CanBitTiming::CanBitTimingConfiguration;
struct modm::platform::CanFilter::ExtendedFilterMask;
struct modm::platform::CanFilter::ExtendedFilterMaskShort;
struct modm::platform::CanFilter::ExtendedIdentifier;
struct modm::platform::CanFilter::ExtendedIdentifierShort;
struct modm::platform::CanFilter::Identifier;
struct modm::platform::CanFilter::IdentifierShort;
struct modm::platform::CanFilter::StandardFilterMask;
struct modm::platform::CanFilter::StandardFilterMaskShort;
struct modm::platform::CanFilter::StandardIdentifier;
struct modm::platform::CanFilter::StandardIdentifierShort;
struct modm::platform::Gpio;
struct modm::platform::Rcc::flash_latency;
// Function
void _delay_ms(uint16_t ms);
void _delay_ns(uint16_t ns);
void _delay_us(uint16_t us);
// Enum
enum ErrorCode;
enum class Peripheral;
// Typedef
typedef void(* modm::platform::InterruptHandler) (void))(void);
using modm::modm::cortex::SysTickTimer = typedef ::modm::platform::SysTickTimer;
using modm::platform::ClockControl = typedef Rcc;
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (0)
 -->
<!-- Title: modm:platform Pages: 1 -->
<svg width="1573pt" height="135pt"
 viewBox="0.00 0.00 1573.00 135.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 131)">
<title>modm:platform</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-131 1569,-131 1569,4 -4,4"/>
<!-- modm_platform -->
<g id="node1" class="node">
<title>modm_platform</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="826.5,-127 748.5,-127 748.5,-89 826.5,-89 826.5,-127"/>
<text text-anchor="middle" x="787.5" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="787.5" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform</text>
</g>
<!-- modm_platform_adc -->
<g id="node2" class="node">
<title>modm_platform_adc</title>
<g id="a_node2"><a xlink:href="../modm-platform-adc" xlink:title="modm:&#10;platform:&#10;adc">
<polygon fill="#d3d3d3" stroke="#000000" points="83,-53 0,-53 0,0 83,0 83,-53"/>
<text text-anchor="middle" x="41.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="41.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="41.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">adc</text>
</a>
</g>
</g>
<!-- modm_platform_adc&#45;&gt;modm_platform -->
<g id="edge1" class="edge">
<title>modm_platform_adc&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M83.2676,-50.1796C86.0069,-51.2485 88.7643,-52.2041 91.5,-53 213.1534,-88.3918 600.2544,-102.658 738.1394,-106.7001"/>
<polygon fill="#000000" stroke="#000000" points="738.3029,-110.2062 748.3995,-106.9954 738.5043,-103.2091 738.3029,-110.2062"/>
</g>
<!-- modm_platform_can -->
<g id="node3" class="node">
<title>modm_platform_can</title>
<g id="a_node3"><a xlink:href="../modm-platform-can" xlink:title="modm:&#10;platform:&#10;can">
<polygon fill="#d3d3d3" stroke="#000000" points="184,-53 101,-53 101,0 184,0 184,-53"/>
<text text-anchor="middle" x="142.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="142.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="142.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">can</text>
</a>
</g>
</g>
<!-- modm_platform_can&#45;&gt;modm_platform -->
<g id="edge2" class="edge">
<title>modm_platform_can&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M184.2802,-50.1366C187.0164,-51.2162 189.7697,-52.1859 192.5,-53 294.2909,-83.3499 613.9623,-100.338 737.8024,-105.9208"/>
<polygon fill="#000000" stroke="#000000" points="737.9757,-109.4319 748.1214,-106.3799 738.2869,-102.4388 737.9757,-109.4319"/>
</g>
<!-- modm_platform_can_common -->
<g id="node4" class="node">
<title>modm_platform_can_common</title>
<g id="a_node4"><a xlink:href="../modm-platform-can-common" xlink:title="modm:&#10;platform:&#10;can.common">
<polygon fill="#d3d3d3" stroke="#000000" points="309,-53 202,-53 202,0 309,0 309,-53"/>
<text text-anchor="middle" x="255.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="255.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="255.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">can.common</text>
</a>
</g>
</g>
<!-- modm_platform_can_common&#45;&gt;modm_platform -->
<g id="edge3" class="edge">
<title>modm_platform_can_common&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M309.2784,-50.5641C312.0344,-51.457 314.7852,-52.2772 317.5,-53 467.4047,-92.9107 651.8731,-103.8681 738.3622,-106.8705"/>
<polygon fill="#000000" stroke="#000000" points="738.3062,-110.3705 748.4151,-107.1987 738.5346,-103.3742 738.3062,-110.3705"/>
</g>
<!-- modm_platform_clock -->
<g id="node5" class="node">
<title>modm_platform_clock</title>
<g id="a_node5"><a xlink:href="../modm-platform-clock" xlink:title="modm:&#10;platform:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="410,-53 327,-53 327,0 410,0 410,-53"/>
<text text-anchor="middle" x="368.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="368.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="368.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_platform_clock&#45;&gt;modm_platform -->
<g id="edge4" class="edge">
<title>modm_platform_clock&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M410.0547,-49.8411C412.8699,-51.0107 415.7001,-52.0794 418.5,-53 529.0467,-89.3472 666.1749,-101.7663 738.1254,-105.9409"/>
<polygon fill="#000000" stroke="#000000" points="738.0798,-109.4436 748.2571,-106.4989 738.4648,-102.4542 738.0798,-109.4436"/>
</g>
<!-- modm_platform_core -->
<g id="node6" class="node">
<title>modm_platform_core</title>
<g id="a_node6"><a xlink:href="../modm-platform-core" xlink:title="modm:&#10;platform:&#10;core">
<polygon fill="#d3d3d3" stroke="#000000" points="511,-53 428,-53 428,0 511,0 511,-53"/>
<text text-anchor="middle" x="469.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="469.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="469.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">core</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_platform -->
<g id="edge5" class="edge">
<title>modm_platform_core&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M511.1157,-49.6637C513.9159,-50.8767 516.7262,-52.0035 519.5,-53 593.4095,-79.5515 683.6437,-94.7486 738.2835,-102.1823"/>
<polygon fill="#000000" stroke="#000000" points="738.0169,-105.6775 748.3905,-103.5238 738.938,-98.7384 738.0169,-105.6775"/>
</g>
<!-- modm_platform_cortex_m -->
<g id="node7" class="node">
<title>modm_platform_cortex_m</title>
<g id="a_node7"><a xlink:href="../modm-platform-cortex-m" xlink:title="modm:&#10;platform:&#10;cortex&#45;m">
<polygon fill="#d3d3d3" stroke="#000000" points="612,-53 529,-53 529,0 612,0 612,-53"/>
<text text-anchor="middle" x="570.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="570.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="570.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">cortex&#45;m</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_platform -->
<g id="edge6" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M612.2614,-49.2925C615.0259,-50.5964 617.7885,-51.8448 620.5,-53 659.3504,-69.552 704.9812,-84.2765 738.7183,-94.3149"/>
<polygon fill="#000000" stroke="#000000" points="737.7621,-97.6819 748.3434,-97.1479 739.7386,-90.9667 737.7621,-97.6819"/>
</g>
<!-- modm_platform_gpio -->
<g id="node8" class="node">
<title>modm_platform_gpio</title>
<g id="a_node8"><a xlink:href="../modm-platform-gpio" xlink:title="modm:&#10;platform:&#10;gpio">
<polygon fill="#d3d3d3" stroke="#000000" points="713,-53 630,-53 630,0 713,0 713,-53"/>
<text text-anchor="middle" x="671.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="671.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="671.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio</text>
</a>
</g>
</g>
<!-- modm_platform_gpio&#45;&gt;modm_platform -->
<g id="edge7" class="edge">
<title>modm_platform_gpio&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M709.4827,-53.1861C723.0333,-62.7066 738.2454,-73.3944 751.614,-82.787"/>
<polygon fill="#000000" stroke="#000000" points="749.9024,-85.8619 760.0969,-88.7469 753.9266,-80.1342 749.9024,-85.8619"/>
</g>
<!-- modm_platform_gpio_common -->
<g id="node9" class="node">
<title>modm_platform_gpio_common</title>
<g id="a_node9"><a xlink:href="../modm-platform-gpio-common" xlink:title="modm:&#10;platform:&#10;gpio.common">
<polygon fill="#d3d3d3" stroke="#000000" points="844,-53 731,-53 731,0 844,0 844,-53"/>
<text text-anchor="middle" x="787.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="787.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="787.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio.common</text>
</a>
</g>
</g>
<!-- modm_platform_gpio_common&#45;&gt;modm_platform -->
<g id="edge8" class="edge">
<title>modm_platform_gpio_common&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M787.5,-53.1861C787.5,-61.3465 787.5,-70.3646 787.5,-78.6895"/>
<polygon fill="#000000" stroke="#000000" points="784.0001,-78.7469 787.5,-88.7469 791.0001,-78.747 784.0001,-78.7469"/>
</g>
<!-- modm_platform_i2c -->
<g id="node10" class="node">
<title>modm_platform_i2c</title>
<g id="a_node10"><a xlink:href="../modm-platform-i2c" xlink:title="modm:&#10;platform:&#10;i2c">
<polygon fill="#d3d3d3" stroke="#000000" points="945,-53 862,-53 862,0 945,0 945,-53"/>
<text text-anchor="middle" x="903.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="903.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="903.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c</text>
</a>
</g>
</g>
<!-- modm_platform_i2c&#45;&gt;modm_platform -->
<g id="edge9" class="edge">
<title>modm_platform_i2c&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M865.5173,-53.1861C851.9667,-62.7066 836.7546,-73.3944 823.386,-82.787"/>
<polygon fill="#000000" stroke="#000000" points="821.0734,-80.1342 814.9031,-88.7469 825.0976,-85.8619 821.0734,-80.1342"/>
</g>
<!-- modm_platform_rcc -->
<g id="node11" class="node">
<title>modm_platform_rcc</title>
<g id="a_node11"><a xlink:href="../modm-platform-rcc" xlink:title="modm:&#10;platform:&#10;rcc">
<polygon fill="#d3d3d3" stroke="#000000" points="1046,-53 963,-53 963,0 1046,0 1046,-53"/>
<text text-anchor="middle" x="1004.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1004.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1004.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">rcc</text>
</a>
</g>
</g>
<!-- modm_platform_rcc&#45;&gt;modm_platform -->
<g id="edge10" class="edge">
<title>modm_platform_rcc&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M962.7386,-49.2925C959.9741,-50.5964 957.2115,-51.8448 954.5,-53 915.6496,-69.552 870.0188,-84.2765 836.2817,-94.3149"/>
<polygon fill="#000000" stroke="#000000" points="835.2614,-90.9667 826.6566,-97.1479 837.2379,-97.6819 835.2614,-90.9667"/>
</g>
<!-- modm_platform_spi -->
<g id="node12" class="node">
<title>modm_platform_spi</title>
<g id="a_node12"><a xlink:href="../modm-platform-spi" xlink:title="modm:&#10;platform:&#10;spi">
<polygon fill="#d3d3d3" stroke="#000000" points="1147,-53 1064,-53 1064,0 1147,0 1147,-53"/>
<text text-anchor="middle" x="1105.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1105.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1105.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi</text>
</a>
</g>
</g>
<!-- modm_platform_spi&#45;&gt;modm_platform -->
<g id="edge11" class="edge">
<title>modm_platform_spi&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1063.8843,-49.6637C1061.0841,-50.8767 1058.2738,-52.0035 1055.5,-53 981.5905,-79.5515 891.3563,-94.7486 836.7165,-102.1823"/>
<polygon fill="#000000" stroke="#000000" points="836.062,-98.7384 826.6095,-103.5238 836.9831,-105.6775 836.062,-98.7384"/>
</g>
<!-- modm_platform_spi_bitbang -->
<g id="node13" class="node">
<title>modm_platform_spi_bitbang</title>
<g id="a_node13"><a xlink:href="../modm-platform-spi-bitbang" xlink:title="modm:&#10;platform:&#10;spi.bitbang">
<polygon fill="#d3d3d3" stroke="#000000" points="1262,-53 1165,-53 1165,0 1262,0 1262,-53"/>
<text text-anchor="middle" x="1213.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1213.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1213.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi.bitbang</text>
</a>
</g>
</g>
<!-- modm_platform_spi_bitbang&#45;&gt;modm_platform -->
<g id="edge12" class="edge">
<title>modm_platform_spi_bitbang&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1164.7253,-50.2604C1161.9712,-51.2611 1159.2176,-52.1844 1156.5,-53 1045.0428,-86.4514 908.2821,-100.0393 836.6318,-105.1685"/>
<polygon fill="#000000" stroke="#000000" points="836.2787,-101.6844 826.5436,-105.8652 836.7611,-108.6678 836.2787,-101.6844"/>
</g>
<!-- modm_platform_systick -->
<g id="node14" class="node">
<title>modm_platform_systick</title>
<g id="a_node14"><a xlink:href="../modm-platform-systick" xlink:title="modm:&#10;platform:&#10;systick">
<polygon fill="#d3d3d3" stroke="#000000" points="1363,-53 1280,-53 1280,0 1363,0 1363,-53"/>
<text text-anchor="middle" x="1321.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1321.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1321.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">systick</text>
</a>
</g>
</g>
<!-- modm_platform_systick&#45;&gt;modm_platform -->
<g id="edge13" class="edge">
<title>modm_platform_systick&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1279.9817,-49.9554C1277.1577,-51.097 1274.3155,-52.1283 1271.5,-53 1191.0706,-77.9022 942.9444,-97.3404 836.6527,-104.749"/>
<polygon fill="#000000" stroke="#000000" points="836.2526,-101.2682 826.5175,-105.4488 836.7349,-108.2515 836.2526,-101.2682"/>
</g>
<!-- modm_platform_timer -->
<g id="node15" class="node">
<title>modm_platform_timer</title>
<g id="a_node15"><a xlink:href="../modm-platform-timer" xlink:title="modm:&#10;platform:&#10;timer">
<polygon fill="#d3d3d3" stroke="#000000" points="1464,-53 1381,-53 1381,0 1464,0 1464,-53"/>
<text text-anchor="middle" x="1422.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1422.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1422.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">timer</text>
</a>
</g>
</g>
<!-- modm_platform_timer&#45;&gt;modm_platform -->
<g id="edge14" class="edge">
<title>modm_platform_timer&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1380.7183,-50.1316C1377.9825,-51.2125 1375.2297,-52.1838 1372.5,-53 1272.5474,-82.887 958.87,-100.126 836.7975,-105.8491"/>
<polygon fill="#000000" stroke="#000000" points="836.4495,-102.3614 826.6221,-106.3202 836.7733,-109.3539 836.4495,-102.3614"/>
</g>
<!-- modm_platform_uart -->
<g id="node16" class="node">
<title>modm_platform_uart</title>
<g id="a_node16"><a xlink:href="../modm-platform-uart" xlink:title="modm:&#10;platform:&#10;uart">
<polygon fill="#d3d3d3" stroke="#000000" points="1565,-53 1482,-53 1482,0 1565,0 1565,-53"/>
<text text-anchor="middle" x="1523.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1523.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1523.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">uart</text>
</a>
</g>
</g>
<!-- modm_platform_uart&#45;&gt;modm_platform -->
<g id="edge15" class="edge">
<title>modm_platform_uart&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1481.7313,-50.1759C1478.9923,-51.2457 1476.2352,-52.2026 1473.5,-53 1353.883,-87.8736 973.836,-102.4299 837.1067,-106.6268"/>
<polygon fill="#000000" stroke="#000000" points="836.8147,-103.1339 826.9248,-106.9339 837.0258,-110.1308 836.8147,-103.1339"/>
</g>
</g>
</svg>

