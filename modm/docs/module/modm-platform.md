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
class modm::platform::BitBangOneWireMaster< typename Pin  >;
class modm::platform::BitBangSpiMaster< typename Sck , typename Mosi , typename Miso =GpioUnused >;
class modm::platform::Can1;
class modm::platform::Can2;
class modm::platform::CanFilter;
class modm::platform::FaultReporter;
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
void modm_hardfault_entry();
// Enum
enum ErrorCode;
enum Peripheral;
// Typedef
typedef void(* modm::platform::InterruptHandler) (void))(void);
using modm::cortex::instead = typedef ::modm::platform::SysTickTimer;
using modm::platform::instead = typedef Rcc;
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:platform Pages: 1 -->
<svg width="1536pt" height="135pt"
 viewBox="0.00 0.00 1536.00 135.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 131)">
<title>modm:platform</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-131 1532,-131 1532,4 -4,4"/>
<!-- modm_platform -->
<g id="node1" class="node">
<title>modm_platform</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="802,-127 738,-127 738,-89 802,-89 802,-127"/>
<text text-anchor="middle" x="770" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="770" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform</text>
</g>
<!-- modm_platform_1_wire_bitbang -->
<g id="node2" class="node">
<title>modm_platform_1_wire_bitbang</title>
<g id="a_node2"><a xlink:href="../modm-platform-1-wire-bitbang" xlink:title="modm:&#10;platform:&#10;1&#45;wire.bitbang">
<polygon fill="#d3d3d3" stroke="#000000" points="96,-53 0,-53 0,0 96,0 96,-53"/>
<text text-anchor="middle" x="48" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="48" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="48" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">1&#45;wire.bitbang</text>
</a>
</g>
</g>
<!-- modm_platform_1_wire_bitbang&#45;&gt;modm_platform -->
<g id="edge1" class="edge">
<title>modm_platform_1_wire_bitbang&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M96.0744,-50.257C99.0566,-51.2898 102.0462,-52.2178 105,-53 223.4548,-84.366 600.2615,-101.4512 727.5347,-106.4384"/>
<polygon fill="#000000" stroke="#000000" points="727.5378,-109.941 737.6657,-106.8304 727.8085,-102.9463 727.5378,-109.941"/>
</g>
<!-- modm_platform_adc -->
<g id="node3" class="node">
<title>modm_platform_adc</title>
<g id="a_node3"><a xlink:href="../modm-platform-adc" xlink:title="modm:&#10;platform:&#10;adc">
<polygon fill="#d3d3d3" stroke="#000000" points="182,-53 114,-53 114,0 182,0 182,-53"/>
<text text-anchor="middle" x="148" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="148" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="148" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">adc</text>
</a>
</g>
</g>
<!-- modm_platform_adc&#45;&gt;modm_platform -->
<g id="edge2" class="edge">
<title>modm_platform_adc&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M182.0567,-49.3518C185.0052,-50.7511 188.0069,-51.9965 191,-53 290.684,-86.4225 611.3331,-101.864 727.485,-106.4617"/>
<polygon fill="#000000" stroke="#000000" points="727.624,-109.9696 737.7525,-106.8608 727.896,-102.9749 727.624,-109.9696"/>
</g>
<!-- modm_platform_can -->
<g id="node4" class="node">
<title>modm_platform_can</title>
<g id="a_node4"><a xlink:href="../modm-platform-can" xlink:title="modm:&#10;platform:&#10;can">
<polygon fill="#d3d3d3" stroke="#000000" points="268,-53 200,-53 200,0 268,0 268,-53"/>
<text text-anchor="middle" x="234" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="234" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="234" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">can</text>
</a>
</g>
</g>
<!-- modm_platform_can&#45;&gt;modm_platform -->
<g id="edge3" class="edge">
<title>modm_platform_can&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M268.0751,-49.2975C271.0196,-50.709 274.0152,-51.972 277,-53 360.1513,-81.6397 624.4111,-99.5884 727.88,-105.6761"/>
<polygon fill="#000000" stroke="#000000" points="727.7715,-109.1756 737.9576,-106.2613 728.1774,-102.1874 727.7715,-109.1756"/>
</g>
<!-- modm_platform_can_common -->
<g id="node5" class="node">
<title>modm_platform_can_common</title>
<g id="a_node5"><a xlink:href="../modm-platform-can-common" xlink:title="modm:&#10;platform:&#10;can.common">
<polygon fill="#d3d3d3" stroke="#000000" points="374,-53 286,-53 286,0 374,0 374,-53"/>
<text text-anchor="middle" x="330" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="330" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="330" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">can.common</text>
</a>
</g>
</g>
<!-- modm_platform_can_common&#45;&gt;modm_platform -->
<g id="edge4" class="edge">
<title>modm_platform_can_common&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M374.0986,-49.8445C377.0703,-51.0132 380.0538,-52.0808 383,-53 504.4861,-90.9011 655.6516,-102.7878 727.8253,-106.4351"/>
<polygon fill="#000000" stroke="#000000" points="727.7322,-109.9345 737.8877,-106.9155 728.0661,-102.9425 727.7322,-109.9345"/>
</g>
<!-- modm_platform_clock -->
<g id="node6" class="node">
<title>modm_platform_clock</title>
<g id="a_node6"><a xlink:href="../modm-platform-clock" xlink:title="modm:&#10;platform:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="460,-53 392,-53 392,0 460,0 460,-53"/>
<text text-anchor="middle" x="426" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="426" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="426" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_platform_clock&#45;&gt;modm_platform -->
<g id="edge5" class="edge">
<title>modm_platform_clock&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M460.1584,-49.0687C463.0842,-50.5315 466.0528,-51.8687 469,-53 557.1894,-86.8525 668.0946,-100.2131 727.6065,-105.2162"/>
<polygon fill="#000000" stroke="#000000" points="727.5064,-108.7194 737.7542,-106.0297 728.0658,-101.7417 727.5064,-108.7194"/>
</g>
<!-- modm_platform_core -->
<g id="node7" class="node">
<title>modm_platform_core</title>
<g id="a_node7"><a xlink:href="../modm-platform-core" xlink:title="modm:&#10;platform:&#10;core">
<polygon fill="#d3d3d3" stroke="#000000" points="546,-53 478,-53 478,0 546,0 546,-53"/>
<text text-anchor="middle" x="512" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="512" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="512" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">core</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_platform -->
<g id="edge6" class="edge">
<title>modm_platform_core&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M546.2497,-48.8427C549.155,-50.3562 552.094,-51.7667 555,-53 612.4596,-77.3866 683.3012,-92.9347 727.5739,-101.0643"/>
<polygon fill="#000000" stroke="#000000" points="727.2282,-104.558 737.6889,-102.8772 728.4632,-97.6678 727.2282,-104.558"/>
</g>
<!-- modm_platform_cortex_m -->
<g id="node8" class="node">
<title>modm_platform_cortex_m</title>
<g id="a_node8"><a xlink:href="../modm-platform-cortex-m" xlink:title="modm:&#10;platform:&#10;cortex&#45;m">
<polygon fill="#d3d3d3" stroke="#000000" points="632,-53 564,-53 564,0 632,0 632,-53"/>
<text text-anchor="middle" x="598" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="598" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="598" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">cortex&#45;m</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_platform -->
<g id="edge7" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M632.0011,-48.0892C635.0125,-49.8072 638.0407,-51.4682 641,-53 669.4706,-67.7376 702.7824,-81.8656 728.4226,-92.112"/>
<polygon fill="#000000" stroke="#000000" points="727.2615,-95.4166 737.8478,-95.8422 729.8375,-88.9078 727.2615,-95.4166"/>
</g>
<!-- modm_platform_fault -->
<g id="node9" class="node">
<title>modm_platform_fault</title>
<g id="a_node9"><a xlink:href="../modm-platform-fault" xlink:title="modm:&#10;platform:&#10;fault">
<polygon fill="#d3d3d3" stroke="#000000" points="718,-53 650,-53 650,0 718,0 718,-53"/>
<text text-anchor="middle" x="684" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="684" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="684" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">fault</text>
</a>
</g>
</g>
<!-- modm_platform_fault&#45;&gt;modm_platform -->
<g id="edge8" class="edge">
<title>modm_platform_fault&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M712.1596,-53.1861C721.823,-62.3439 732.6262,-72.5818 742.2563,-81.708"/>
<polygon fill="#000000" stroke="#000000" points="740.0179,-84.4087 749.6839,-88.7469 744.833,-79.3278 740.0179,-84.4087"/>
</g>
<!-- modm_platform_gpio -->
<g id="node10" class="node">
<title>modm_platform_gpio</title>
<g id="a_node10"><a xlink:href="../modm-platform-gpio" xlink:title="modm:&#10;platform:&#10;gpio">
<polygon fill="#d3d3d3" stroke="#000000" points="804,-53 736,-53 736,0 804,0 804,-53"/>
<text text-anchor="middle" x="770" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="770" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="770" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio</text>
</a>
</g>
</g>
<!-- modm_platform_gpio&#45;&gt;modm_platform -->
<g id="edge9" class="edge">
<title>modm_platform_gpio&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M770,-53.1861C770,-61.3465 770,-70.3646 770,-78.6895"/>
<polygon fill="#000000" stroke="#000000" points="766.5001,-78.7469 770,-88.7469 773.5001,-78.747 766.5001,-78.7469"/>
</g>
<!-- modm_platform_gpio_common -->
<g id="node11" class="node">
<title>modm_platform_gpio_common</title>
<g id="a_node11"><a xlink:href="../modm-platform-gpio-common" xlink:title="modm:&#10;platform:&#10;gpio.common">
<polygon fill="#d3d3d3" stroke="#000000" points="915.5,-53 822.5,-53 822.5,0 915.5,0 915.5,-53"/>
<text text-anchor="middle" x="869" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="869" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="869" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio.common</text>
</a>
</g>
</g>
<!-- modm_platform_gpio_common&#45;&gt;modm_platform -->
<g id="edge10" class="edge">
<title>modm_platform_gpio_common&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M836.5837,-53.1861C825.2393,-62.5253 812.5303,-72.9877 801.2805,-82.2489"/>
<polygon fill="#000000" stroke="#000000" points="798.8831,-79.689 793.3872,-88.7469 803.3321,-85.0934 798.8831,-79.689"/>
</g>
<!-- modm_platform_i2c -->
<g id="node12" class="node">
<title>modm_platform_i2c</title>
<g id="a_node12"><a xlink:href="../modm-platform-i2c" xlink:title="modm:&#10;platform:&#10;i2c">
<polygon fill="#d3d3d3" stroke="#000000" points="1002,-53 934,-53 934,0 1002,0 1002,-53"/>
<text text-anchor="middle" x="968" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="968" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="968" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c</text>
</a>
</g>
</g>
<!-- modm_platform_i2c&#45;&gt;modm_platform -->
<g id="edge11" class="edge">
<title>modm_platform_i2c&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M933.62,-48.5541C930.7439,-50.1323 927.8472,-51.6364 925,-53 888.0521,-70.6951 843.8339,-85.8312 812.0997,-95.7188"/>
<polygon fill="#000000" stroke="#000000" points="810.7915,-92.4591 802.2601,-98.7412 812.847,-99.1506 810.7915,-92.4591"/>
</g>
<!-- modm_platform_rcc -->
<g id="node13" class="node">
<title>modm_platform_rcc</title>
<g id="a_node13"><a xlink:href="../modm-platform-rcc" xlink:title="modm:&#10;platform:&#10;rcc">
<polygon fill="#d3d3d3" stroke="#000000" points="1088,-53 1020,-53 1020,0 1088,0 1088,-53"/>
<text text-anchor="middle" x="1054" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1054" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1054" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">rcc</text>
</a>
</g>
</g>
<!-- modm_platform_rcc&#45;&gt;modm_platform -->
<g id="edge12" class="edge">
<title>modm_platform_rcc&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1019.7854,-48.9269C1016.8722,-50.4214 1013.9218,-51.8046 1011,-53 944.251,-80.3079 861.2865,-95.464 812.1578,-102.6548"/>
<polygon fill="#000000" stroke="#000000" points="811.6135,-99.1969 802.2058,-104.0698 812.599,-106.1272 811.6135,-99.1969"/>
</g>
<!-- modm_platform_spi -->
<g id="node14" class="node">
<title>modm_platform_spi</title>
<g id="a_node14"><a xlink:href="../modm-platform-spi" xlink:title="modm:&#10;platform:&#10;spi">
<polygon fill="#d3d3d3" stroke="#000000" points="1174,-53 1106,-53 1106,0 1174,0 1174,-53"/>
<text text-anchor="middle" x="1140" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1140" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1140" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi</text>
</a>
</g>
</g>
<!-- modm_platform_spi&#45;&gt;modm_platform -->
<g id="edge13" class="edge">
<title>modm_platform_spi&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1105.8591,-49.1148C1102.9294,-50.5673 1099.9551,-51.8895 1097,-53 999.2452,-89.736 875.8375,-102.026 812.216,-106.0692"/>
<polygon fill="#000000" stroke="#000000" points="811.9344,-102.5797 802.1612,-106.6706 812.3524,-109.5672 811.9344,-102.5797"/>
</g>
<!-- modm_platform_spi_bitbang -->
<g id="node15" class="node">
<title>modm_platform_spi_bitbang</title>
<g id="a_node15"><a xlink:href="../modm-platform-spi-bitbang" xlink:title="modm:&#10;platform:&#10;spi.bitbang">
<polygon fill="#d3d3d3" stroke="#000000" points="1269.5,-53 1192.5,-53 1192.5,0 1269.5,0 1269.5,-53"/>
<text text-anchor="middle" x="1231" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1231" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1231" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi.bitbang</text>
</a>
</g>
</g>
<!-- modm_platform_spi_bitbang&#45;&gt;modm_platform -->
<g id="edge14" class="edge">
<title>modm_platform_spi_bitbang&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1192.2497,-49.436C1189.1777,-50.7753 1186.0738,-51.9868 1183,-53 1114.6879,-75.518 903.2643,-96.2424 812.3109,-104.3701"/>
<polygon fill="#000000" stroke="#000000" points="811.7423,-100.9067 802.0902,-105.2756 812.3601,-107.8794 811.7423,-100.9067"/>
</g>
<!-- modm_platform_systick -->
<g id="node16" class="node">
<title>modm_platform_systick</title>
<g id="a_node16"><a xlink:href="../modm-platform-systick" xlink:title="modm:&#10;platform:&#10;systick">
<polygon fill="#d3d3d3" stroke="#000000" points="1356,-53 1288,-53 1288,0 1356,0 1356,-53"/>
<text text-anchor="middle" x="1322" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1322" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1322" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">systick</text>
</a>
</g>
</g>
<!-- modm_platform_systick&#45;&gt;modm_platform -->
<g id="edge15" class="edge">
<title>modm_platform_systick&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1287.9288,-49.309C1284.9835,-50.7179 1281.9866,-51.9771 1279,-53 1192.7398,-82.5434 917.8122,-100.059 812.0326,-105.8501"/>
<polygon fill="#000000" stroke="#000000" points="811.8379,-102.3554 802.0412,-106.3897 812.2154,-109.3453 811.8379,-102.3554"/>
</g>
<!-- modm_platform_timer -->
<g id="node17" class="node">
<title>modm_platform_timer</title>
<g id="a_node17"><a xlink:href="../modm-platform-timer" xlink:title="modm:&#10;platform:&#10;timer">
<polygon fill="#d3d3d3" stroke="#000000" points="1442,-53 1374,-53 1374,0 1442,0 1442,-53"/>
<text text-anchor="middle" x="1408" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1408" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1408" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">timer</text>
</a>
</g>
</g>
<!-- modm_platform_timer&#45;&gt;modm_platform -->
<g id="edge16" class="edge">
<title>modm_platform_timer&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1373.9461,-49.3602C1370.9969,-50.7576 1367.9944,-52.0002 1365,-53 1262.0808,-87.3623 930.0723,-102.2913 812.0883,-106.6023"/>
<polygon fill="#000000" stroke="#000000" points="811.8708,-103.1077 802.0026,-106.9638 812.1217,-110.1032 811.8708,-103.1077"/>
</g>
<!-- modm_platform_uart -->
<g id="node18" class="node">
<title>modm_platform_uart</title>
<g id="a_node18"><a xlink:href="../modm-platform-uart" xlink:title="modm:&#10;platform:&#10;uart">
<polygon fill="#d3d3d3" stroke="#000000" points="1528,-53 1460,-53 1460,0 1528,0 1528,-53"/>
<text text-anchor="middle" x="1494" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1494" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="1494" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">uart</text>
</a>
</g>
</g>
<!-- modm_platform_uart&#45;&gt;modm_platform -->
<g id="edge17" class="edge">
<title>modm_platform_uart&#45;&gt;modm_platform</title>
<path fill="none" stroke="#000000" d="M1459.9589,-49.3987C1457.0068,-50.7875 1454.0001,-52.0176 1451,-53 1331.3528,-92.1784 941.8671,-104.2002 812.3295,-107.1636"/>
<polygon fill="#000000" stroke="#000000" points="811.9525,-103.671 802.0327,-107.3925 812.1081,-110.6693 811.9525,-103.671"/>
</g>
</g>
</svg>

