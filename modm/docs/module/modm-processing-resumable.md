!!! warning "These module docs are in beta and may be incomplete."

# modm:processing:resumable: Resumable Functions

An implementation of lightweight resumable functions which allow for nested calling.

This base class and its macros allows you to implement and use several
resumable functions in one class.
This allows you to modularize your code by placing it into its own resumable functions
instead of the placing everything into one big method.
It also allows you to call and run resumable functions within your resumables,
so you can reuse their functionality.

Note that you should call resumable functions within a protothreads, with the
`PT_CALL(group.resumable())` macro, which will return the result of the
resumable function. To call a resumable function inside another resumable
function, use the `RF_CALL(group.resumable())`.

You may use the `RF_CALL_BLOCKING(group.resumable())` macro to execute
a resumable function outside of a protothread, however, this which will
force the CPU to busy-wait until the resumable function ended.

!!! warning "Resumable functions are not thread-safe!"
    Use an external mutex to arbitrate access if two Protothreads access the
    same resumable function.

You can either run your group of resumable functions independently from each
other, or only run one function at a time, but it may nest calls to its own
group's functions.
So you need to inherit your group from `modm::Resumable<#Functions>`, and from
`modm::NestedResumable<#Functions>` respectively.


## Independent operation

You must begin each resumable function using `RF_BEGIN(index)` where `index` is
the unique index of your resumable function starting at zero.
You may exit and return a value by using `RF_RETURN(value)` or
return the result of another resumable function using `RF_RETURN_CALL(resumable())`.
This return value is wrapped in a `modm::ResumableResult<Type>` struct
and transparently returned by the `RF_CALL` macro so it can be used
to influence your program flow.
If the resumable function reaches `RF_END()` it will exit automatically,
with the result of `0` cast to the return type.
Should you wish to return a value at the end, you may use
`RF_END_RETURN(value)`.
You may also return the result of another resumable function using
`RF_END_RETURN_CALL(resumable())`.

Be aware that this class keeps a separate state for each of your resumable functions.
This allows each resumable function to be run at the same time.
This might require the use of an internal semaphore or mutex if such
dependencies exist in your use case.
Take a look at the `NestedResumable` class for mutually exclusive resumable functions,
which also require a little less memory.


## Nested operation

You are responsible to choosing the right nesting depth!
This class will guard itself against calling another resumable function at too
deep a nesting level and fail the `resumable.begin.nesting` assertion!
It is then up to you to recognize this in your program design
and increase the nesting depth or rethink your code.

The resumable functions of this class are mutually exclusive, so only one
resumable function of the same object can run at the same time. Even if you
call another resumable function, it will simply return `modm::rf::WrongState`.
Using the `RF_CALL(resumable())` macro, you can wait for these
resumable functions to become available and then run them, so you usually do
not need to worry about those cases.

You must begin each resumable function using `RF_BEGIN()`.
You may exit and return a value by using `RF_RETURN(value)` or
return the result of another resumable function using `RF_RETURN_CALL(resumable())`.
This return value is wrapped in a `modm::ResumableResult<Type>` struct
and transparently returned by the `RF_CALL` macro so it can be used
to influence your program flow.
If the resumable function reaches `RF_END()` it will exit automatically,
with the result of `0` cast to the return type.
Should you wish to return a value at the end, you may use
`RF_END_RETURN(value)`.
You may also return the result of another resumable function using
`RF_END_RETURN_CALL(resumable())`.


## Example

Here is a (slightly over-engineered) example:

```cpp
#include <modm/platform/platform.hpp>
#include <modm/processing/processing.hpp>

using Led = GpioOutputB0;

class BlinkingLight : public modm::pt::Protothread, private modm::NestedResumable<2>
{
public:
    bool
    run()
    {
        PT_BEGIN();

        // set everything up
        Led::setOutput();
        Led::set();

        while (true)
        {
            Led::set();
            PT_CALL(waitForTimer());

            Led::reset();
            PT_CALL(setTimer(200));

            PT_WAIT_UNTIL(timeout.isExpired());
        }

        PT_END();
    }

    modm::ResumableResult<bool>
    waitForTimer()
    {
        RF_BEGIN();

        // nested calling is allowed
        if (RF_CALL(setTimer(100)))
        {
            RF_WAIT_UNTIL(timeout.isExpired());
            RF_RETURN(true);
        }

        RF_END_RETURN(false);
    }

    modm::ResumableResult<bool>
    setTimer(uint16_t new_timeout)
    {
        RF_BEGIN();

        timeout.restart(new_timeout);

        if(timeout.isArmed()) {
            RF_RETURN(true);
        }

        // clean up code goes here

        RF_END_RETURN(false);
    }

private:
    modm::ShortTimeout timeout;
};

BlinkingLight light;

while (1) {
    light.run();
}
```

