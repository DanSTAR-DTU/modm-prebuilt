!!! warning "These module docs are in beta and may be incomplete."

# modm:processing:timer: Software Timers

An implementation of lightweight software timeouts and periodic timers.

- `modm::Timestamp` containing a time in millisecond resolution up to 49 days.
- `modm::Timeout` for timeouts up to 24 days.
- `modm::PeriodicTimer` for periodic timeouts up to 24 days periods.

There are also 16-bit versions of these, in case you need to preserve memory:

- `modm::ShortTimestamp` containing a time in millisecond resolution up to 65 seconds.
- `modm::ShortTimeout` for timeouts up to 32 seconds.
- `modm::ShortPeriodicTimer` for periodic timeouts up to 32 second periods.

These classes default to using `modm::Clock`, which must be implemented on your
system to return a time with millisecond resolution.

You may also create your own timers with custom time bases and resolutions
using these classes:

- `modm::GenericTimestamp`.
- `modm::GenericTimeout`.
- `modm::GenericPeriodicTimer`.

!!! warning
	Never use these classes when a precise timebase is needed!

## Timeouts

The `modm::GenericTimeout` classes allow for a signal to be generated after a
period of time, which can also be used to execute code once after timeout
expiration.

Its behavior can be described by the following annotated waveform:

- C: Default Constructor
- S: (Re-)Start timeout
- E: Timeout Expired
- H: Code handler (`execute()` returned `true`)
- P: Stop timeout

```
Event:    C      S      E     H       P            S          E H
                         _____________                         ______________
Expired:  ______________/             \_______________________/              ...
                  ______                            __________
Armed:    _______/      \__________________________/          \______________...
          _______                      ____________
Stopped:         \____________________/            \_________________________...

                              _                                 _
Handle:   ___________________/ \_______________________________/ \___________...

Remaining:   0  |   +   |      -      |     0      |     +     |   -
```

The default constructor initializes the timeout in the `Stopped` state,
in which `isExpired()` and `execute()` never return `true`.
If you need a timeout to expire immidiately after construction, you need
to explicitly initialize the constructor with time `0`, which has the
same behavior as `restart(0)`.

If you want to execute code once after the timeout expired, poll the
`execute()` method, which returns `true` exactly once after expiration.

```cpp
if (timeout.execute())
{
    // called once after timeout
    Led::toggle();
}
```

Be aware, however, that since this method is polled, it cannot execute
exactly at the time of expiration, but some time after expiration, as
indicated in the above waveform graph.

The `remaining()` time until expiration is signed positive before, and
negative after expiration. This means `Clock::now() + Timeout::remaining()`
will yield the timestamp of the expiration.
If the timeout is stopped, `remaining()` returns zero.

## Periodic Timers

The `modm::GenericPeriodicTimer` class allows for periodic code execution.

Its behavior can be described by the following annotated waveform:

- C: Constructor
- S: (Re-)Start timer
- I: Period interval
- H: Code handler (`execute()` returned `true`)
- P: Stop timer

```
Event:    C         IH        I         I  H      I  S    IH   I    IH  P
                     _         _____________       __      _    ______
Expired:  __________/ \_______/             \_____/  \____/ \__/      \____...
          __________   _______               _____    ____   __        _
Armed:              \_/       \_____________/     \__/    \_/  \______/ \__...
                                                                         __
Stopped:  ______________________________________________________________/  ...

                     _                     _               _         _
Handle:   __________/ \___________________/ \_____________/ \_______/ \____...

Remaining:     +    |0|   +   |     -     |0|  +  | -|  + |0| +| -  |0|+| 0
```
 *
 * If you want to execute code once per period interval, poll the
 * `execute()` method, which returns `true` exactly once after expiration.
 *
 * @code
 * if (timer.execute())
 * {
 *     // periodically called once
 *     Led::toggle();
 * }
 * @endcode
 *
 * Be aware, however, that since this method is polled, it cannot execute
 * exactly at the time of expiration, but some time after expiration, as
 * indicated in the above waveform graph.
 * If one or several periods are missed when polling `execute()`, these
 * code executions are discarded and will not be caught up.
 * Instead, `execute()` returns `true` once and then reschedules itself
 * for the next period, without any period skewing.
 *
 * @warning	Never use this class when a precise timebase is needed!
 *
 * Notice, that the `PeriodicTimerState::Expired` is reset to
 * `PeriodicTimerState::Armed` only after `execute()` has returned `true`.
 * This is different to the behavior of GenericTimeout, where calls to
 * `GenericTimeout::execute()` have no impact on class state.
 *
 * The `remaining()` time until period expiration is signed positive before,
 * and negative after period expiration until `execute()` is called.
 * If the timer is stopped, `remaining()` returns zero.

## Content

```cpp
// Class
class modm::GenericPeriodicTimer< class Clock , typename TimestampType =modm::Timestamp >;
class modm::GenericTimeout< class Clock, class TimestampType=modm::Timestamp >;
class modm::GenericTimestamp< typename T >;
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
```
## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:processing:timer Pages: 1 -->
<svg width="332pt" height="239pt"
 viewBox="0.00 0.00 332.00 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:processing:timer</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-235 328,-235 328,4 -4,4"/>
