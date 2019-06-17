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
const std::array<uint8_t, 20>& modm::build_id();
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
enum Abandonment;
enum MemoryTrait;
enum RomCommand;
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
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:architecture Pages: 1 -->
<svg width="2929pt" height="224pt"
 viewBox="0.00 0.00 2929.00 224.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 220)">
<title>modm:architecture</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-220 2925,-220 2925,4 -4,4"/>
<!-- modm_architecture -->
<g id="node1" class="node">
<title>modm_architecture</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="1577,-127 1497,-127 1497,-89 1577,-89 1577,-127"/>
<text text-anchor="middle" x="1537" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1537" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture</text>
</g>
<!-- modm_math_units -->
<g id="node2" class="node">
<title>modm_math_units</title>
<g id="a_node2"><a xlink:href="../modm-math-units" xlink:title="modm:&#10;math:&#10;units">
<polygon fill="#d3d3d3" stroke="#000000" points="1565,-216 1509,-216 1509,-163 1565,-163 1565,-216"/>
<text text-anchor="middle" x="1537" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1537" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="1537" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">units</text>
</a>
</g>
</g>
<!-- modm_architecture&#45;&gt;modm_math_units -->
<g id="edge1" class="edge">
<title>modm_architecture&#45;&gt;modm_math_units</title>
<path fill="none" stroke="#000000" d="M1531.1601,-127.3144C1530.4434,-134.9606 1530.15,-144.0143 1530.2799,-152.8129"/>
<polygon fill="#000000" stroke="#000000" points="1526.7859,-153.0508 1530.6269,-162.9249 1533.7818,-152.8107 1526.7859,-153.0508"/>
</g>
<!-- modm_math_units&#45;&gt;modm_architecture -->
<g id="edge29" class="edge">
<title>modm_math_units&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1543.3731,-162.9249C1543.8243,-154.762 1543.8704,-145.7322 1543.5114,-137.3922"/>
<polygon fill="#000000" stroke="#000000" points="1546.997,-137.0595 1542.8399,-127.3144 1540.0125,-137.5249 1546.997,-137.0595"/>
</g>
<!-- modm_architecture_1_wire -->
<g id="node3" class="node">
<title>modm_architecture_1_wire</title>
<g id="a_node3"><a xlink:href="../modm-architecture-1-wire" xlink:title="modm:&#10;architecture:&#10;1&#45;wire">
<polygon fill="#d3d3d3" stroke="#000000" points="84,-53 0,-53 0,0 84,0 84,-53"/>
<text text-anchor="middle" x="42" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="42" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="42" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">1&#45;wire</text>
</a>
</g>
</g>
<!-- modm_architecture_1_wire&#45;&gt;modm_architecture -->
<g id="edge2" class="edge">
<title>modm_architecture_1_wire&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M84.0077,-50.0949C86.9957,-51.2207 90.0097,-52.2089 93,-53 229.8155,-89.1968 1247.3102,-104.3142 1486.6799,-107.3903"/>
<polygon fill="#000000" stroke="#000000" points="1486.7903,-110.8919 1496.8341,-107.5195 1486.8794,-103.8924 1486.7903,-110.8919"/>
</g>
<!-- modm_architecture_accessor -->
<g id="node4" class="node">
<title>modm_architecture_accessor</title>
<g id="a_node4"><a xlink:href="../modm-architecture-accessor" xlink:title="modm:&#10;architecture:&#10;accessor">
<polygon fill="#d3d3d3" stroke="#000000" points="186,-53 102,-53 102,0 186,0 186,-53"/>
<text text-anchor="middle" x="144" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="144" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="144" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">accessor</text>
</a>
</g>
</g>
<!-- modm_architecture_accessor&#45;&gt;modm_architecture -->
<g id="edge3" class="edge">
<title>modm_architecture_accessor&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M186.0104,-50.0849C188.9977,-51.2131 192.0108,-52.2045 195,-53 321.4992,-86.6631 1258.5922,-103.5874 1486.9381,-107.2378"/>
<polygon fill="#000000" stroke="#000000" points="1486.9046,-110.7376 1496.9589,-107.3967 1487.0157,-103.7385 1486.9046,-110.7376"/>
</g>
<!-- modm_architecture_adc -->
<g id="node5" class="node">
<title>modm_architecture_adc</title>
<g id="a_node5"><a xlink:href="../modm-architecture-adc" xlink:title="modm:&#10;architecture:&#10;adc">
<polygon fill="#d3d3d3" stroke="#000000" points="288,-53 204,-53 204,0 288,0 288,-53"/>
<text text-anchor="middle" x="246" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="246" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="246" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">adc</text>
</a>
</g>
</g>
<!-- modm_architecture_adc&#45;&gt;modm_architecture -->
<g id="edge4" class="edge">
<title>modm_architecture_adc&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M288.0135,-50.0733C291.0001,-51.2043 294.0122,-52.1995 297,-53 413.0929,-84.1031 1268.5732,-102.7657 1486.5651,-107.0492"/>
<polygon fill="#000000" stroke="#000000" points="1486.69,-110.5522 1496.7564,-107.2481 1486.8266,-103.5536 1486.69,-110.5522"/>
</g>
<!-- modm_architecture_assert -->
<g id="node6" class="node">
<title>modm_architecture_assert</title>
<g id="a_node6"><a xlink:href="../modm-architecture-assert" xlink:title="modm:&#10;architecture:&#10;assert">
<polygon fill="#d3d3d3" stroke="#000000" points="390,-53 306,-53 306,0 390,0 390,-53"/>
<text text-anchor="middle" x="348" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="348" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="348" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">assert</text>
</a>
</g>
</g>
<!-- modm_architecture_assert&#45;&gt;modm_architecture -->
<g id="edge5" class="edge">
<title>modm_architecture_assert&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M390.0172,-50.0595C393.0029,-51.1939 396.0138,-52.1936 399,-53 504.8138,-81.5743 1280.4661,-101.8981 1486.7369,-106.8382"/>
<polygon fill="#000000" stroke="#000000" points="1486.8928,-110.3428 1496.9733,-107.0819 1487.0595,-103.3448 1486.8928,-110.3428"/>
</g>
<!-- modm_architecture_atomic -->
<g id="node7" class="node">
<title>modm_architecture_atomic</title>
<g id="a_node7"><a xlink:href="../modm-architecture-atomic" xlink:title="modm:&#10;architecture:&#10;atomic">
<polygon fill="#d3d3d3" stroke="#000000" points="492,-53 408,-53 408,0 492,0 492,-53"/>
<text text-anchor="middle" x="450" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="450" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="450" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">atomic</text>
</a>
</g>
</g>
<!-- modm_architecture_atomic&#45;&gt;modm_architecture -->
<g id="edge6" class="edge">
<title>modm_architecture_atomic&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M492.0216,-50.0431C495.0063,-51.1814 498.0157,-52.1865 501,-53 596.5019,-79.0329 1291.9195,-100.9125 1486.5207,-106.5755"/>
<polygon fill="#000000" stroke="#000000" points="1486.6494,-110.0806 1496.7466,-106.8715 1486.8521,-103.0835 1486.6494,-110.0806"/>
</g>
<!-- modm_architecture_block_device -->
<g id="node8" class="node">
<title>modm_architecture_block_device</title>
<g id="a_node8"><a xlink:href="../modm-architecture-block-device" xlink:title="modm:&#10;architecture:&#10;block.device">
<polygon fill="#d3d3d3" stroke="#000000" points="596,-53 510,-53 510,0 596,0 596,-53"/>
<text text-anchor="middle" x="553" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="553" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="553" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">block.device</text>
</a>
</g>
</g>
<!-- modm_architecture_block_device&#45;&gt;modm_architecture -->
<g id="edge7" class="edge">
<title>modm_architecture_block_device&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M596.1662,-50.1312C599.1064,-51.2297 602.0665,-52.2035 605,-53 773.4415,-98.7351 1318.0861,-106.4445 1486.7422,-107.7396"/>
<polygon fill="#000000" stroke="#000000" points="1486.8092,-111.2401 1496.8344,-107.8128 1486.86,-104.2403 1486.8092,-111.2401"/>
</g>
<!-- modm_architecture_build_id -->
<g id="node9" class="node">
<title>modm_architecture_build_id</title>
<g id="a_node9"><a xlink:href="../modm-architecture-build-id" xlink:title="modm:&#10;architecture:&#10;build_id">
<polygon fill="#d3d3d3" stroke="#000000" points="698,-53 614,-53 614,0 698,0 698,-53"/>
<text text-anchor="middle" x="656" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="656" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="656" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">build_id</text>
</a>
</g>
</g>
<!-- modm_architecture_build_id&#45;&gt;modm_architecture -->
<g id="edge8" class="edge">
<title>modm_architecture_build_id&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M698.0341,-49.9978C701.0158,-51.147 704.0211,-52.1669 707,-53 854.8493,-94.3469 1329.6256,-104.952 1486.3033,-107.359"/>
<polygon fill="#000000" stroke="#000000" points="1486.5405,-110.8628 1496.5916,-107.5122 1486.6448,-103.8636 1486.5405,-110.8628"/>
</g>
<!-- modm_architecture_can -->
<g id="node10" class="node">
<title>modm_architecture_can</title>
<g id="a_node10"><a xlink:href="../modm-architecture-can" xlink:title="modm:&#10;architecture:&#10;can">
<polygon fill="#d3d3d3" stroke="#000000" points="800,-53 716,-53 716,0 800,0 800,-53"/>
<text text-anchor="middle" x="758" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="758" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="758" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">can</text>
</a>
</g>
</g>
<!-- modm_architecture_can&#45;&gt;modm_architecture -->
<g id="edge9" class="edge">
<title>modm_architecture_can&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M800.0431,-49.9659C803.0226,-51.1229 806.025,-52.1532 809,-53 936.7016,-89.3493 1342.9679,-103.0397 1486.3223,-106.8134"/>
<polygon fill="#000000" stroke="#000000" points="1486.4952,-110.319 1496.5822,-107.0783 1486.6759,-103.3213 1486.4952,-110.319"/>
</g>
<!-- modm_architecture_clock -->
<g id="node11" class="node">
<title>modm_architecture_clock</title>
<g id="a_node11"><a xlink:href="../modm-architecture-clock" xlink:title="modm:&#10;architecture:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="902,-53 818,-53 818,0 902,0 902,-53"/>
<text text-anchor="middle" x="860" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="860" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="860" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_architecture_clock&#45;&gt;modm_architecture -->
<g id="edge10" class="edge">
<title>modm_architecture_clock&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M902.0553,-49.9238C905.0318,-51.0909 908.0302,-52.135 911,-53 1018.7407,-84.3809 1357.5596,-100.8435 1486.5037,-106.0999"/>
<polygon fill="#000000" stroke="#000000" points="1486.7385,-109.6121 1496.8711,-106.5167 1487.0198,-102.6177 1486.7385,-109.6121"/>
</g>
<!-- modm_architecture_delay -->
<g id="node12" class="node">
<title>modm_architecture_delay</title>
<g id="a_node12"><a xlink:href="../modm-architecture-delay" xlink:title="modm:&#10;architecture:&#10;delay">
<polygon fill="#d3d3d3" stroke="#000000" points="1004,-53 920,-53 920,0 1004,0 1004,-53"/>
<text text-anchor="middle" x="962" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="962" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="962" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">delay</text>
</a>
</g>
</g>
<!-- modm_architecture_delay&#45;&gt;modm_architecture -->
<g id="edge11" class="edge">
<title>modm_architecture_delay&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1004.0725,-49.8655C1007.0449,-51.0467 1010.0376,-52.1099 1013,-53 1100.924,-79.4192 1373.1371,-98.223 1486.5728,-105.1106"/>
<polygon fill="#000000" stroke="#000000" points="1486.53,-108.6143 1496.722,-105.7207 1486.9501,-101.6269 1486.53,-108.6143"/>
</g>
<!-- modm_architecture_gpio -->
<g id="node13" class="node">
<title>modm_architecture_gpio</title>
<g id="a_node13"><a xlink:href="../modm-architecture-gpio" xlink:title="modm:&#10;architecture:&#10;gpio">
<polygon fill="#d3d3d3" stroke="#000000" points="1106,-53 1022,-53 1022,0 1106,0 1106,-53"/>
<text text-anchor="middle" x="1064" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1064" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1064" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio</text>
</a>
</g>
</g>
<!-- modm_architecture_gpio&#45;&gt;modm_architecture -->
<g id="edge12" class="edge">
<title>modm_architecture_gpio&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1106.0988,-49.7798C1109.0648,-50.9817 1112.049,-52.0729 1115,-53 1183.3487,-74.4731 1390.3221,-94.976 1486.6646,-103.6555"/>
<polygon fill="#000000" stroke="#000000" points="1486.4712,-107.152 1496.7432,-104.5566 1487.0947,-100.1799 1486.4712,-107.152"/>
</g>
<!-- modm_architecture_gpio_expander -->
<g id="node14" class="node">
<title>modm_architecture_gpio_expander</title>
<g id="a_node14"><a xlink:href="../modm-architecture-gpio-expander" xlink:title="modm:&#10;architecture:&#10;gpio.expander">
<polygon fill="#d3d3d3" stroke="#000000" points="1218,-53 1124,-53 1124,0 1218,0 1218,-53"/>
<text text-anchor="middle" x="1171" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1171" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1171" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio.expander</text>
</a>
</g>
</g>
<!-- modm_architecture_gpio_expander&#45;&gt;modm_architecture -->
<g id="edge13" class="edge">
<title>modm_architecture_gpio_expander&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1218.3091,-49.9403C1221.2193,-51.0527 1224.1308,-52.0847 1227,-53 1315.8543,-81.3441 1423.9793,-96.3381 1486.213,-103.1863"/>
<polygon fill="#000000" stroke="#000000" points="1486.2242,-106.7075 1496.5405,-104.2941 1486.9708,-99.7474 1486.2242,-106.7075"/>
</g>
<!-- modm_architecture_heap -->
<g id="node15" class="node">
<title>modm_architecture_heap</title>
<g id="a_node15"><a xlink:href="../modm-architecture-heap" xlink:title="modm:&#10;architecture:&#10;heap">
<polygon fill="#d3d3d3" stroke="#000000" points="1320,-53 1236,-53 1236,0 1320,0 1320,-53"/>
<text text-anchor="middle" x="1278" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1278" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1278" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">heap</text>
</a>
</g>
</g>
<!-- modm_architecture_heap&#45;&gt;modm_architecture -->
<g id="edge14" class="edge">
<title>modm_architecture_heap&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1320.2514,-49.3438C1323.1806,-50.651 1326.1148,-51.8847 1329,-53 1381.4129,-73.2603 1444.0547,-88.702 1486.8031,-97.9919"/>
<polygon fill="#000000" stroke="#000000" points="1486.1742,-101.4365 1496.6853,-100.1069 1487.6392,-94.5915 1486.1742,-101.4365"/>
</g>
<!-- modm_architecture_i2c -->
<g id="node16" class="node">
<title>modm_architecture_i2c</title>
<g id="a_node16"><a xlink:href="../modm-architecture-i2c" xlink:title="modm:&#10;architecture:&#10;i2c">
<polygon fill="#d3d3d3" stroke="#000000" points="1422,-53 1338,-53 1338,0 1422,0 1422,-53"/>
<text text-anchor="middle" x="1380" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1380" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1380" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c&#45;&gt;modm_architecture -->
<g id="edge15" class="edge">
<title>modm_architecture_i2c&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1422.0237,-48.3383C1425.0613,-49.9161 1428.0777,-51.4826 1431,-53 1450.7656,-63.2631 1472.6076,-74.5973 1491.303,-84.2967"/>
<polygon fill="#000000" stroke="#000000" points="1489.7168,-87.4167 1500.2051,-88.915 1492.9403,-81.2031 1489.7168,-87.4167"/>
</g>
<!-- modm_architecture_i2c_device -->
<g id="node17" class="node">
<title>modm_architecture_i2c_device</title>
<g id="a_node17"><a xlink:href="../modm-architecture-i2c-device" xlink:title="modm:&#10;architecture:&#10;i2c.device">
<polygon fill="#d3d3d3" stroke="#000000" points="1524,-53 1440,-53 1440,0 1524,0 1524,-53"/>
<text text-anchor="middle" x="1482" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1482" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1482" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.device</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_device&#45;&gt;modm_architecture -->
<g id="edge16" class="edge">
<title>modm_architecture_i2c_device&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1500.0091,-53.1861C1505.8832,-61.8906 1512.4159,-71.5708 1518.3369,-80.3447"/>
<polygon fill="#000000" stroke="#000000" points="1515.5121,-82.4157 1524.0071,-88.7469 1521.3144,-78.5 1515.5121,-82.4157"/>
</g>
<!-- modm_architecture_i2c_multiplexer -->
<g id="node18" class="node">
<title>modm_architecture_i2c_multiplexer</title>
<g id="a_node18"><a xlink:href="../modm-architecture-i2c-multiplexer" xlink:title="modm:&#10;architecture:&#10;i2c.multiplexer">
<polygon fill="#d3d3d3" stroke="#000000" points="1642,-53 1542,-53 1542,0 1642,0 1642,-53"/>
<text text-anchor="middle" x="1592" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1592" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1592" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.multiplexer</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_multiplexer&#45;&gt;modm_architecture -->
<g id="edge17" class="edge">
<title>modm_architecture_i2c_multiplexer&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1573.9909,-53.1861C1568.1168,-61.8906 1561.5841,-71.5708 1555.6631,-80.3447"/>
<polygon fill="#000000" stroke="#000000" points="1552.6856,-78.5 1549.9929,-88.7469 1558.4879,-82.4157 1552.6856,-78.5"/>
</g>
<!-- modm_architecture_interrupt -->
<g id="node19" class="node">
<title>modm_architecture_interrupt</title>
<g id="a_node19"><a xlink:href="../modm-architecture-interrupt" xlink:title="modm:&#10;architecture:&#10;interrupt">
<polygon fill="#d3d3d3" stroke="#000000" points="1744,-53 1660,-53 1660,0 1744,0 1744,-53"/>
<text text-anchor="middle" x="1702" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1702" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1702" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">interrupt</text>
</a>
</g>
</g>
<!-- modm_architecture_interrupt&#45;&gt;modm_architecture -->
<g id="edge18" class="edge">
<title>modm_architecture_interrupt&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1659.7487,-48.5768C1656.7929,-50.0835 1653.8549,-51.57 1651,-53 1629.9677,-63.5349 1606.5918,-74.8763 1586.5098,-84.5076"/>
<polygon fill="#000000" stroke="#000000" points="1584.7478,-81.4706 1577.2388,-88.9448 1587.7699,-87.7846 1584.7478,-81.4706"/>
</g>
<!-- modm_architecture_memory -->
<g id="node20" class="node">
<title>modm_architecture_memory</title>
<g id="a_node20"><a xlink:href="../modm-architecture-memory" xlink:title="modm:&#10;architecture:&#10;memory">
<polygon fill="#d3d3d3" stroke="#000000" points="1846,-53 1762,-53 1762,0 1846,0 1846,-53"/>
<text text-anchor="middle" x="1804" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1804" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1804" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">memory</text>
</a>
</g>
</g>
<!-- modm_architecture_memory&#45;&gt;modm_architecture -->
<g id="edge19" class="edge">
<title>modm_architecture_memory&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1761.7604,-49.3745C1758.8284,-50.6743 1755.8902,-51.898 1753,-53 1697.8193,-74.0397 1631.6664,-89.5403 1587.2338,-98.6288"/>
<polygon fill="#000000" stroke="#000000" points="1586.3744,-95.2314 1577.2614,-100.6355 1587.7554,-102.0939 1586.3744,-95.2314"/>
</g>
<!-- modm_architecture_register -->
<g id="node21" class="node">
<title>modm_architecture_register</title>
<g id="a_node21"><a xlink:href="../modm-architecture-register" xlink:title="modm:&#10;architecture:&#10;register">
<polygon fill="#d3d3d3" stroke="#000000" points="1948,-53 1864,-53 1864,0 1948,0 1948,-53"/>
<text text-anchor="middle" x="1906" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="1906" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="1906" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">register</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_architecture -->
<g id="edge20" class="edge">
<title>modm_architecture_register&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1863.8549,-49.6378C1860.9001,-50.874 1857.9311,-52.0116 1855,-53 1763.4703,-83.8648 1650.9987,-98.1022 1587.3032,-104.1094"/>
<polygon fill="#000000" stroke="#000000" points="1586.7714,-100.6433 1577.1309,-105.0376 1587.4076,-107.6143 1586.7714,-100.6433"/>
</g>
<!-- modm_architecture_spi -->
<g id="node22" class="node">
<title>modm_architecture_spi</title>
<g id="a_node22"><a xlink:href="../modm-architecture-spi" xlink:title="modm:&#10;architecture:&#10;spi">
<polygon fill="#d3d3d3" stroke="#000000" points="2050,-53 1966,-53 1966,0 2050,0 2050,-53"/>
<text text-anchor="middle" x="2008" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2008" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2008" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi</text>
</a>
</g>
</g>
<!-- modm_architecture_spi&#45;&gt;modm_architecture -->
<g id="edge21" class="edge">
<title>modm_architecture_spi&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M1965.9005,-49.7777C1962.9347,-50.9801 1959.9507,-52.072 1957,-53 1888.9763,-74.3938 1682.9833,-94.9375 1587.0971,-103.6415"/>
<polygon fill="#000000" stroke="#000000" points="1586.7117,-100.1619 1577.0661,-104.5452 1587.3399,-107.1337 1586.7117,-100.1619"/>
</g>
<!-- modm_architecture_spi_device -->
<g id="node23" class="node">
<title>modm_architecture_spi_device</title>
<g id="a_node23"><a xlink:href="../modm-architecture-spi-device" xlink:title="modm:&#10;architecture:&#10;spi.device">
<polygon fill="#d3d3d3" stroke="#000000" points="2152,-53 2068,-53 2068,0 2152,0 2152,-53"/>
<text text-anchor="middle" x="2110" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2110" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2110" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi.device</text>
</a>
</g>
</g>
<!-- modm_architecture_spi_device&#45;&gt;modm_architecture -->
<g id="edge22" class="edge">
<title>modm_architecture_spi_device&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2067.9271,-49.8642C2064.9548,-51.0457 2061.9622,-52.1093 2059,-53 1971.4132,-79.3369 1700.2382,-98.1875 1587.235,-105.0991"/>
<polygon fill="#000000" stroke="#000000" points="1586.8946,-101.6132 1577.1244,-105.7114 1587.3178,-108.6004 1586.8946,-101.6132"/>
</g>
<!-- modm_architecture_uart -->
<g id="node24" class="node">
<title>modm_architecture_uart</title>
<g id="a_node24"><a xlink:href="../modm-architecture-uart" xlink:title="modm:&#10;architecture:&#10;uart">
<polygon fill="#d3d3d3" stroke="#000000" points="2254,-53 2170,-53 2170,0 2254,0 2254,-53"/>
<text text-anchor="middle" x="2212" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2212" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2212" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">uart</text>
</a>
</g>
</g>
<!-- modm_architecture_uart&#45;&gt;modm_architecture -->
<g id="edge23" class="edge">
<title>modm_architecture_uart&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2169.9445,-49.9228C2166.968,-51.0902 2163.9697,-52.1346 2161,-53 2053.6053,-84.2964 1715.8678,-100.8103 1587.3352,-106.0901"/>
<polygon fill="#000000" stroke="#000000" points="1586.8509,-102.6068 1577.0009,-106.5088 1587.1344,-109.601 1586.8509,-102.6068"/>
</g>
<!-- modm_architecture_unaligned -->
<g id="node25" class="node">
<title>modm_architecture_unaligned</title>
<g id="a_node25"><a xlink:href="../modm-architecture-unaligned" xlink:title="modm:&#10;architecture:&#10;unaligned">
<polygon fill="#d3d3d3" stroke="#000000" points="2356,-53 2272,-53 2272,0 2356,0 2356,-53"/>
<text text-anchor="middle" x="2314" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2314" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="2314" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">unaligned</text>
</a>
</g>
</g>
<!-- modm_architecture_unaligned&#45;&gt;modm_architecture -->
<g id="edge24" class="edge">
<title>modm_architecture_unaligned&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2271.9567,-49.9652C2268.9773,-51.1223 2265.975,-52.1529 2263,-53 2135.651,-89.2632 1730.4997,-103.0084 1587.5386,-106.8049"/>
<polygon fill="#000000" stroke="#000000" points="1587.2123,-103.3121 1577.3069,-107.0714 1587.3947,-110.3098 1587.2123,-103.3121"/>
</g>
<!-- modm_container -->
<g id="node26" class="node">
<title>modm_container</title>
<g id="a_node26"><a xlink:href="../modm-container" xlink:title="modm:&#10;container">
<polygon fill="#d3d3d3" stroke="#000000" points="2441.5,-45.5 2374.5,-45.5 2374.5,-7.5 2441.5,-7.5 2441.5,-45.5"/>
<text text-anchor="middle" x="2408" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2408" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">container</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_architecture -->
<g id="edge25" class="edge">
<title>modm_container&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2381.2108,-45.7127C2376.0413,-48.6235 2370.5129,-51.2382 2365,-53 2291.0715,-76.6255 1754.9266,-99.4474 1587.0893,-106.0788"/>
<polygon fill="#000000" stroke="#000000" points="1586.8939,-102.5837 1577.0392,-106.4739 1587.169,-109.5783 1586.8939,-102.5837"/>
</g>
<!-- modm_debug -->
<g id="node27" class="node">
<title>modm_debug</title>
<g id="a_node27"><a xlink:href="../modm-debug" xlink:title="modm:&#10;debug">
<polygon fill="#d3d3d3" stroke="#000000" points="2516,-45.5 2460,-45.5 2460,-7.5 2516,-7.5 2516,-45.5"/>
<text text-anchor="middle" x="2488" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2488" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">debug</text>
</a>
</g>
</g>
<!-- modm_debug&#45;&gt;modm_architecture -->
<g id="edge26" class="edge">
<title>modm_debug&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2465.2317,-45.7436C2460.7548,-48.6512 2455.9149,-51.2569 2451,-53 2369.5634,-81.8818 1767.1873,-101.3768 1587.472,-106.5969"/>
<polygon fill="#000000" stroke="#000000" points="1587.1377,-103.105 1577.2427,-106.8918 1587.3395,-110.1021 1587.1377,-103.105"/>
</g>
<!-- modm_math_filter -->
<g id="node28" class="node">
<title>modm_math_filter</title>
<g id="a_node28"><a xlink:href="../modm-math-filter" xlink:title="modm:&#10;math:&#10;filter">
<polygon fill="#d3d3d3" stroke="#000000" points="2590,-53 2534,-53 2534,0 2590,0 2590,-53"/>
<text text-anchor="middle" x="2562" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2562" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="2562" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">filter</text>
</a>
</g>
</g>
<!-- modm_math_filter&#45;&gt;modm_architecture -->
<g id="edge27" class="edge">
<title>modm_math_filter&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2533.7945,-49.0193C2530.9339,-50.5816 2527.9811,-51.9503 2525,-53 2436.2887,-84.2377 1776.4008,-102.2783 1587.4291,-106.8422"/>
<polygon fill="#000000" stroke="#000000" points="1587.1366,-103.3481 1577.2233,-107.0866 1587.3043,-110.3461 1587.1366,-103.3481"/>
</g>
<!-- modm_math_geometry -->
<g id="node29" class="node">
<title>modm_math_geometry</title>
<g id="a_node29"><a xlink:href="../modm-math-geometry" xlink:title="modm:&#10;math:&#10;geometry">
<polygon fill="#d3d3d3" stroke="#000000" points="2676,-53 2608,-53 2608,0 2676,0 2676,-53"/>
<text text-anchor="middle" x="2642" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2642" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="2642" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">geometry</text>
</a>
</g>
</g>
<!-- modm_math_geometry&#45;&gt;modm_architecture -->
<g id="edge28" class="edge">
<title>modm_math_geometry&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2607.9896,-49.4948C2605.0306,-50.862 2602.014,-52.061 2599,-53 2501.8641,-83.2631 1784.6181,-102.1878 1587.1825,-106.8624"/>
<polygon fill="#000000" stroke="#000000" points="1587.0053,-103.3655 1577.0903,-107.0996 1587.1698,-110.3636 1587.0053,-103.3655"/>
</g>
<!-- modm_math_utils -->
<g id="node30" class="node">
<title>modm_math_utils</title>
<g id="a_node30"><a xlink:href="../modm-math-utils" xlink:title="modm:&#10;math:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="2750,-53 2694,-53 2694,0 2750,0 2750,-53"/>
<text text-anchor="middle" x="2722" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2722" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="2722" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_math_utils&#45;&gt;modm_architecture -->
<g id="edge30" class="edge">
<title>modm_math_utils&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2693.8031,-49.044C2690.9407,-50.6011 2687.9851,-51.9618 2685,-53 2580.4606,-89.3572 1794.7596,-104.0075 1587.1738,-107.2669"/>
<polygon fill="#000000" stroke="#000000" points="1587.1053,-103.7674 1577.1608,-107.4223 1587.214,-110.7666 1587.1053,-103.7674"/>
</g>
<!-- modm_processing_protothread -->
<g id="node31" class="node">
<title>modm_processing_protothread</title>
<g id="a_node31"><a xlink:href="../modm-processing-protothread" xlink:title="modm:&#10;processing:&#10;protothread">
<polygon fill="#d3d3d3" stroke="#000000" points="2847.5,-53 2768.5,-53 2768.5,0 2847.5,0 2847.5,-53"/>
<text text-anchor="middle" x="2808" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2808" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="2808" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">protothread</text>
</a>
</g>
</g>
<!-- modm_processing_protothread&#45;&gt;modm_architecture -->
<g id="edge31" class="edge">
<title>modm_processing_protothread&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2768.2341,-49.852C2765.1725,-51.0746 2762.0756,-52.1476 2759,-53 2644.9827,-84.6008 1803.4569,-102.8623 1587.4289,-107.0618"/>
<polygon fill="#000000" stroke="#000000" points="1587.255,-103.5644 1577.3244,-107.2568 1587.3901,-110.5631 1587.255,-103.5644"/>
</g>
<!-- modm_utils -->
<g id="node32" class="node">
<title>modm_utils</title>
<g id="a_node32"><a xlink:href="../modm-utils" xlink:title="modm:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="2921,-45.5 2865,-45.5 2865,-7.5 2921,-7.5 2921,-45.5"/>
<text text-anchor="middle" x="2893" y="-30.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="2893" y="-15.3" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_utils&#45;&gt;modm_architecture -->
<g id="edge32" class="edge">
<title>modm_utils&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M2871.3256,-45.552C2866.8523,-48.5593 2861.9785,-51.2496 2857,-53 2735.7234,-95.6406 1814.5093,-105.7778 1587.4557,-107.6386"/>
<polygon fill="#000000" stroke="#000000" points="1587.1459,-104.1409 1577.1743,-107.721 1587.2021,-111.1406 1587.1459,-104.1409"/>
</g>
</g>
</svg>

