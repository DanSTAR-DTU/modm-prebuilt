!!! warning "These module docs are in beta and may be incomplete."

# modm:processing: Processing

Constructs to influence program flow execution, timing, synchronization
and delegation.

## Content

```cpp
// Class
class modm::GenericPeriodicTimer< class Clock , typename TimestampType =modm::Timestamp >;
class modm::GenericTimeout< class Clock, class TimestampType=modm::Timestamp >;
class modm::GenericTimestamp< typename T >;
class modm::NestedResumable< uint8_t Levels=1 >;
class modm::Resumable< uint8_t Functions=1 >;
class modm::Task;
class modm::pt::Protothread;
class modm::pt::Semaphore;
// Struct
struct modm::ResumableResult< typename T >;
// Function
IOStream& modm::operator<<(IOStream &os, const GenericTimestamp< T > &t);
// Enum
enum PeriodicTimerState;
enum TimeoutState;
// Typedef
using modm::PeriodicTimer = typedef GenericPeriodicTimer< ::modm::Clock, Timestamp>;
using modm::ShortPeriodicTimer = typedef GenericPeriodicTimer< ::modm::Clock, ShortTimestamp>;
using modm::ShortTimeout = typedef GenericTimeout< ::modm::Clock, ShortTimestamp>;
using modm::ShortTimestamp = typedef GenericTimestamp<uint16_t>;
using modm::Timeout = typedef GenericTimeout< ::modm::Clock, Timestamp>;
using modm::Timestamp = typedef GenericTimestamp<uint32_t>;
// Define
#define MODM_RESUMABLE_MODULE_NAME
#define PT_BEGIN
#define PT_CALL(resumable)
#define PT_END
#define PT_EXIT
#define PT_RESTART
#define PT_SPAWN(child)
#define PT_WAIT_THREAD(child)
#define PT_WAIT_UNTIL(condition)
#define PT_WAIT_WHILE(condition)
#define PT_YIELD
#define RF_BEGIN
#define RF_BEGIN(index)
#define RF_CALL(resumable)
#define RF_CALL_BLOCKING(resumable)
#define RF_END
#define RF_END_RETURN(result)
#define RF_END_RETURN_CALL(resumable)
#define RF_RETURN
#define RF_RETURN(result)
#define RF_RETURN_CALL(resumable)
#define RF_WAIT_THREAD(child)
#define RF_WAIT_UNTIL(condition)
#define RF_WAIT_WHILE(condition)
#define RF_YIELD
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:processing Pages: 1 -->
<svg width="280pt" height="135pt"
 viewBox="0.00 0.00 279.50 135.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 131)">
<title>modm:processing</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-131 275.5,-131 275.5,4 -4,4"/>
<!-- modm_processing -->
<g id="node1" class="node">
<title>modm_processing</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="173.5,-127 99.5,-127 99.5,-89 173.5,-89 173.5,-127"/>
<text text-anchor="middle" x="136.5" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="136.5" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing</text>
</g>
<!-- modm_processing_protothread -->
<g id="node2" class="node">
<title>modm_processing_protothread</title>
<g id="a_node2"><a xlink:href="../modm-processing-protothread" xlink:title="modm:&#10;processing:&#10;protothread">
<polygon fill="#d3d3d3" stroke="#000000" points="79,-53 0,-53 0,0 79,0 79,-53"/>
<text text-anchor="middle" x="39.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="39.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="39.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">protothread</text>
</a>
</g>
</g>
<!-- modm_processing_protothread&#45;&gt;modm_processing -->
<g id="edge1" class="edge">
<title>modm_processing_protothread&#45;&gt;modm_processing</title>
<path fill="none" stroke="#000000" d="M71.2614,-53.1861C82.3767,-62.5253 94.8289,-72.9877 105.8514,-82.2489"/>
<polygon fill="#000000" stroke="#000000" points="103.6775,-84.9938 113.5853,-88.7469 108.1805,-79.6344 103.6775,-84.9938"/>
</g>
<!-- modm_processing_resumable -->
<g id="node3" class="node">
<title>modm_processing_resumable</title>
<g id="a_node3"><a xlink:href="../modm-processing-resumable" xlink:title="modm:&#10;processing:&#10;resumable">
<polygon fill="#d3d3d3" stroke="#000000" points="175.5,-53 97.5,-53 97.5,0 175.5,0 175.5,-53"/>
<text text-anchor="middle" x="136.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="136.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="136.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">resumable</text>
</a>
</g>
</g>
<!-- modm_processing_resumable&#45;&gt;modm_processing -->
<g id="edge2" class="edge">
<title>modm_processing_resumable&#45;&gt;modm_processing</title>
<path fill="none" stroke="#000000" d="M136.5,-53.1861C136.5,-61.3465 136.5,-70.3646 136.5,-78.6895"/>
<polygon fill="#000000" stroke="#000000" points="133.0001,-78.7469 136.5,-88.7469 140.0001,-78.747 133.0001,-78.7469"/>
</g>
<!-- modm_processing_timer -->
<g id="node4" class="node">
<title>modm_processing_timer</title>
<g id="a_node4"><a xlink:href="../modm-processing-timer" xlink:title="modm:&#10;processing:&#10;timer">
<polygon fill="#d3d3d3" stroke="#000000" points="271.5,-53 193.5,-53 193.5,0 271.5,0 271.5,-53"/>
<text text-anchor="middle" x="232.5" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="232.5" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="232.5" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">timer</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_processing -->
<g id="edge3" class="edge">
<title>modm_processing_timer&#45;&gt;modm_processing</title>
<path fill="none" stroke="#000000" d="M201.066,-53.1861C190.0653,-62.5253 177.7415,-72.9877 166.8326,-82.2489"/>
<polygon fill="#000000" stroke="#000000" points="164.5366,-79.6069 159.1785,-88.7469 169.067,-84.9432 164.5366,-79.6069"/>
</g>
</g>
</svg>
