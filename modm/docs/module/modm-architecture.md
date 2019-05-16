!!! warning "These module docs are in beta and may be incomplete."

# modm:architecture: Architecture Interfaces

All hardware peripherals with common interfaces.

## Content

```cpp
// Class
class modm::Adc;
class modm::AdcInterrupt;
class modm::BlockAllocator< typename T , unsigned int BLOCK_SIZE >;
class modm::BlockDevice;
class modm::Can;
class modm::Clock;
class modm::GpioExpander;
class modm::GpioExpanderPin< typename GpioExpander , GpioExpander & expander, typename GpioExpander::Pin pin >;
class modm::GpioExpanderPort< typename GpioExpander , GpioExpander & expander, typename GpioExpander::Pin StartPin, uint8_t Width, GpioPort::DataOrder DataOrder=GpioPort::DataOrder::Normal >;
class modm::GpioIO;
class modm::GpioInput;
class modm::GpioOutput;
class modm::GpioPort;
class modm::I2cDevice< class I2cMaster, uint8_t NestingLevels=10, class Transaction=I2cWriteReadTransaction >;
class modm::I2cMaster;
class modm::I2cMultiplexer< class I2cMaster , class I2cMultiplexerDevice  >;
class modm::I2cMultiplexerChannel< auto & multiplexer, typename std::remove_reference_t< decltype(multiplexer)>::Channel channel >;
class modm::I2cReadTransaction;
class modm::I2cTransaction;
class modm::I2cWriteReadTransaction;
class modm::I2cWriteTransaction;
class modm::PeripheralDriver;
class modm::SpiDevice< class SpiMaster  >;
class modm::SpiMaster;
class modm::Uart;
class modm::accessor::Flash< typename T >;
class modm::accessor::Ram< typename T >;
class modm::atomic::Container< typename T >;
class modm::atomic::Flag;
class modm::atomic::Lock;
class modm::atomic::Queue< typename T , std::size_t N >;
class modm::atomic::Unlock;
// Struct
struct modm::Configuration< typename Parent , typename Enum , typename Parent::UnderlyingType Mask, typename Parent::UnderlyingType Position=0 >;
struct modm::Flags< typename Enum , typename T  >;
struct modm::FlagsGroup< T... >;
struct modm::FlagsOperators< typename Enum, typename T >;
struct modm::Gpio;
struct modm::I2c;
struct modm::I2cTransaction::Reading;
struct modm::I2cTransaction::Starting;
struct modm::I2cTransaction::Writing;
struct modm::Register< typename T >;
struct modm::Spi;
struct modm::Value< typename Parent , typename Parent::UnderlyingType Width, typename Parent::UnderlyingType Position=0 >;
struct modm::can::Message::Flags;
struct modm::can::Message;
struct modm::i2cMultiplexer;
struct modm::unaligned_t< typename T  >;
// Function
bool modm_assert(bool condition, const char *module, const char *location, const char *failure);
bool modm_assert(bool condition, const char *module, const char *location, const char *failure, uintptr_t context);
bool modm_assert_debug(bool condition, const char *module, const char *location, const char *failure);
bool modm_assert_debug(bool condition, const char *module, const char *location, const char *failure, uintptr_t context);
inline ::modm::accessor::Flash<T> modm::accessor::asFlash(const T *ptr);
modm::IOStream& modm::can::operator<<(modm::IOStream &s, const modm::can::Message m);
modm::IOStream& modm::operator<<(modm::IOStream &s, const modm::I2c::Operation op);
modm::IOStream& modm::operator<<(modm::IOStream &s, const modm::I2c::OperationAfterRead op);
modm::IOStream& modm::operator<<(modm::IOStream &s, const modm::I2c::OperationAfterStart op);
modm::IOStream& modm::operator<<(modm::IOStream &s, const modm::I2c::OperationAfterWrite op);
modm::IOStream& modm::operator<<(modm::IOStream &stream, const Can::BusState state);
modm::IOStream& operator<<(modm::IOStream &os, modm::accessor::Flash< T > ptr);
static modm::IOStream& operator<<(modm::IOStream &os, modm::accessor::Flash< char > ptr);
unaligned_t<T>* modm::asUnaligned(U *value);
void modm::delayMicroseconds(uint16_t us);
void modm::delayMilliseconds(uint16_t ms);
void modm::delayNanoseconds(uint16_t ns);
void modm_abandon(const char *module, const char *location, const char *failure, uintptr_t context) modm_weak;
void* operator new(size_t size, modm::MemoryTraits traits) noexcept;
void* operator new[](size_t size, modm::MemoryTraits traits) noexcept;
volatile T& modm::accessor::asVolatile(T &value);
// Enum
enum RomCommand;
enum class Abandonment;
enum class MemoryTrait;
// Variable
constexpr MemoryTraits modm::MemoryBackup = (MemoryTrait::AccessSBus | MemoryTrait::TypeNonVolatile);
constexpr MemoryTraits modm::MemoryDMA = (MemoryTrait::AccessSBus | MemoryTrait::AccessDMA);
constexpr MemoryTraits modm::MemoryDMA2D = (MemoryTrait::AccessSBus | MemoryTrait::AccessDMA2D);
constexpr MemoryTraits modm::MemoryDefault = MemoryDMA;
constexpr MemoryTraits modm::MemoryExternal = (MemoryTrait::AccessSBus | MemoryTrait::TypeExternal);
constexpr MemoryTraits modm::MemoryFastCode = (MemoryTrait::AccessIBus | MemoryTrait::TypeCoreCoupled);
constexpr MemoryTraits modm::MemoryFastData = (MemoryTrait::AccessDBus | MemoryTrait::TypeCoreCoupled);
// Typedef
using modm::AssertionHandler = typedef Abandonment (*)(const char * module, const char * location, const char * failure, uintptr_t context);
using modm::Flags16 = typedef Flags<Enum, uint16_t>;
using modm::Flags32 = typedef Flags<Enum, uint32_t>;
using modm::Flags8 = typedef Flags<Enum, uint8_t>;
using modm::MemoryTraits = typedef Flags16<MemoryTrait>;
using modm::Register16 = typedef Register<uint16_t>;
using modm::Register32 = typedef Register<uint32_t>;
using modm::Register8 = typedef Register<uint8_t>;
// Define
#define EXTERN_FLASH_STORAGE(var)
#define EXTERN_FLASH_STORAGE_STRING(s)
#define FLASH_STORAGE(var)
#define FLASH_STORAGE_STRING(s)
#define IFSS(s)
#define MODM_ALIGNMENT
#define MODM_ARRAY_SIZE(x)
#define MODM_ASSERTION_HANDLER(handler)
#define MODM_BYTE_ORDER
#define MODM_COMPILER_GCC
#define MODM_COMPILER_MSVC
#define MODM_COMPILER_STRING
#define MODM_CONCAT(a, b)
#define MODM_CONCAT3(a, b, c)
#define MODM_CONCAT4(a, b, c, d)
#define MODM_CONCAT5(a, b, c, d, e)
#define MODM_CPU_ARM
#define MODM_CPU_ARM7TDMI
#define MODM_CPU_ATMEGA
#define MODM_CPU_ATTINY
#define MODM_CPU_ATXMEGA
#define MODM_CPU_AVR
#define MODM_CPU_CORTEX_M0
#define MODM_CPU_CORTEX_M3
#define MODM_CPU_CORTEX_M4
#define MODM_CPU_STRING
#define MODM_FLAGS16(Enum)
#define MODM_FLAGS32(Enum)
#define MODM_FLAGS8(Enum)
#define MODM_ISR(vector, ...)
#define MODM_ISR_CALL(vector)
#define MODM_ISR_DECL(vector)
#define MODM_ISR_NAME(vector)
#define MODM_IS_BIG_ENDIAN
#define MODM_IS_LITTLE_ENDIAN
#define MODM_ORDER_BIG_ENDIAN
#define MODM_ORDER_LITTLE_ENDIAN
#define MODM_ORDER_PDP_ENDIAN
#define MODM_OS_HOSTED
#define MODM_OS_HOSTED_64
#define MODM_OS_LINUX
#define MODM_OS_OSX
#define MODM_OS_STRING
#define MODM_OS_UNIX
#define MODM_OS_WIN32
#define MODM_OS_WIN64
#define MODM_SIZEOF_POINTER
#define MODM_STRINGIFY(s)
#define MODM_TYPE_FLAGS(Parent)
#define modm_aligned(n)
#define modm_always_inline
#define modm_fastcode
#define modm_fastdata
#define modm_likely(x)
#define modm_packed
#define modm_section(s)
#define modm_unlikely(x)
#define modm_unused
#define modm_weak
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (0)
 -->
<!-- Title: modm:architecture Pages: 1 -->
<svg width="3492pt" height="224pt"
 viewBox="0.00 0.00 3491.50 224.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 220)">
<title>modm:architecture</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-220 3487.5,-220 3487.5,4 -4,4"/>
<!-- modm_architecture -->
<g id="node1" class="node">
<title>modm_architecture</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="1918,-127 1812,-127 1812,-89 1918,-89 1918,-127"/>
<text text-anchor="middle" x="1865" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1865" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture</text>
</g>
<!-- modm_math_units -->
<g id="node2" class="node">
<title>modm_math_units</title>
<g id="a_node2"><a xlink:href="../modm-math-units" xlink:title="modm:&#10;math:&#10;units">
<polygon fill="#d3d3d3" stroke="#000000" points="1897.5,-216 1832.5,-216 1832.5,-163 1897.5,-163 1897.5,-216"/>
<text text-anchor="middle" x="1865" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1865" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="1865" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">units</text>
</a>
</g>
</g>
<!-- modm_architecture&#45;&gt;modm_math_units -->
<g id="edge1" class="edge">
<title>modm_architecture&#45;&gt;modm_math_units</title>
<path fill="none" stroke="#000000" d="M1859.1601,-127.3144C1858.4434,-134.9606 1858.15,-144.0143 1858.2799,-152.8129"/>
<polygon fill="#000000" stroke="#000000" points="1854.7859,-153.0508 1858.6269,-162.9249 1861.7818,-152.8107 1854.7859,-153.0508"/>
</g>
<!-- modm_math_units&#45;&gt;modm_architecture -->
<g id="edge28" class="edge">
<title>modm_math_units&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1871.3731,-162.9249C1871.8243,-154.762 1871.8704,-145.7322 1871.5114,-137.3922"/>
<polygon fill="#000000" stroke="#000000" points="1874.997,-137.0595 1870.8399,-127.3144 1868.0125,-137.5249 1874.997,-137.0595"/>
</g>
<!-- modm_architecture_1_wire -->
<g id="node3" class="node">
<title>modm_architecture_1_wire</title>
<g id="a_node3"><a xlink:href="../modm-architecture-1-wire" xlink:title="modm:&#10;architecture:&#10;1&#45;wire">
<polygon fill="#d3d3d3" stroke="#000000" points="110,-53 0,-53 0,0 110,0 110,-53"/>
<text text-anchor="middle" x="55" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="55" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="55" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">1&#45;wire</text>
</a>
</g>
</g>
<!-- modm_architecture_1_wire&#45;&gt;modm_architecture -->
<g id="edge2" class="edge">
<title>modm_architecture_1_wire&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M110.1088,-50.7457C113.0854,-51.6055 116.0608,-52.3673 119,-53 285.7406,-88.8935 1507.672,-104.175 1801.5623,-107.3533"/>
<polygon fill="#000000" stroke="#000000" points="1801.5851,-110.8537 1811.6222,-107.4613 1801.6603,-103.8541 1801.5851,-110.8537"/>
</g>
<!-- modm_architecture_accessor -->
<g id="node4" class="node">
<title>modm_architecture_accessor</title>
<g id="a_node4"><a xlink:href="../modm-architecture-accessor" xlink:title="modm:&#10;architecture:&#10;accessor">
<polygon fill="#d3d3d3" stroke="#000000" points="238,-53 128,-53 128,0 238,0 238,-53"/>
<text text-anchor="middle" x="183" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="183" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="183" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">accessor</text>
</a>
</g>
</g>
<!-- modm_architecture_accessor&#45;&gt;modm_architecture -->
<g id="edge3" class="edge">
<title>modm_architecture_accessor&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M238.1107,-50.7368C241.0868,-51.5988 244.0616,-52.3636 247,-53 400.5724,-86.2589 1520.4752,-103.3806 1801.28,-107.1793"/>
<polygon fill="#000000" stroke="#000000" points="1801.646,-110.6845 1811.6921,-107.3193 1801.7401,-103.6851 1801.646,-110.6845"/>
</g>
<!-- modm_architecture_adc -->
<g id="node5" class="node">
<title>modm_architecture_adc</title>
<g id="a_node5"><a xlink:href="../modm-architecture-adc" xlink:title="modm:&#10;architecture:&#10;adc">
<polygon fill="#d3d3d3" stroke="#000000" points="366,-53 256,-53 256,0 366,0 366,-53"/>
<text text-anchor="middle" x="311" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="311" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="311" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">adc</text>
</a>
</g>
</g>
<!-- modm_architecture_adc&#45;&gt;modm_architecture -->
<g id="edge4" class="edge">
<title>modm_architecture_adc&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M366.113,-50.7263C369.0885,-51.5911 372.0626,-52.3594 375,-53 515.5433,-83.6519 1535.3573,-102.5401 1801.5731,-106.9841"/>
<polygon fill="#000000" stroke="#000000" points="1801.7786,-110.4879 1811.8354,-107.1545 1801.8949,-103.4889 1801.7786,-110.4879"/>
</g>
<!-- modm_architecture_assert -->
<g id="node6" class="node">
<title>modm_architecture_assert</title>
<g id="a_node6"><a xlink:href="../modm-architecture-assert" xlink:title="modm:&#10;architecture:&#10;assert">
<polygon fill="#d3d3d3" stroke="#000000" points="494,-53 384,-53 384,0 494,0 494,-53"/>
<text text-anchor="middle" x="439" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="439" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="439" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">assert</text>
</a>
</g>
</g>
<!-- modm_architecture_assert&#45;&gt;modm_architecture -->
<g id="edge5" class="edge">
<title>modm_architecture_assert&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M494.1157,-50.7139C497.0905,-51.5819 500.0637,-52.3543 503,-53 630.5198,-81.0429 1550.3761,-101.6101 1801.6946,-106.7502"/>
<polygon fill="#000000" stroke="#000000" points="1801.687,-110.2506 1811.7562,-106.955 1801.8295,-103.2521 1801.687,-110.2506"/>
</g>
<!-- modm_architecture_atomic -->
<g id="node7" class="node">
<title>modm_architecture_atomic</title>
<g id="a_node7"><a xlink:href="../modm-architecture-atomic" xlink:title="modm:&#10;architecture:&#10;atomic">
<polygon fill="#d3d3d3" stroke="#000000" points="622,-53 512,-53 512,0 622,0 622,-53"/>
<text text-anchor="middle" x="567" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="567" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="567" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">atomic</text>
</a>
</g>
</g>
<!-- modm_architecture_atomic&#45;&gt;modm_architecture -->
<g id="edge6" class="edge">
<title>modm_architecture_atomic&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M622.119,-50.6989C625.093,-51.5709 628.065,-52.3481 631,-53 745.4858,-78.4277 1565.3164,-100.5582 1801.4883,-106.4604"/>
<polygon fill="#000000" stroke="#000000" points="1801.546,-109.9628 1811.63,-106.7128 1801.7202,-102.965 1801.546,-109.9628"/>
</g>
<!-- modm_architecture_block_device -->
<g id="node8" class="node">
<title>modm_architecture_block_device</title>
<g id="a_node8"><a xlink:href="../modm-architecture-block-device" xlink:title="modm:&#10;architecture:&#10;block.device">
<polygon fill="#d3d3d3" stroke="#000000" points="750,-53 640,-53 640,0 750,0 750,-53"/>
<text text-anchor="middle" x="695" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="695" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="695" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">block.device</text>
</a>
</g>
</g>
<!-- modm_architecture_block_device&#45;&gt;modm_architecture -->
<g id="edge7" class="edge">
<title>modm_architecture_block_device&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M750.1231,-50.6805C753.096,-51.5572 756.0667,-52.3406 759,-53 959.7267,-98.1244 1597.8891,-106.2402 1801.7476,-107.6884"/>
<polygon fill="#000000" stroke="#000000" points="1801.959,-111.1898 1811.9826,-107.7581 1802.0067,-104.19 1801.959,-111.1898"/>
</g>
<!-- modm_architecture_can -->
<g id="node9" class="node">
<title>modm_architecture_can</title>
<g id="a_node9"><a xlink:href="../modm-architecture-can" xlink:title="modm:&#10;architecture:&#10;can">
<polygon fill="#d3d3d3" stroke="#000000" points="878,-53 768,-53 768,0 878,0 878,-53"/>
<text text-anchor="middle" x="823" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="823" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="823" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">can</text>
</a>
</g>
</g>
<!-- modm_architecture_can&#45;&gt;modm_architecture -->
<g id="edge8" class="edge">
<title>modm_architecture_can&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M878.1284,-50.6572C881.0999,-51.5401 884.0689,-52.3311 887,-53 1062.0266,-92.944 1613.582,-104.412 1801.5371,-107.2048"/>
<polygon fill="#000000" stroke="#000000" points="1801.7525,-110.7082 1811.8024,-107.354 1801.8544,-103.7089 1801.7525,-110.7082"/>
</g>
<!-- modm_architecture_clock -->
<g id="node10" class="node">
<title>modm_architecture_clock</title>
<g id="a_node10"><a xlink:href="../modm-architecture-clock" xlink:title="modm:&#10;architecture:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="1006,-53 896,-53 896,0 1006,0 1006,-53"/>
<text text-anchor="middle" x="951" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="951" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="951" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_architecture_clock&#45;&gt;modm_architecture -->
<g id="edge9" class="edge">
<title>modm_architecture_clock&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1006.1353,-50.627C1009.105,-51.5178 1012.0717,-52.3187 1015,-53 1164.6166,-87.8098 1631.2079,-102.3435 1801.756,-106.5851"/>
<polygon fill="#000000" stroke="#000000" points="1801.7494,-110.0859 1811.8323,-106.8322 1801.9211,-103.088 1801.7494,-110.0859"/>
</g>
<!-- modm_architecture_delay -->
<g id="node11" class="node">
<title>modm_architecture_delay</title>
<g id="a_node11"><a xlink:href="../modm-architecture-delay" xlink:title="modm:&#10;architecture:&#10;delay">
<polygon fill="#d3d3d3" stroke="#000000" points="1134,-53 1024,-53 1024,0 1134,0 1134,-53"/>
<text text-anchor="middle" x="1079" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1079" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1079" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">delay</text>
</a>
</g>
</g>
<!-- modm_architecture_delay&#45;&gt;modm_architecture -->
<g id="edge10" class="edge">
<title>modm_architecture_delay&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1134.145,-50.5862C1137.1121,-51.4876 1140.0757,-52.302 1143,-53 1267.3104,-82.6709 1649.6156,-99.9054 1801.6715,-105.7413"/>
<polygon fill="#000000" stroke="#000000" points="1801.6565,-109.2432 1811.7822,-106.1256 1801.9224,-102.2482 1801.6565,-109.2432"/>
</g>
<!-- modm_architecture_gpio -->
<g id="node12" class="node">
<title>modm_architecture_gpio</title>
<g id="a_node12"><a xlink:href="../modm-architecture-gpio" xlink:title="modm:&#10;architecture:&#10;gpio">
<polygon fill="#d3d3d3" stroke="#000000" points="1262,-53 1152,-53 1152,0 1262,0 1262,-53"/>
<text text-anchor="middle" x="1207" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1207" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1207" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio</text>
</a>
</g>
</g>
<!-- modm_architecture_gpio&#45;&gt;modm_architecture -->
<g id="edge11" class="edge">
<title>modm_architecture_gpio&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1262.1591,-50.5281C1265.1226,-51.4447 1268.0814,-52.2782 1271,-53 1370.2322,-77.5416 1669.6249,-96.9382 1801.4823,-104.5319"/>
<polygon fill="#000000" stroke="#000000" points="1801.6355,-108.0462 1811.819,-105.1227 1802.0351,-101.0577 1801.6355,-108.0462"/>
</g>
<!-- modm_architecture_gpio_expander -->
<g id="node13" class="node">
<title>modm_architecture_gpio_expander</title>
<g id="a_node13"><a xlink:href="../modm-architecture-gpio-expander" xlink:title="modm:&#10;architecture:&#10;gpio.expander">
<polygon fill="#d3d3d3" stroke="#000000" points="1399.5,-53 1280.5,-53 1280.5,0 1399.5,0 1399.5,-53"/>
<text text-anchor="middle" x="1340" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1340" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1340" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio.expander</text>
</a>
</g>
</g>
<!-- modm_architecture_gpio_expander&#45;&gt;modm_architecture -->
<g id="edge12" class="edge">
<title>modm_architecture_gpio_expander&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1399.5491,-50.4029C1402.723,-51.3522 1405.8865,-52.227 1409,-53 1546.5339,-87.1472 1712.8662,-100.3896 1801.7352,-105.2955"/>
<polygon fill="#000000" stroke="#000000" points="1801.7477,-108.8011 1811.9203,-105.8398 1802.1214,-101.8111 1801.7477,-108.8011"/>
</g>
<!-- modm_architecture_heap -->
<g id="node14" class="node">
<title>modm_architecture_heap</title>
<g id="a_node14"><a xlink:href="../modm-architecture-heap" xlink:title="modm:&#10;architecture:&#10;heap">
<polygon fill="#d3d3d3" stroke="#000000" points="1528,-53 1418,-53 1418,0 1528,0 1528,-53"/>
<text text-anchor="middle" x="1473" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1473" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1473" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">heap</text>
</a>
</g>
</g>
<!-- modm_architecture_heap&#45;&gt;modm_architecture -->
<g id="edge13" class="edge">
<title>modm_architecture_heap&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1528.229,-50.2662C1531.1742,-51.2512 1534.1101,-52.171 1537,-53 1627.2728,-78.8952 1735.2403,-94.0872 1801.977,-101.7235"/>
<polygon fill="#000000" stroke="#000000" points="1801.5927,-105.2023 1811.9208,-102.8402 1802.3739,-98.246 1801.5927,-105.2023"/>
</g>
<!-- modm_architecture_i2c -->
<g id="node15" class="node">
<title>modm_architecture_i2c</title>
<g id="a_node15"><a xlink:href="../modm-architecture-i2c" xlink:title="modm:&#10;architecture:&#10;i2c">
<polygon fill="#d3d3d3" stroke="#000000" points="1656,-53 1546,-53 1546,0 1656,0 1656,-53"/>
<text text-anchor="middle" x="1601" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1601" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1601" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c&#45;&gt;modm_architecture -->
<g id="edge14" class="edge">
<title>modm_architecture_i2c&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1656.3417,-49.9082C1659.2575,-50.9867 1662.1562,-52.0244 1665,-53 1710.2182,-68.512 1762.32,-82.7258 1802.0072,-92.8166"/>
<polygon fill="#000000" stroke="#000000" points="1801.2656,-96.2391 1811.8179,-95.2915 1802.9778,-89.4518 1801.2656,-96.2391"/>
</g>
<!-- modm_architecture_i2c_device -->
<g id="node16" class="node">
<title>modm_architecture_i2c_device</title>
<g id="a_node16"><a xlink:href="../modm-architecture-i2c-device" xlink:title="modm:&#10;architecture:&#10;i2c.device">
<polygon fill="#d3d3d3" stroke="#000000" points="1784,-53 1674,-53 1674,0 1784,0 1784,-53"/>
<text text-anchor="middle" x="1729" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1729" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1729" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.device</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_device&#45;&gt;modm_architecture -->
<g id="edge15" class="edge">
<title>modm_architecture_i2c_device&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1773.5315,-53.1861C1789.8723,-62.9786 1808.2739,-74.0061 1824.2648,-83.5888"/>
<polygon fill="#000000" stroke="#000000" points="1822.4953,-86.6088 1832.8722,-88.7469 1826.0936,-80.6044 1822.4953,-86.6088"/>
</g>
<!-- modm_architecture_i2c_multiplexer -->
<g id="node17" class="node">
<title>modm_architecture_i2c_multiplexer</title>
<g id="a_node17"><a xlink:href="../modm-architecture-i2c-multiplexer" xlink:title="modm:&#10;architecture:&#10;i2c.multiplexer">
<polygon fill="#d3d3d3" stroke="#000000" points="1927.5,-53 1802.5,-53 1802.5,0 1927.5,0 1927.5,-53"/>
<text text-anchor="middle" x="1865" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1865" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1865" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.multiplexer</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_multiplexer&#45;&gt;modm_architecture -->
<g id="edge16" class="edge">
<title>modm_architecture_i2c_multiplexer&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1865,-53.1861C1865,-61.3465 1865,-70.3646 1865,-78.6895"/>
<polygon fill="#000000" stroke="#000000" points="1861.5001,-78.7469 1865,-88.7469 1868.5001,-78.747 1861.5001,-78.7469"/>
</g>
<!-- modm_architecture_interrupt -->
<g id="node18" class="node">
<title>modm_architecture_interrupt</title>
<g id="a_node18"><a xlink:href="../modm-architecture-interrupt" xlink:title="modm:&#10;architecture:&#10;interrupt">
<polygon fill="#d3d3d3" stroke="#000000" points="2056,-53 1946,-53 1946,0 2056,0 2056,-53"/>
<text text-anchor="middle" x="2001" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2001" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2001" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">interrupt</text>
</a>
</g>
</g>
<!-- modm_architecture_interrupt&#45;&gt;modm_architecture -->
<g id="edge17" class="edge">
<title>modm_architecture_interrupt&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1956.4685,-53.1861C1940.1277,-62.9786 1921.7261,-74.0061 1905.7352,-83.5888"/>
<polygon fill="#000000" stroke="#000000" points="1903.9064,-80.6044 1897.1278,-88.7469 1907.5047,-86.6088 1903.9064,-80.6044"/>
</g>
<!-- modm_architecture_memory -->
<g id="node19" class="node">
<title>modm_architecture_memory</title>
<g id="a_node19"><a xlink:href="../modm-architecture-memory" xlink:title="modm:&#10;architecture:&#10;memory">
<polygon fill="#d3d3d3" stroke="#000000" points="2184,-53 2074,-53 2074,0 2184,0 2184,-53"/>
<text text-anchor="middle" x="2129" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2129" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2129" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">memory</text>
</a>
</g>
</g>
<!-- modm_architecture_memory&#45;&gt;modm_architecture -->
<g id="edge18" class="edge">
<title>modm_architecture_memory&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2073.6583,-49.9082C2070.7425,-50.9867 2067.8438,-52.0244 2065,-53 2019.7818,-68.512 1967.68,-82.7258 1927.9928,-92.8166"/>
<polygon fill="#000000" stroke="#000000" points="1927.0222,-89.4518 1918.1821,-95.2915 1928.7344,-96.2391 1927.0222,-89.4518"/>
</g>
<!-- modm_architecture_register -->
<g id="node20" class="node">
<title>modm_architecture_register</title>
<g id="a_node20"><a xlink:href="../modm-architecture-register" xlink:title="modm:&#10;architecture:&#10;register">
<polygon fill="#d3d3d3" stroke="#000000" points="2312,-53 2202,-53 2202,0 2312,0 2312,-53"/>
<text text-anchor="middle" x="2257" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2257" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2257" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">register</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_architecture -->
<g id="edge19" class="edge">
<title>modm_architecture_register&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2201.771,-50.2662C2198.8258,-51.2512 2195.8899,-52.171 2193,-53 2102.7272,-78.8952 1994.7597,-94.0872 1928.023,-101.7235"/>
<polygon fill="#000000" stroke="#000000" points="1927.6261,-98.246 1918.0792,-102.8402 1928.4073,-105.2023 1927.6261,-98.246"/>
</g>
<!-- modm_architecture_spi -->
<g id="node21" class="node">
<title>modm_architecture_spi</title>
<g id="a_node21"><a xlink:href="../modm-architecture-spi" xlink:title="modm:&#10;architecture:&#10;spi">
<polygon fill="#d3d3d3" stroke="#000000" points="2440,-53 2330,-53 2330,0 2440,0 2440,-53"/>
<text text-anchor="middle" x="2385" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2385" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2385" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi</text>
</a>
</g>
</g>
<!-- modm_architecture_spi&#45;&gt;modm_architecture -->
<g id="edge20" class="edge">
<title>modm_architecture_spi&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2329.8158,-50.4295C2326.8589,-51.3718 2323.9083,-52.2378 2321,-53 2183.9195,-88.9241 2017.4111,-101.4764 1928.392,-105.794"/>
<polygon fill="#000000" stroke="#000000" points="1928.0156,-102.3077 1918.1892,-106.2691 1928.3412,-109.3001 1928.0156,-102.3077"/>
</g>
<!-- modm_architecture_spi_device -->
<g id="node22" class="node">
<title>modm_architecture_spi_device</title>
<g id="a_node22"><a xlink:href="../modm-architecture-spi-device" xlink:title="modm:&#10;architecture:&#10;spi.device">
<polygon fill="#d3d3d3" stroke="#000000" points="2568,-53 2458,-53 2458,0 2568,0 2568,-53"/>
<text text-anchor="middle" x="2513" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2513" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2513" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi.device</text>
</a>
</g>
</g>
<!-- modm_architecture_spi_device&#45;&gt;modm_architecture -->
<g id="edge21" class="edge">
<title>modm_architecture_spi_device&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2457.8395,-50.5225C2454.8764,-51.4405 2451.918,-52.2759 2449,-53 2351.6837,-77.1489 2058.5387,-96.6892 1928.4112,-104.4224"/>
<polygon fill="#000000" stroke="#000000" points="1927.9814,-100.9416 1918.2049,-105.0246 1928.3938,-107.9294 1927.9814,-100.9416"/>
</g>
<!-- modm_architecture_uart -->
<g id="node23" class="node">
<title>modm_architecture_uart</title>
<g id="a_node23"><a xlink:href="../modm-architecture-uart" xlink:title="modm:&#10;architecture:&#10;uart">
<polygon fill="#d3d3d3" stroke="#000000" points="2696,-53 2586,-53 2586,0 2696,0 2696,-53"/>
<text text-anchor="middle" x="2641" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2641" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2641" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">uart</text>
</a>
</g>
</g>
<!-- modm_architecture_uart&#45;&gt;modm_architecture -->
<g id="edge22" class="edge">
<title>modm_architecture_uart&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2585.8541,-50.5824C2582.8872,-51.4848 2579.924,-52.3005 2577,-53 2454.638,-82.2746 2078.7621,-99.7012 1928.2654,-105.6655"/>
<polygon fill="#000000" stroke="#000000" points="1928.1092,-102.1689 1918.2542,-106.0585 1928.3839,-109.1635 1928.1092,-102.1689"/>
</g>
<!-- modm_architecture_unaligned -->
<g id="node24" class="node">
<title>modm_architecture_unaligned</title>
<g id="a_node24"><a xlink:href="../modm-architecture-unaligned" xlink:title="modm:&#10;architecture:&#10;unaligned">
<polygon fill="#d3d3d3" stroke="#000000" points="2824,-53 2714,-53 2714,0 2824,0 2824,-53"/>
<text text-anchor="middle" x="2769" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2769" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2769" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">unaligned</text>
</a>
</g>
</g>
<!-- modm_architecture_unaligned&#45;&gt;modm_architecture -->
<g id="edge23" class="edge">
<title>modm_architecture_unaligned&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2713.864,-50.6243C2710.8945,-51.5157 2707.928,-52.3176 2705,-53 2557.3973,-87.4009 2097.5894,-102.1616 1928.3509,-106.5257"/>
<polygon fill="#000000" stroke="#000000" points="1928.0162,-103.033 1918.1084,-106.7863 1928.1942,-110.0308 1928.0162,-103.033"/>
</g>
<!-- modm_container -->
<g id="node25" class="node">
<title>modm_container</title>
<g id="a_node25"><a xlink:href="../modm-container" xlink:title="modm:&#10;container">
<polygon fill="#d3d3d3" stroke="#000000" points="2927.5,-45.5 2842.5,-45.5 2842.5,-7.5 2927.5,-7.5 2927.5,-45.5"/>
<text text-anchor="middle" x="2885" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2885" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">container</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_architecture -->
<g id="edge24" class="edge">
<title>modm_container&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2852.5372,-45.6076C2846.2385,-48.5688 2839.5535,-51.2283 2833,-53 2746.1543,-76.4783 2129.2977,-99.0993 1928.2075,-105.9216"/>
<polygon fill="#000000" stroke="#000000" points="1927.9766,-102.4273 1918.1005,-106.263 1928.213,-109.4233 1927.9766,-102.4273"/>
</g>
<!-- modm_debug -->
<g id="node26" class="node">
<title>modm_debug</title>
<g id="a_node26"><a xlink:href="../modm-debug" xlink:title="modm:&#10;debug">
<polygon fill="#d3d3d3" stroke="#000000" points="3010.5,-45.5 2945.5,-45.5 2945.5,-7.5 3010.5,-7.5 3010.5,-45.5"/>
<text text-anchor="middle" x="2978" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2978" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">debug</text>
</a>
</g>
</g>
<!-- modm_debug&#45;&gt;modm_architecture -->
<g id="edge25" class="edge">
<title>modm_debug&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2953.0145,-45.5171C2947.9476,-48.5276 2942.4841,-51.228 2937,-53 2841.2589,-83.9358 2144.1419,-101.8183 1928.4339,-106.655"/>
<polygon fill="#000000" stroke="#000000" points="1928.1475,-103.1604 1918.2279,-106.8822 1928.3033,-110.1587 1928.1475,-103.1604"/>
</g>
<!-- modm_math_filter -->
<g id="node27" class="node">
<title>modm_math_filter</title>
<g id="a_node27"><a xlink:href="../modm-math-filter" xlink:title="modm:&#10;math:&#10;filter">
<polygon fill="#d3d3d3" stroke="#000000" points="3093.5,-53 3028.5,-53 3028.5,0 3093.5,0 3093.5,-53"/>
<text text-anchor="middle" x="3061" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="3061" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="3061" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">filter</text>
</a>
</g>
</g>
<!-- modm_math_filter&#45;&gt;modm_architecture -->
<g id="edge26" class="edge">
<title>modm_math_filter&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M3028.1674,-49.7343C3025.4818,-51.0046 3022.7437,-52.119 3020,-53 2916.0543,-86.3759 2154.7972,-102.7431 1928.3609,-106.9084"/>
<polygon fill="#000000" stroke="#000000" points="1928.2382,-103.41 1918.3037,-107.0918 1928.3659,-110.4088 1928.2382,-103.41"/>
</g>
<!-- modm_math_geometry -->
<g id="node28" class="node">
<title>modm_math_geometry</title>
<g id="a_node28"><a xlink:href="../modm-math-geometry" xlink:title="modm:&#10;math:&#10;geometry">
<polygon fill="#d3d3d3" stroke="#000000" points="3196.5,-53 3111.5,-53 3111.5,0 3196.5,0 3196.5,-53"/>
<text text-anchor="middle" x="3154" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="3154" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="3154" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">geometry</text>
</a>
</g>
</g>
<!-- modm_math_geometry&#45;&gt;modm_architecture -->
<g id="edge27" class="edge">
<title>modm_math_geometry&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M3111.4081,-50.2902C3108.6113,-51.3317 3105.795,-52.2511 3103,-53 2989.2901,-83.469 2165.08,-102.0413 1928.3944,-106.7876"/>
<polygon fill="#000000" stroke="#000000" points="1928.1606,-103.2914 1918.2324,-106.9901 1928.3001,-110.29 1928.1606,-103.2914"/>
</g>
<!-- modm_math_utils -->
<g id="node29" class="node">
<title>modm_math_utils</title>
<g id="a_node29"><a xlink:href="../modm-math-utils" xlink:title="modm:&#10;math:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="3279.5,-53 3214.5,-53 3214.5,0 3279.5,0 3279.5,-53"/>
<text text-anchor="middle" x="3247" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="3247" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="3247" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_math_utils&#45;&gt;modm_architecture -->
<g id="edge29" class="edge">
<title>modm_math_utils&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M3214.1739,-49.7548C3211.4868,-51.0203 3208.7466,-52.1281 3206,-53 3083.6666,-91.8357 2178.5313,-104.5636 1928.7067,-107.3562"/>
<polygon fill="#000000" stroke="#000000" points="1928.3122,-103.8603 1918.3515,-107.4705 1928.3895,-110.8599 1928.3122,-103.8603"/>
</g>
<!-- modm_processing_protothread -->
<g id="node30" class="node">
<title>modm_processing_protothread</title>
<g id="a_node30"><a xlink:href="../modm-processing-protothread" xlink:title="modm:&#10;processing:&#10;protothread">
<polygon fill="#d3d3d3" stroke="#000000" points="3400,-53 3298,-53 3298,0 3400,0 3400,-53"/>
<text text-anchor="middle" x="3349" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="3349" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="3349" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">protothread</text>
</a>
</g>
</g>
<!-- modm_processing_protothread&#45;&gt;modm_architecture -->
<g id="edge30" class="edge">
<title>modm_processing_protothread&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M3297.7162,-50.6202C3294.8022,-51.528 3291.8846,-52.3329 3289,-53 3155.5629,-83.8571 2187.5797,-102.4826 1928.5868,-106.9501"/>
<polygon fill="#000000" stroke="#000000" points="1928.1681,-103.4567 1918.2296,-107.1277 1928.2882,-110.4557 1928.1681,-103.4567"/>
</g>
<!-- modm_utils -->
<g id="node31" class="node">
<title>modm_utils</title>
<g id="a_node31"><a xlink:href="../modm-utils" xlink:title="modm:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="3483.5,-45.5 3418.5,-45.5 3418.5,-7.5 3483.5,-7.5 3483.5,-45.5"/>
<text text-anchor="middle" x="3451" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="3451" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_utils&#45;&gt;modm_architecture -->
<g id="edge31" class="edge">
<title>modm_utils&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M3425.3799,-45.5878C3420.1917,-48.5918 3414.6023,-51.2717 3409,-53 3266.1046,-97.0834 2200.496,-106.0997 1928.2041,-107.6915"/>
<polygon fill="#000000" stroke="#000000" points="1928.0772,-104.1921 1918.0975,-107.7493 1928.1173,-111.192 1928.0772,-104.1921"/>
</g>
</g>
</svg>

