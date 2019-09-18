!!! warning "These module docs are in beta and may be incomplete."

# modm:architecture:memory: Memory Traits



## Content

```cpp
// Function
void* operator new(size_t size, modm::MemoryTraits traits) noexcept;
void* operator new[](size_t size, modm::MemoryTraits traits) noexcept;
// Enum
enum MemoryTrait;
// Variable
constexpr MemoryTraits modm::MemoryBackup = (MemoryTrait::AccessSBus | MemoryTrait::TypeNonVolatile);
constexpr MemoryTraits modm::MemoryDMA = (MemoryTrait::AccessSBus | MemoryTrait::AccessDMA);
constexpr MemoryTraits modm::MemoryDMA2D = (MemoryTrait::AccessSBus | MemoryTrait::AccessDMA2D);
constexpr MemoryTraits modm::MemoryDefault = MemoryDMA;
constexpr MemoryTraits modm::MemoryExternal = (MemoryTrait::AccessSBus | MemoryTrait::TypeExternal);
constexpr MemoryTraits modm::MemoryFastCode = (MemoryTrait::AccessIBus | MemoryTrait::TypeCoreCoupled);
constexpr MemoryTraits modm::MemoryFastData = (MemoryTrait::AccessDBus | MemoryTrait::TypeCoreCoupled);
// Typedef
using modm::MemoryTraits = typedef Flags16<MemoryTrait>;
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:architecture:memory Pages: 1 -->
<svg width="190pt" height="239pt"
 viewBox="0.00 0.00 190.00 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:architecture:memory</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-235 186,-235 186,4 -4,4"/>
<!-- modm_architecture_memory -->
<g id="node1" class="node">
<title>modm_architecture_memory</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="132,-142 48,-142 48,-89 132,-89 132,-142"/>
<text text-anchor="middle" x="90" y="-126.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="90" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="90" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">memory</text>
</g>
<!-- modm_architecture -->
<g id="node2" class="node">
<title>modm_architecture</title>
<g id="a_node2"><a xlink:href="../modm-architecture" xlink:title="modm:&#10;architecture">
<polygon fill="#d3d3d3" stroke="#000000" points="80,-223.5 0,-223.5 0,-185.5 80,-185.5 80,-223.5"/>
<text text-anchor="middle" x="40" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="40" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">architecture</text>
</a>
</g>
</g>
<!-- modm_architecture_memory&#45;&gt;modm_architecture -->
<g id="edge1" class="edge">
<title>modm_architecture_memory&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M74.9984,-142.2029C68.8857,-153.0834 61.8183,-165.6634 55.6417,-176.6579"/>
<polygon fill="#000000" stroke="#000000" points="52.5498,-175.0155 50.7032,-185.4482 58.6527,-178.4441 52.5498,-175.0155"/>
</g>
<!-- modm_architecture_register -->
<g id="node3" class="node">
<title>modm_architecture_register</title>
<g id="a_node3"><a xlink:href="../modm-architecture-register" xlink:title="modm:&#10;architecture:&#10;register">
<polygon fill="#d3d3d3" stroke="#000000" points="182,-231 98,-231 98,-178 182,-178 182,-231"/>
<text text-anchor="middle" x="140" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="140" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="140" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">register</text>
</a>
</g>
</g>
<!-- modm_architecture_memory&#45;&gt;modm_architecture_register -->
<g id="edge2" class="edge">
<title>modm_architecture_memory&#45;&gt;modm_architecture_register</title>
<path fill="none" stroke="#000000" d="M105.0016,-142.2029C109.7479,-150.6513 115.0698,-160.1243 120.1034,-169.0841"/>
<polygon fill="#000000" stroke="#000000" points="117.0595,-170.8118 125.0089,-177.8159 123.1624,-167.3832 117.0595,-170.8118"/>
</g>
<!-- modm_platform_cortex_m -->
<g id="node4" class="node">
<title>modm_platform_cortex_m</title>
<g id="a_node4"><a xlink:href="../modm-platform-cortex-m" xlink:title="modm:&#10;platform:&#10;cortex&#45;m">
<polygon fill="#d3d3d3" stroke="#000000" points="124,-53 56,-53 56,0 124,0 124,-53"/>
<text text-anchor="middle" x="90" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="90" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="90" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">cortex&#45;m</text>
</a>
</g>
</g>
<!-- modm_platform_cortex_m&#45;&gt;modm_architecture_memory -->
<g id="edge3" class="edge">
<title>modm_platform_cortex_m&#45;&gt;modm_architecture_memory</title>
<path fill="none" stroke="#000000" d="M90,-53.2029C90,-61.2113 90,-70.1403 90,-78.6802"/>
<polygon fill="#000000" stroke="#000000" points="86.5001,-78.8159 90,-88.8159 93.5001,-78.8159 86.5001,-78.8159"/>
</g>
</g>
</svg>
