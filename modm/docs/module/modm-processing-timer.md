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
enum class PeriodicTimerState;
enum class TimeoutState;
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
<!-- Generated by graphviz version 2.40.1 (0)
 -->
<!-- Title: modm:processing:timer Pages: 1 -->
<svg width="398pt" height="239pt"
 viewBox="0.00 0.00 397.50 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:processing:timer</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-235 393.5,-235 393.5,4 -4,4"/>
<!-- modm_processing_timer -->
<g id="node1" class="node">
<title>modm_processing_timer</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="210.5,-142 111.5,-142 111.5,-89 210.5,-89 210.5,-142"/>
<text text-anchor="middle" x="161" y="-126.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="161" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="161" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">timer</text>
</g>
<!-- modm_architecture_clock -->
<g id="node2" class="node">
<title>modm_architecture_clock</title>
<g id="a_node2"><a xlink:href="../modm-architecture-clock" xlink:title="modm:&#10;architecture:&#10;clock">
<polygon fill="#d3d3d3" stroke="#000000" points="110,-231 0,-231 0,-178 110,-178 110,-231"/>
<text text-anchor="middle" x="55" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="55" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="55" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">clock</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_architecture_clock -->
<g id="edge1" class="edge">
<title>modm_processing_timer&#45;&gt;modm_architecture_clock</title>
<path fill="none" stroke="#000000" d="M122.9512,-142.2029C111.6136,-151.2673 99.3074,-161.5112 88.2948,-171.0353"/>
<polygon fill="#000000" stroke="#000000" points="85.763,-168.5997 80.5369,-177.8159 90.3696,-173.8703 85.763,-168.5997"/>
</g>
<!-- modm_io -->
<g id="node3" class="node">
<title>modm_io</title>
<g id="a_node3"><a xlink:href="../modm-io" xlink:title="modm:&#10;io">
<polygon fill="#d3d3d3" stroke="#000000" points="193.5,-223.5 128.5,-223.5 128.5,-185.5 193.5,-185.5 193.5,-223.5"/>
<text text-anchor="middle" x="161" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="161" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">io</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_io -->
<g id="edge2" class="edge">
<title>modm_processing_timer&#45;&gt;modm_io</title>
<path fill="none" stroke="#000000" d="M161,-142.2029C161,-152.6482 161,-164.6597 161,-175.3311"/>
<polygon fill="#000000" stroke="#000000" points="157.5001,-175.4482 161,-185.4482 164.5001,-175.4483 157.5001,-175.4482"/>
</g>
<!-- modm_math_utils -->
<g id="node4" class="node">
<title>modm_math_utils</title>
<g id="a_node4"><a xlink:href="../modm-math-utils" xlink:title="modm:&#10;math:&#10;utils">
<polygon fill="#d3d3d3" stroke="#000000" points="276.5,-231 211.5,-231 211.5,-178 276.5,-178 276.5,-231"/>
<text text-anchor="middle" x="244" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="244" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">math:</text>
<text text-anchor="middle" x="244" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">utils</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_math_utils -->
<g id="edge3" class="edge">
<title>modm_processing_timer&#45;&gt;modm_math_utils</title>
<path fill="none" stroke="#000000" d="M185.9027,-142.2029C194.1919,-151.0913 203.5387,-161.1138 212.2732,-170.4797"/>
<polygon fill="#000000" stroke="#000000" points="209.7349,-172.8897 219.1148,-177.8159 214.8542,-168.1155 209.7349,-172.8897"/>
</g>
<!-- modm_processing -->
<g id="node5" class="node">
<title>modm_processing</title>
<g id="a_node5"><a xlink:href="../modm-processing" xlink:title="modm:&#10;processing">
<polygon fill="#d3d3d3" stroke="#000000" points="389.5,-223.5 294.5,-223.5 294.5,-185.5 389.5,-185.5 389.5,-223.5"/>
<text text-anchor="middle" x="342" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="342" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">processing</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_processing -->
<g id="edge4" class="edge">
<title>modm_processing_timer&#45;&gt;modm_processing</title>
<path fill="none" stroke="#000000" d="M210.9382,-140.0552C236.846,-152.7945 268.3457,-168.2832 293.9704,-180.8833"/>
<polygon fill="#000000" stroke="#000000" points="292.6058,-184.1125 303.1241,-185.3842 295.6947,-177.8308 292.6058,-184.1125"/>
</g>
<!-- modm_architecture_clock&#45;&gt;modm_processing_timer -->
<g id="edge5" class="edge">
<title>modm_architecture_clock&#45;&gt;modm_processing_timer</title>
<path fill="none" stroke="#000000" d="M93.0252,-177.8159C104.3615,-168.7532 116.6679,-158.5096 127.6823,-148.9845"/>
<polygon fill="#000000" stroke="#000000" points="130.2154,-151.419 135.4418,-142.2029 125.6089,-146.1482 130.2154,-151.419"/>
</g>
<!-- modm_driver_bme280 -->
<g id="node6" class="node">
<title>modm_driver_bme280</title>
<g id="a_node6"><a xlink:href="../modm-driver-bme280" xlink:title="modm:&#10;driver:&#10;bme280">
<polygon fill="#d3d3d3" stroke="#000000" points="198,-53 124,-53 124,0 198,0 198,-53"/>
<text text-anchor="middle" x="161" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="161" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">driver:</text>
<text text-anchor="middle" x="161" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">bme280</text>
</a>
</g>
</g>
<!-- modm_driver_bme280&#45;&gt;modm_processing_timer -->
<g id="edge6" class="edge">
<title>modm_driver_bme280&#45;&gt;modm_processing_timer</title>
<path fill="none" stroke="#000000" d="M161,-53.2029C161,-61.2113 161,-70.1403 161,-78.6802"/>
<polygon fill="#000000" stroke="#000000" points="157.5001,-78.8159 161,-88.8159 164.5001,-78.8159 157.5001,-78.8159"/>
</g>
</g>
</svg>

