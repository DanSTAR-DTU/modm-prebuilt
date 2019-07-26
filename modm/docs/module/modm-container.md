!!! warning "These module docs are in beta and may be incomplete."

# modm:container: Containers

A container is a holder object that stores a collection other objects (its
elements). They are implemented as class templates, which allows a great
flexibility in the types supported as elements.

Many containers have several member functions in common, and share
functionalities. The decision of which type of container to use for a specific
need does not generally depend only on the functionality offered by the
container, but also on the efficiency of some of its members (complexity).
This is especially true for sequence containers, which offer different
trade-offs in complexity between inserting/removing elements and accessing
them.

`modm::Stack` and `modm::Queue` are implemented as container adapters. Container
adapters are not full container classes, but classes that provide a specific
interface relying on an object of one of the container classes (such as
`modm::BoundedDeque` or `modm::LinkedList`) to handle the elements.
The underlying container is encapsulated in such a way that its elements are
accessed by the members of the container class independently of the underlying
container class used.

Sequence containers:

- `modm::DynamicArray`
- `modm::LinkedList`
- `modm::DoublyLinkedList`
- `modm::BoundedDeque`

Container adapters:

- `modm::Queue`
- `modm::Stack`
- `modm::BoundedStack`
- `modm::BoundedQueue`

Other:

- `modm::SmartPointer`
- `modm::Pair`

Two special containers hiding in the `modm:architecture:atomic` module:

- `modm::atomic::Queue`
- `modm::atomic::Container`

The first is a simple, interrupt-safe queue (but only for the AVRs).
Whenever you need to exchange data between a interrupt routine and the normal
program consider using this queue.

The atomic container wraps objects and provides atomic access to
them. This comes in handy when simple objects are accessed by an interrupt
and the main program. The container provides secure access without much work
in this case.

## Generic Interface

All implementation share a common set of function. Not every container implement
every one of it, only a subset which is sufficient for the container.

- append
- prepend
- (insert)
- removeFront
- removeBack
- (remove)
- getFront
- getBack

- push (only Stack and Queue)
- pop (only Stack and Queue)
- get (only Stack and Queue)

- at
- operator[]

- clear

- isEmpty
- isFull
- getSize
- getMaxSize
- getCapacity

## Content