For other examples take a look in the `examples` folder in the modm
root folder. The given example is in `modm/examples/generic/resumable`.

## Options
#### check_nesting_depth

Check nesting call depth: `True` ∈ `{ True, False }`

Nested resumable functions protect against memory corruption by checking if the
nesting level is within the allocated nesting level depth, on first entry to
the function. If the allocated nesting level is exceeded, the assertion
`resumable.begin.nesting` fails.

You may disable this behavior by disabling this check, then instead of the
assertion, the function on entry returns the `modm::rf::NestingError` state value.
`PT_CALL()` and `RF_CALL()` macros will respond to this error by stopping
function polling and just continuing program execution.

!!! info "Performance Penalty"
    This check is performed during the call to `RF_BEGIN(N)`, so exactly once
    on function entry and not during every polling call, so the performance
    penalty is relatively small.
## Content

```cpp
// Class
class modm::NestedResumable< uint8_t Levels=1 >;
class modm::Resumable< uint8_t Functions=1 >;
// Struct
struct modm::ResumableResult< typename T >;
// Define
#define MODM_RESUMABLE_MODULE_NAME
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
<!-- Generated by graphviz version 2.40.1 (0)
 -->
<!-- Title: modm:processing:resumable Pages: 1 -->
<svg width="656pt" height="239pt"
 viewBox="0.00 0.00 656.00 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:processing:resumable</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-235 652,-235 652,4 -4,4"/>
<!-- modm_processing_resumable -->
<g id="node1" class="node">
<title>modm_processing_resumable</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="370.5,-142 271.5,-142 271.5,-89 370.5,-89 370.5,-142"/>
<text text-anchor="middle" x="321" y="-126.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="321" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="321" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">resumable</text>
</g>
<!-- modm_architecture_assert -->
<g id="node2" class="node">
<title>modm_architecture_assert</title>
<g id="a_node2"><a xlink:href="../modm-architecture-assert" xlink:title="modm:&#10;architecture:&#10;assert">
<polygon fill="#d3d3d3" stroke="#000000" points="316,-231 206,-231 206,-178 316,-178 316,-231"/>
<text text-anchor="middle" x="261" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="261" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="261" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">assert</text>
</a>
</g>
</g>
<!-- modm_processing_resumable&#45;&gt;modm_architecture_assert -->
<g id="edge1" class="edge">
<title>modm_processing_resumable&#45;&gt;modm_architecture_assert</title>
<path fill="none" stroke="#000000" d="M302.998,-142.2029C297.2432,-150.7393 290.7831,-160.3218 284.6872,-169.3639"/>
<polygon fill="#000000" stroke="#000000" points="281.6771,-167.5677 278.9893,-177.8159 287.4814,-171.4807 281.6771,-167.5677"/>
</g>
<!-- modm_processing -->
<g id="node3" class="node">
<title>modm_processing</title>
<g id="a_node3"><a xlink:href="../modm-processing" xlink:title="modm:&#10;processing">
<polygon fill="#d3d3d3" stroke="#000000" points="429.5,-223.5 334.5,-223.5 334.5,-185.5 429.5,-185.5 429.5,-223.5"/>
<text text-anchor="middle" x="382" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="382" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">processing</text>
</a>
</g>
</g>
<!-- modm_processing_resumable&#45;&gt;modm_processing -->
<g id="edge2" class="edge">
<title>modm_processing_resumable&#45;&gt;modm_processing</title>
<path fill="none" stroke="#000000" d="M339.302,-142.2029C346.834,-153.1922 355.5542,-165.9151 363.1429,-176.9872"/>
<polygon fill="#000000" stroke="#000000" points="360.4015,-179.1784 368.942,-185.4482 366.1755,-175.221 360.4015,-179.1784"/>
</g>
<!-- modm_architecture_block_device -->
<g id="node4" class="node">
<title>modm_architecture_block_device</title>
<g id="a_node4"><a xlink:href="../modm-architecture-block-device" xlink:title="modm:&#10;architecture:&#10;block.device">
<polygon fill="#d3d3d3" stroke="#000000" points="110,-53 0,-53 0,0 110,0 110,-53"/>
<text text-anchor="middle" x="55" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="55" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="55" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">block.device</text>
</a>
</g>
</g>
<!-- modm_architecture_block_device&#45;&gt;modm_processing_resumable -->
<g id="edge3" class="edge">
<title>modm_architecture_block_device&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M110.0315,-49.6654C113.055,-50.8196 116.0589,-51.9387 119,-53 166.3715,-70.0945 220.9815,-86.8162 261.4676,-98.6554"/>
<polygon fill="#000000" stroke="#000000" points="260.6025,-102.0487 271.1821,-101.4818 262.5581,-95.3274 260.6025,-102.0487"/>
</g>
<!-- modm_architecture_gpio_expander -->
<g id="node5" class="node">
<title>modm_architecture_gpio_expander</title>
<g id="a_node5"><a xlink:href="../modm-architecture-gpio-expander" xlink:title="modm:&#10;architecture:&#10;gpio.expander">
<polygon fill="#d3d3d3" stroke="#000000" points="247.5,-53 128.5,-53 128.5,0 247.5,0 247.5,-53"/>
<text text-anchor="middle" x="188" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="188" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="188" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio.expander</text>
</a>
</g>
</g>
<!-- modm_architecture_gpio_expander&#45;&gt;modm_processing_resumable -->
<g id="edge4" class="edge">
<title>modm_architecture_gpio_expander&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M227.9044,-53.2029C241.9761,-62.6193 257.95,-73.3086 272.6441,-83.1415"/>
<polygon fill="#000000" stroke="#000000" points="270.8664,-86.1633 281.1238,-88.8159 274.7594,-80.3456 270.8664,-86.1633"/>
</g>
<!-- modm_architecture_i2c_device -->
<g id="node6" class="node">
<title>modm_architecture_i2c_device</title>
<g id="a_node6"><a xlink:href="../modm-architecture-i2c-device" xlink:title="modm:&#10;architecture:&#10;i2c.device">
<polygon fill="#d3d3d3" stroke="#000000" points="376,-53 266,-53 266,0 376,0 376,-53"/>
<text text-anchor="middle" x="321" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="321" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="321" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.device</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_device&#45;&gt;modm_processing_resumable -->
<g id="edge5" class="edge">
<title>modm_architecture_i2c_device&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M321,-53.2029C321,-61.2113 321,-70.1403 321,-78.6802"/>
<polygon fill="#000000" stroke="#000000" points="317.5001,-78.8159 321,-88.8159 324.5001,-78.8159 317.5001,-78.8159"/>
</g>
<!-- modm_architecture_i2c_multiplexer -->
<g id="node7" class="node">
<title>modm_architecture_i2c_multiplexer</title>
<g id="a_node7"><a xlink:href="../modm-architecture-i2c-multiplexer" xlink:title="modm:&#10;architecture:&#10;i2c.multiplexer">
<polygon fill="#d3d3d3" stroke="#000000" points="519.5,-53 394.5,-53 394.5,0 519.5,0 519.5,-53"/>
<text text-anchor="middle" x="457" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="457" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="457" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.multiplexer</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_multiplexer&#45;&gt;modm_processing_resumable -->
<g id="edge6" class="edge">
<title>modm_architecture_i2c_multiplexer&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M416.1955,-53.2029C401.8064,-62.6193 385.4722,-73.3086 370.4467,-83.1415"/>
<polygon fill="#000000" stroke="#000000" points="368.2267,-80.4114 361.7757,-88.8159 372.0598,-86.2687 368.2267,-80.4114"/>
</g>
<!-- modm_architecture_spi -->
<g id="node8" class="node">
<title>modm_architecture_spi</title>
<g id="a_node8"><a xlink:href="../modm-architecture-spi" xlink:title="modm:&#10;architecture:&#10;spi">
<polygon fill="#d3d3d3" stroke="#000000" points="648,-53 538,-53 538,0 648,0 648,-53"/>
<text text-anchor="middle" x="593" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="593" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="593" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi</text>
</a>
</g>
</g>
<!-- modm_architecture_spi&#45;&gt;modm_processing_resumable -->
<g id="edge7" class="edge">
<title>modm_architecture_spi&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M537.9789,-49.6945C534.9527,-50.8412 531.9454,-51.9507 529,-53 479.5108,-70.6307 422.2782,-87.5836 380.3907,-99.3741"/>
<polygon fill="#000000" stroke="#000000" points="379.3106,-96.0418 370.6242,-102.1079 381.1976,-102.7827 379.3106,-96.0418"/>
</g>
</g>
</svg>