<!-- modm_processing_timer -->
<g id="node1" class="node">
<title>modm_processing_timer</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="169,-142 91,-142 91,-89 169,-89 169,-142"/>
<text text-anchor="middle" x="130" y="-126.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="130" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="130" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">timer</text>
</g>
<!-- modm_architecture_clock -->
<g id="node2" class="node">
<title>modm_architecture_clock</title>
<g id="a_node2"><a xlink:href="../modm-architecture-clock" xlink:title="modm:&#10;architecture:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="84,-231 0,-231 0,-178 84,-178 84,-231"/>
<text text-anchor="middle" x="42" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="42" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="42" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_architecture_clock -->
<g id="edge1" class="edge">
<title>modm_processing_timer&#45;&gt;modm_architecture_clock</title>
<path fill="none" stroke="#000000" d="M97.3518,-142.2029C88.032,-151.0913 78.0103,-161.1138 69.0573,-170.4797"/>
<polygon fill="#000000" stroke="#000000" points="66.4538,-168.139 62.1401,-177.8159 71.5469,-172.9412 66.4538,-168.139"/>
</g>
<!-- modm_io -->
<g id="node3" class="node">
<title>modm_io</title>
<g id="a_node3"><a xlink:href="../modm-io" xlink:title="modm:&#10;io">
<polygon fill="#d3d3d3" stroke="#000000" points="158,-223.5 102,-223.5 102,-185.5 158,-185.5 158,-223.5"/>
<text text-anchor="middle" x="130" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="130" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">io</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_io -->
<g id="edge2" class="edge">
<title>modm_processing_timer&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M130,-142.2029C130,-152.6482 130,-164.6597 130,-175.3311"/>
<polygon fill="#000000" stroke="#000000" points="126.5001,-175.4482 130,-185.4482 133.5001,-175.4483 126.5001,-175.4482"/>
</g>
<!-- modm_math_utils -->
<g id="node4" class="node">
<title>modm_math_utils</title>
<g id="a_node4"><a xlink:href="../modm-math-utils" xlink:title="modm:&#10;math:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="232,-231 176,-231 176,-178 232,-178 232,-231"/>
<text text-anchor="middle" x="204" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="204" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="204" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_math_utils -->
<g id="edge3" class="edge">
<title>modm_processing_timer&#45;&gt;modm_math_utils</title>
<path fill="none" stroke="#000000" d="M152.2024,-142.2029C159.4464,-150.9153 167.5964,-160.7174 175.2502,-169.9225"/>
<polygon fill="#000000" stroke="#000000" points="172.7286,-172.3643 181.8132,-177.8159 178.1111,-167.8889 172.7286,-172.3643"/>
</g>
<!-- modm_processing -->
<g id="node5" class="node">
<title>modm_processing</title>
<g id="a_node5"><a xlink:href="../modm-processing" xlink:title="modm:&#10;processing">
<polygon fill="#d3d3d3" stroke="#000000" points="324,-223.5 250,-223.5 250,-185.5 324,-185.5 324,-223.5"/>
<text text-anchor="middle" x="287" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="287" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">processing</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_processing -->
<g id="edge4" class="edge">
<title>modm_processing_timer&#45;&gt;modm_processing</title>
<path fill="none" stroke="#000000" d="M169.0942,-137.3891C190.3832,-149.34 217.1706,-164.4278 241,-178 242.3014,-178.7412 243.6229,-179.4951 244.9568,-180.2571"/>
<polygon fill="#000000" stroke="#000000" points="243.4393,-183.4214 253.8563,-185.3545 246.9185,-177.3472 243.4393,-183.4214"/>
</g>
<!-- modm_architecture_clock&#45;&gt;modm_processing_timer -->
<g id="edge5" class="edge">
<title>modm_architecture_clock&#45;&gt;modm_processing_timer</title>
<path fill="none" stroke="#000000" d="M74.6285,-177.8159C83.9472,-168.9291 93.9692,-158.9069 102.9238,-149.5402"/>
<polygon fill="#000000" stroke="#000000" points="105.5283,-151.8797 109.8424,-142.2029 100.4354,-147.0773 105.5283,-151.8797"/>
</g>
<!-- modm_driver_bme280 -->
<g id="node6" class="node">
<title>modm_driver_bme280</title>
<g id="a_node6"><a xlink:href="../modm-driver-bme280" xlink:title="modm:&#10;driver:&#10;bme280">
<polygon fill="#d3d3d3" stroke="#000000" points="160.5,-53 99.5,-53 99.5,0 160.5,0 160.5,-53"/>
<text text-anchor="middle" x="130" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="130" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">driver:</text>
<text text-anchor="middle" x="130" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">bme280</text>
</a>
</g>
</g>
<!-- modm_driver_bme280&#45;&gt;modm_processing_timer -->
<g id="edge6" class="edge">
<title>modm_driver_bme280&#45;&gt;modm_processing_timer</title>
<path fill="none" stroke="#000000" d="M130,-53.2029C130,-61.2113 130,-70.1403 130,-78.6802"/>
<polygon fill="#000000" stroke="#000000" points="126.5001,-78.8159 130,-88.8159 133.5001,-78.8159 126.5001,-78.8159"/>
</g>
</g>
</svg>