```cpp
// Class
class modm::BoundedDeque::const_iterator;
class modm::BoundedDeque< typename T , std::size_t N >;
class modm::BoundedQueue< typename T , std::size_t N, typename Container =BoundedDeque<T, N> >;
class modm::BoundedStack< typename T , std::size_t N, typename Container =BoundedDeque<T, N> >;
class modm::DoublyLinkedList::const_iterator;
class modm::DoublyLinkedList::iterator;
class modm::DoublyLinkedList< typename T , typename Allocator =allocator::Dynamic<T> >;
class modm::DynamicArray::const_iterator;
class modm::DynamicArray::iterator;
class modm::DynamicArray< typename T, typename Allocator=allocator::Dynamic<T> >;
class modm::LinkedList::const_iterator;
class modm::LinkedList::iterator;
class modm::LinkedList< typename T , typename Allocator =allocator::Dynamic<T> >;
class modm::Pair< typename T1, typename T2 >;
class modm::Queue< typename T , typename Container  >;
class modm::SmartPointer;
class modm::Stack< typename T , typename Container  >;
// Struct
struct modm::DoublyLinkedList::Node;
struct modm::LinkedList::Node;
// Function
modm::IOStream& modm::operator<<(modm::IOStream &s, const modm::SmartPointer &sPtr);
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:container Pages: 1 -->
<svg width="266pt" height="209pt"
 viewBox="0.00 0.00 266.00 209.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 205)">
<title>modm:container</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-205 262,-205 262,4 -4,4"/>
<!-- modm_container -->
<g id="node1" class="node">
<title>modm_container</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="162.5,-127 95.5,-127 95.5,-89 162.5,-89 162.5,-127"/>
<text text-anchor="middle" x="129" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="129" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">container</text>
</g>
<!-- modm_architecture -->
<g id="node2" class="node">
<title>modm_architecture</title>
<g id="a_node2"><a xlink:href="../modm-architecture" xlink:title="modm:&#10;architecture">
<polygon fill="#d3d3d3" stroke="#000000" points="83,-201 3,-201 3,-163 83,-163 83,-201"/>
<text text-anchor="middle" x="43" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="43" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_architecture -->
<g id="edge1" class="edge">
<title>modm_container&#45;&gt;modm_architecture</title>
<path fill="none" stroke="#000000" d="M106.8529,-127.0568C96.6007,-135.8785 84.2618,-146.4957 73.1749,-156.0355"/>
<polygon fill="#000000" stroke="#000000" points="70.6683,-153.575 65.371,-162.7505 75.234,-158.8811 70.6683,-153.575"/>
</g>
<!-- modm_io -->
<g id="node3" class="node">
<title>modm_io</title>
<g id="a_node3"><a xlink:href="../modm-io" xlink:title="modm:&#10;io">
<polygon fill="#d3d3d3" stroke="#000000" points="157,-201 101,-201 101,-163 157,-163 157,-201"/>
<text text-anchor="middle" x="129" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="129" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">io</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_io -->
<g id="edge2" class="edge">
<title>modm_container&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M129,-127.0568C129,-134.8507 129,-144.0462 129,-152.6619"/>
<polygon fill="#000000" stroke="#000000" points="125.5001,-152.7505 129,-162.7505 132.5001,-152.7506 125.5001,-152.7505"/>
</g>
<!-- modm_utils -->
<g id="node4" class="node">
<title>modm_utils</title>
<g id="a_node4"><a xlink:href="../modm-utils" xlink:title="modm:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="231,-201 175,-201 175,-163 231,-163 231,-201"/>
<text text-anchor="middle" x="203" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="203" y="-170.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_utils -->
<g id="edge3" class="edge">
<title>modm_container&#45;&gt;modm_utils</title>
<path fill="none" stroke="#000000" d="M148.0568,-127.0568C156.7072,-135.7072 167.0841,-146.0841 176.4786,-155.4786"/>
<polygon fill="#000000" stroke="#000000" points="174.2046,-158.1543 183.7505,-162.7505 179.1543,-153.2046 174.2046,-158.1543"/>
</g>
<!-- modm_math_geometry -->
<g id="node5" class="node">
<title>modm_math_geometry</title>
<g id="a_node5"><a xlink:href="../modm-math-geometry" xlink:title="modm:&#10;math:&#10;geometry">
<polygon fill="#d3d3d3" stroke="#000000" points="68,-53 0,-53 0,0 68,0 68,-53"/>
<text text-anchor="middle" x="34" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="34" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="34" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">geometry</text>
</a>
</g>
</g>
<!-- modm_math_geometry&#45;&gt;modm_container -->
<g id="edge4" class="edge">
<title>modm_math_geometry&#45;&gt;modm_container</title>
<path fill="none" stroke="#000000" d="M65.1066,-53.1861C75.8869,-62.4346 87.9514,-72.7846 98.6685,-81.9788"/>
<polygon fill="#000000" stroke="#000000" points="96.6891,-84.8921 106.5578,-88.7469 101.2469,-79.5793 96.6891,-84.8921"/>
</g>
<!-- modm_math_interpolation -->
<g id="node6" class="node">
<title>modm_math_interpolation</title>
<g id="a_node6"><a xlink:href="../modm-math-interpolation" xlink:title="modm:&#10;math:&#10;interpolation">
<polygon fill="#d3d3d3" stroke="#000000" points="172,-53 86,-53 86,0 172,0 172,-53"/>
<text text-anchor="middle" x="129" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="129" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="129" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">interpolation</text>
</a>
</g>
</g>
<!-- modm_math_interpolation&#45;&gt;modm_container -->
<g id="edge5" class="edge">
<title>modm_math_interpolation&#45;&gt;modm_container</title>
<path fill="none" stroke="#000000" d="M129,-53.1861C129,-61.3465 129,-70.3646 129,-78.6895"/>
<polygon fill="#000000" stroke="#000000" points="125.5001,-78.7469 129,-88.7469 132.5001,-78.747 125.5001,-78.7469"/>
</g>
<!-- modm_platform_i2c -->
<g id="node7" class="node">
<title>modm_platform_i2c</title>
<g id="a_node7"><a xlink:href="../modm-platform-i2c" xlink:title="modm:&#10;platform:&#10;i2c">
<polygon fill="#d3d3d3" stroke="#000000" points="258,-53 190,-53 190,0 258,0 258,-53"/>
<text text-anchor="middle" x="224" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="224" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">platform:</text>
<text text-anchor="middle" x="224" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c</text>
</a>
</g>
</g>
<!-- modm_platform_i2c&#45;&gt;modm_container -->
<g id="edge6" class="edge">
<title>modm_platform_i2c&#45;&gt;modm_container</title>
<path fill="none" stroke="#000000" d="M192.8934,-53.1861C182.1131,-62.4346 170.0486,-72.7846 159.3315,-81.9788"/>
<polygon fill="#000000" stroke="#000000" points="156.7531,-79.5793 151.4422,-88.7469 161.3109,-84.8921 156.7531,-79.5793"/>
</g>
</g>
</svg>
