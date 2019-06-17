!!! warning "These module docs are in beta and may be incomplete."

# modm:math: Math



## Content

```cpp
// Class
class modm::Angle;
class modm::Circle2D< typename T >;
class modm::LUDecomposition::LUSubDecomposition< T, OFFSET, WIDTH, OFFSET >;
class modm::LUDecomposition::LUSubDecomposition< typename T , uint8_t OFFSET, uint8_t HEIGHT, uint8_t WIDTH >;
class modm::LUDecomposition::RowOperation< T, 0 >;
class modm::LUDecomposition::RowOperation< typename T , uint8_t HEIGHT >;
class modm::LUDecomposition;
class modm::Line2D< typename T >;
class modm::LineSegment2D< typename T=int16_t >;
class modm::Location2D< typename T=int16_t >;
class modm::Matrix< typename T, uint8_t ROWS, uint8_t COLUMNS >;
class modm::Pid< typename T , unsigned int ScaleFactor=1 >;
class modm::PointSet2D< typename T >;
class modm::Polygon2D< typename T >;
class modm::Quaternion< class T >;
class modm::Ray2D< typename T=int16_t >;
class modm::SCurveController< typename T  >;
class modm::SCurveGenerator< typename T  >;
class modm::Saturated< typename T >;
class modm::Tolerance;
class modm::Vector< T, 1 >;
class modm::Vector< T, 2 >;
class modm::Vector< T, 3 >;
class modm::Vector< T, 4 >;
class modm::Vector< typename T, uint8_t N >;
class modm::filter::Debounce< typename T =uint8_t >;
class modm::filter::Fir< typename T , int N, int BLOCK_SIZE, signed int ScaleFactor=1 >;
class modm::filter::Median< typename T, int N >;
class modm::filter::MovingAverage< typename T , std::size_t N >;
class modm::filter::Ramp< typename T  >;
class modm::interpolation::Lagrange< typename T, template< typename > class Accessor=::modm::accessor::Ram >;
class modm::interpolation::Linear< typename T, template< typename > class Accessor=::modm::accessor::Ram >;
// Struct
struct modm::ArithmeticTraits< typename T  >;
struct modm::GeometricTraits< double >;
struct modm::GeometricTraits< float >;
struct modm::GeometricTraits< int16_t >;
struct modm::GeometricTraits< int32_t >;
struct modm::GeometricTraits< int8_t >;
struct modm::GeometricTraits< typename T  >;
struct modm::GeometricTraits< uint8_t >;
struct modm::Pid::Parameter;
struct modm::SCurveController::Parameter;
struct modm::detail::MakeSigned< Vector< T, N > >;
struct modm::detail::MakeUnsigned< Vector< T, N > >;
struct modm::detail::WideType< Vector< T, N > >;
// Function
IOStream& modm::operator<<(IOStream &, const Matrix< T, WIDTH, HEIGHT > &);
IOStream& modm::operator<<(IOStream &os, const Location2D< T > &l);
IOStream& modm::operator<<(IOStream &os, const Vector< U, 2 > &c);
Quaternion<T> modm::operator*(const T &lhs, const Quaternion< T > &rhs);
Saturated<U> modm::abs(const Saturated< U > &x);
Saturated<U> modm::operator+(const Saturated< U > &a, const Saturated< U > &b);
Saturated<U> modm::operator-(const Saturated< U > &a, const Saturated< U > &b);
Saturated<U> modm::operator-(const Saturated< U > &x);
T modm::determinant(const modm::Matrix< T, 1, 1 > &m);
T modm::determinant(const modm::Matrix< T, 2, 2 > &m);
T modm::determinant(const modm::Matrix< T, N, N > &m);
Vector<U, 2> modm::operator*(float scale, const Vector< U, 2 > &vector);
Vector<U, 2> modm::operator/(float scale, const Vector< U, 2 > &vector);
bool modm::isPositive(const float &a);
bool modm::operator!=(const Saturated< U > &a, const Saturated< U > &b);
bool modm::operator==(const Saturated< U > &a, const Saturated< U > &b);
const T& modm::max(const T &a, const T &b);
const T& modm::max(const T &a, const T &b, Compare compare);
const T& modm::min(const T &a, const T &b);
const T& modm::min(const T &a, const T &b, Compare compare);
constexpr T modm::max(const T a, const T b, const T c);
constexpr baudrate_t modm::Bd(T value);
constexpr baudrate_t modm::MBd(T value);
constexpr baudrate_t modm::kBd(T value);
constexpr bitrate_t modm::Mbps(T value);
constexpr bitrate_t modm::bps(T value);
constexpr bitrate_t modm::kbps(T value);
constexpr bool modm::isBigEndian();
constexpr bool modm::isLittleEndian();
constexpr frequency_t modm::Hz(T value);
constexpr frequency_t modm::MHz(T value);
constexpr frequency_t modm::kHz(T value);
constexpr int8_t modm::leftmostBit(uint32_t value);
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
constexpr uint32_t modm::pow(uint32_t base, uint8_t exponent);
int16_t modm::fromBigEndian(int16_t value);
int16_t modm::fromLittleEndian(int16_t value);
int16_t modm::toBigEndian(int16_t value);
int16_t modm::toLittleEndian(int16_t value);
int32_t modm::fromBigEndian(int32_t value);
int32_t modm::fromLittleEndian(int32_t value);
int32_t modm::math::mac(int32_t result, int16_t a, int16_t b);
int32_t modm::math::mul(int16_t a, int16_t b);
int32_t modm::toBigEndian(int32_t value);
int32_t modm::toLittleEndian(int32_t value);
static Vector<T, 1> modm::operator*(const U &lhs, const Vector< T, 1 > &rhs);
static Vector<T, 3> modm::operator*(const U &lhs, const Vector< T, 3 > &rhs);
static Vector<T,4> modm::operator*(const U &lhs, const Vector< T, 4 > &rhs);
static Vector<U, 3> modm::operator*(const Matrix< T, 3, 3 > &lhs, const Vector< U, 3 > &rhs);
static Vector<U,4> modm::operator*(const Matrix< T, 4, 4 > &lhs, const Vector< U, 4 > &rhs);
static constexpr float modm::toDegree(float angle);
static constexpr float modm::toRadian(float angle);
std::size_t modm::bitCount(uint16_t n);
std::size_t modm::bitCount(uint32_t n);
std::size_t modm::bitCount(uint8_t n);
uint16_t modm::bitReverse(uint16_t n);
uint16_t modm::fromBigEndian(uint16_t value);
uint16_t modm::fromLittleEndian(uint16_t value);
uint16_t modm::math::sqrt(uint32_t a);
uint16_t modm::swap(uint16_t n);
uint16_t modm::toBigEndian(uint16_t value);
uint16_t modm::toLittleEndian(uint16_t value);
uint16_t modm__sqrt32(uint32_t a);
uint32_t modm::bitReverse(uint32_t n);
uint32_t modm::fromBigEndian(uint32_t value);
uint32_t modm::fromLittleEndian(uint32_t value);
uint32_t modm::math::crc32(const uint8_t *data, size_t length);
uint32_t modm::math::mul(uint16_t a, uint16_t b);
uint32_t modm::swap(uint32_t n);
uint32_t modm::toBigEndian(uint32_t value);
uint32_t modm::toLittleEndian(uint32_t value);
uint8_t modm::bitReverse(uint8_t n);
uint8_t modm::swap(uint8_t n);
void modm::swap(int16_t &a, int16_t &b);
void modm::swap(uint8_t &a, uint8_t &b);
// Variable
constexpr uint16_t modm::Bit10 = (1ul << 10);
constexpr uint16_t modm::Bit11 = (1ul << 11);
constexpr uint16_t modm::Bit12 = (1ul << 12);
constexpr uint16_t modm::Bit13 = (1ul << 13);
constexpr uint16_t modm::Bit14 = (1ul << 14);
constexpr uint16_t modm::Bit15 = (1ul << 15);
constexpr uint16_t modm::Bit8 = (1ul <<  8);
constexpr uint16_t modm::Bit9 = (1ul <<  9);
constexpr uint32_t modm::Bit16 = (1ul << 16);
constexpr uint32_t modm::Bit17 = (1ul << 17);
constexpr uint32_t modm::Bit18 = (1ul << 18);
constexpr uint32_t modm::Bit19 = (1ul << 19);
constexpr uint32_t modm::Bit20 = (1ul << 20);
constexpr uint32_t modm::Bit21 = (1ul << 21);
constexpr uint32_t modm::Bit22 = (1ul << 22);
constexpr uint32_t modm::Bit23 = (1ul << 23);
constexpr uint32_t modm::Bit24 = (1ul << 24);
constexpr uint32_t modm::Bit25 = (1ul << 25);
constexpr uint32_t modm::Bit26 = (1ul << 26);
constexpr uint32_t modm::Bit27 = (1ul << 27);
constexpr uint32_t modm::Bit28 = (1ul << 28);
constexpr uint32_t modm::Bit29 = (1ul << 29);
constexpr uint32_t modm::Bit30 = (1ul << 30);
constexpr uint32_t modm::Bit31 = (1ul << 31);
constexpr uint8_t modm::Bit0 = (1ul <<  0);
constexpr uint8_t modm::Bit1 = (1ul <<  1);
constexpr uint8_t modm::Bit2 = (1ul <<  2);
constexpr uint8_t modm::Bit3 = (1ul <<  3);
constexpr uint8_t modm::Bit4 = (1ul <<  4);
constexpr uint8_t modm::Bit5 = (1ul <<  5);
constexpr uint8_t modm::Bit6 = (1ul <<  6);
constexpr uint8_t modm::Bit7 = (1ul <<  7);
// Typedef
typedef Matrix<float, 1, 1> modm::Matrix1f;
typedef Matrix<float, 2, 2> modm::Matrix2f;
typedef Matrix<float, 3, 3> modm::Matrix3f;
typedef Matrix<float, 4, 4> modm::Matrix4f;
typedef Vector<float, 1> modm::Vector1f;
typedef Vector<float, 2> modm::Vector2f;
typedef Vector<float, 3> modm::Vector3f;
typedef Vector<float, 4> modm::Vector4f;
typedef Vector<int16_t, 1> modm::Vector1i;
typedef Vector<int16_t, 2> modm::Vector2i;
typedef Vector<int16_t, 3> modm::Vector3i;
typedef Vector<int16_t, 4> modm::Vector4i;
typedef Vector<uint16_t, 2> modm::Vector2u;
typedef Vector<uint16_t, 3> modm::Vector3u;
typedef Vector<uint16_t, 4> modm::Vector4u;
using modm::SignedType = typedef typename detail::MakeSigned<T>::type;
using modm::UnsignedType = typedef typename detail::MakeUnsigned<T>::type;
using modm::WideType = typedef typename detail::WideType<T>::type;
using modm::baudrate_t = typedef uint32_t;
using modm::bitrate_t = typedef uint32_t;
using modm::frequency_t = typedef uint32_t;
using modm::percent_t = typedef uint16_t;
// Define
#define IMPLEMENT_VECTOR_ACCESSOR2(a, b)
#define IMPLEMENT_VECTOR_ACCESSOR3(a, b, c)
#define IMPLEMENT_VECTOR_ACCESSOR4(a, b, c, d)
#define M_1_PI
#define M_2_PI
#define M_PI
#define M_PI_2
#define M_PI_4
#define M_SQRT2
#define M_TWOPI
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:math Pages: 1 -->
<svg width="486pt" height="224pt"
 viewBox="0.00 0.00 486.00 224.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 220)">
<title>modm:math</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-220 482,-220 482,4 -4,4"/>
<!-- modm_math -->
<g id="node1" class="node">
<title>modm_math</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="275,-127 219,-127 219,-89 275,-89 275,-127"/>
<text text-anchor="middle" x="247" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="247" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">math</text>
</g>
<!-- modm_math_units -->
<g id="node2" class="node">
<title>modm_math_units</title>
<g id="a_node2"><a xlink:href="../modm-math-units" xlink:title="modm:&#10;math:&#10;units">
<polygon fill="#d3d3d3" stroke="#000000" points="275,-216 219,-216 219,-163 275,-163 275,-216"/>
<text text-anchor="middle" x="247" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="247" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="247" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">units</text>
</a>
</g>
</g>
<!-- modm_math&#45;&gt;modm_math_units -->
<g id="edge1" class="edge">
<title>modm_math&#45;&gt;modm_math_units</title>
<path fill="none" stroke="#000000" d="M241.1601,-127.3144C240.4434,-134.9606 240.15,-144.0143 240.2799,-152.8129"/>
<polygon fill="#000000" stroke="#000000" points="236.7859,-153.0508 240.6269,-162.9249 243.7818,-152.8107 236.7859,-153.0508"/>
</g>
<!-- modm_math_units&#45;&gt;modm_math -->
<g id="edge7" class="edge">
<title>modm_math_units&#45;&gt;modm_math</title>
<path fill="none" stroke="#000000" d="M253.3731,-162.9249C253.8243,-154.762 253.8704,-145.7322 253.5114,-137.3922"/>
<polygon fill="#000000" stroke="#000000" points="256.997,-137.0595 252.8399,-127.3144 250.0125,-137.5249 256.997,-137.0595"/>
</g>
<!-- modm_math_filter -->
<g id="node3" class="node">
<title>modm_math_filter</title>
<g id="a_node3"><a xlink:href="../modm-math-filter" xlink:title="modm:&#10;math:&#10;filter">
<polygon fill="#d3d3d3" stroke="#000000" points="56,-53 0,-53 0,0 56,0 56,-53"/>
<text text-anchor="middle" x="28" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="28" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="28" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">filter</text>
</a>
</g>
</g>
<!-- modm_math_filter&#45;&gt;modm_math -->
<g id="edge2" class="edge">
<title>modm_math_filter&#45;&gt;modm_math</title>
<path fill="none" stroke="#000000" d="M56.0977,-48.0213C59.0265,-49.8361 62.0228,-51.5342 65,-53 112.1,-76.1891 170.925,-91.7937 208.7261,-100.2873"/>
<polygon fill="#000000" stroke="#000000" points="208.2859,-103.7737 218.8033,-102.4947 209.7838,-96.9359 208.2859,-103.7737"/>
</g>
<!-- modm_math_geometry -->
<g id="node4" class="node">
<title>modm_math_geometry</title>
<g id="a_node4"><a xlink:href="../modm-math-geometry" xlink:title="modm:&#10;math:&#10;geometry">
<polygon fill="#d3d3d3" stroke="#000000" points="142,-53 74,-53 74,0 142,0 142,-53"/>
<text text-anchor="middle" x="108" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="108" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="108" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">geometry</text>
</a>
</g>
</g>
<!-- modm_math_geometry&#45;&gt;modm_math -->
<g id="edge3" class="edge">
<title>modm_math_geometry&#45;&gt;modm_math</title>
<path fill="none" stroke="#000000" d="M142.2074,-47.7177C145.174,-49.5164 148.1356,-51.2974 151,-53 170.2631,-64.4498 191.9063,-76.8862 209.7598,-87.0321"/>
<polygon fill="#000000" stroke="#000000" points="208.2926,-90.2237 218.7179,-92.1116 211.7454,-84.1344 208.2926,-90.2237"/>
</g>
<!-- modm_math_interpolation -->
<g id="node5" class="node">
<title>modm_math_interpolation</title>
<g id="a_node5"><a xlink:href="../modm-math-interpolation" xlink:title="modm:&#10;math:&#10;interpolation">
<polygon fill="#d3d3d3" stroke="#000000" points="246,-53 160,-53 160,0 246,0 246,-53"/>
<text text-anchor="middle" x="203" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="203" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="203" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">interpolation</text>
</a>
</g>
</g>
<!-- modm_math_interpolation&#45;&gt;modm_math -->
<g id="edge4" class="edge">
<title>modm_math_interpolation&#45;&gt;modm_math</title>
<path fill="none" stroke="#000000" d="M217.4072,-53.1861C222.0087,-61.7092 227.1152,-71.1679 231.7729,-79.7952"/>
<polygon fill="#000000" stroke="#000000" points="228.7752,-81.6102 236.6057,-88.7469 234.9348,-78.2847 228.7752,-81.6102"/>
</g>
<!-- modm_math_matrix -->
<g id="node6" class="node">
<title>modm_math_matrix</title>
<g id="a_node6"><a xlink:href="../modm-math-matrix" xlink:title="modm:&#10;math:&#10;matrix">
<polygon fill="#d3d3d3" stroke="#000000" points="320,-53 264,-53 264,0 320,0 320,-53"/>
<text text-anchor="middle" x="292" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="292" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="292" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">matrix</text>
</a>
</g>
</g>
<!-- modm_math_matrix&#45;&gt;modm_math -->
<g id="edge5" class="edge">
<title>modm_math_matrix&#45;&gt;modm_math</title>
<path fill="none" stroke="#000000" d="M277.2653,-53.1861C272.5593,-61.7092 267.3368,-71.1679 262.5732,-79.7952"/>
<polygon fill="#000000" stroke="#000000" points="259.4002,-78.3009 257.6305,-88.7469 265.5282,-81.6845 259.4002,-78.3009"/>
</g>
<!-- modm_math_saturated -->
<g id="node7" class="node">
<title>modm_math_saturated</title>
<g id="a_node7"><a xlink:href="../modm-math-saturated" xlink:title="modm:&#10;math:&#10;saturated">
<polygon fill="#d3d3d3" stroke="#000000" points="403.5,-53 338.5,-53 338.5,0 403.5,0 403.5,-53"/>
<text text-anchor="middle" x="371" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="371" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="371" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">saturated</text>
</a>
</g>
</g>
<!-- modm_math_saturated&#45;&gt;modm_math -->
<g id="edge6" class="edge">
<title>modm_math_saturated&#45;&gt;modm_math</title>
<path fill="none" stroke="#000000" d="M338.4205,-47.9132C321.6778,-58.9174 301.2088,-72.3708 283.887,-83.7557"/>
<polygon fill="#000000" stroke="#000000" points="281.5863,-81.0795 275.1521,-89.4968 285.4311,-86.9292 281.5863,-81.0795"/>
</g>
<!-- modm_math_utils -->
<g id="node8" class="node">
<title>modm_math_utils</title>
<g id="a_node8"><a xlink:href="../modm-math-utils" xlink:title="modm:&#10;math:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="478,-53 422,-53 422,0 478,0 478,-53"/>
<text text-anchor="middle" x="450" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="450" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="450" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_math_utils&#45;&gt;modm_math -->
<g id="edge8" class="edge">
<title>modm_math_utils&#45;&gt;modm_math</title>
<path fill="none" stroke="#000000" d="M421.8611,-47.939C418.9406,-49.7704 415.9576,-51.4949 413,-53 371.3138,-74.2137 319.6812,-89.8137 285.1714,-98.8738"/>
<polygon fill="#000000" stroke="#000000" points="283.8567,-95.5977 275.0412,-101.4745 285.5974,-102.3779 283.8567,-95.5977"/>
</g>
</g>
</svg>

