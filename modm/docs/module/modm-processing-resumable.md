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
<!-- Generated by graphviz version 2.40.1 (20161225.0304)
 -->
<!-- Title: modm:processing:resumable Pages: 1 -->
<svg width="528pt" height="239pt"
 viewBox="0.00 0.00 528.00 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:processing:resumable</title>
<polygon fill="#ffffff" stroke="transparent" points="-4,4 -4,-235 524,-235 524,4 -4,4"/>
<!-- modm_processing_resumable -->
<g id="node1" class="node">
<title>modm_processing_resumable</title>
<polygon fill="#d3d3d3" stroke="#000000" stroke-width="2" points="297,-142 219,-142 219,-89 297,-89 297,-142"/>
<text text-anchor="middle" x="258" y="-126.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="258" y="-111.8" font-family="Times,serif" font-size="14.00" fill="#000000">processing:</text>
<text text-anchor="middle" x="258" y="-96.8" font-family="Times,serif" font-size="14.00" fill="#000000">resumable</text>
</g>
<!-- modm_architecture_assert -->
<g id="node2" class="node">
<title>modm_architecture_assert</title>
<g id="a_node2"><a xlink:href="../modm-architecture-assert" xlink:title="modm:&#10;architecture:&#10;assert">
<polygon fill="#d3d3d3" stroke="#000000" points="252,-231 168,-231 168,-178 252,-178 252,-231"/>
<text text-anchor="middle" x="210" y="-215.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="210" y="-200.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="210" y="-185.8" font-family="Times,serif" font-size="14.00" fill="#000000">assert</text>
</a>
</g>
</g>
<!-- modm_processing_resumable&#45;&gt;modm_architecture_assert -->
<g id="edge1" class="edge">
<title>modm_processing_resumable&#45;&gt;modm_architecture_assert</title>
<path fill="none" stroke="#000000" d="M243.5984,-142.2029C239.0895,-150.5633 234.0393,-159.927 229.2518,-168.8039"/>
<polygon fill="#000000" stroke="#000000" points="226.0578,-167.3529 224.3914,-177.8159 232.2189,-170.6758 226.0578,-167.3529"/>
</g>
<!-- modm_processing -->
<g id="node3" class="node">
<title>modm_processing</title>
<g id="a_node3"><a xlink:href="../modm-processing" xlink:title="modm:&#10;processing">
<polygon fill="#d3d3d3" stroke="#000000" points="344,-223.5 270,-223.5 270,-185.5 344,-185.5 344,-223.5"/>
<text text-anchor="middle" x="307" y="-208.3" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="307" y="-193.3" font-family="Times,serif" font-size="14.00" fill="#000000">processing</text>
</a>
</g>
</g>
<!-- modm_processing_resumable&#45;&gt;modm_processing -->
<g id="edge2" class="edge">
<title>modm_processing_resumable&#45;&gt;modm_processing</title>
<path fill="none" stroke="#000000" d="M272.7016,-142.2029C278.692,-153.0834 285.618,-165.6634 291.6712,-176.6579"/>
<polygon fill="#000000" stroke="#000000" points="288.6218,-178.3762 296.5108,-185.4482 294.7538,-175.0001 288.6218,-178.3762"/>
</g>
<!-- modm_architecture_block_device -->
<g id="node4" class="node">
<title>modm_architecture_block_device</title>
<g id="a_node4"><a xlink:href="../modm-architecture-block-device" xlink:title="modm:&#10;architecture:&#10;block.device">
<polygon fill="#d3d3d3" stroke="#000000" points="86,-53 0,-53 0,0 86,0 86,-53"/>
<text text-anchor="middle" x="43" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="43" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="43" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">block.device</text>
</a>
</g>
</g>
<!-- modm_architecture_block_device&#45;&gt;modm_processing_resumable -->
<g id="edge3" class="edge">
<title>modm_architecture_block_device&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M86.2468,-48.9686C89.1931,-50.3664 92.1303,-51.7228 95,-53 132.64,-69.7516 176.2666,-86.3358 208.9086,-98.2069"/>
<polygon fill="#000000" stroke="#000000" points="208.0732,-101.6265 218.6675,-101.7347 210.453,-95.0434 208.0732,-101.6265"/>
</g>
<!-- modm_architecture_gpio_expander -->
<g id="node5" class="node">
<title>modm_architecture_gpio_expander</title>
<g id="a_node5"><a xlink:href="../modm-architecture-gpio-expander" xlink:title="modm:&#10;architecture:&#10;gpio.expander">
<polygon fill="#d3d3d3" stroke="#000000" points="198,-53 104,-53 104,0 198,0 198,-53"/>
<text text-anchor="middle" x="151" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="151" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="151" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">gpio.expander</text>
</a>
</g>
</g>
<!-- modm_architecture_gpio_expander&#45;&gt;modm_processing_resumable -->
<g id="edge4" class="edge">
<title>modm_architecture_gpio_expander&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M183.1035,-53.2029C194.107,-62.3553 206.5561,-72.7102 218.1004,-82.3125"/>
<polygon fill="#000000" stroke="#000000" points="215.9928,-85.1119 225.9191,-88.8159 220.4692,-79.7303 215.9928,-85.1119"/>
</g>
<!-- modm_architecture_i2c_device -->
<g id="node6" class="node">
<title>modm_architecture_i2c_device</title>
<g id="a_node6"><a xlink:href="../modm-architecture-i2c-device" xlink:title="modm:&#10;architecture:&#10;i2c.device">
<polygon fill="#d3d3d3" stroke="#000000" points="300,-53 216,-53 216,0 300,0 300,-53"/>
<text text-anchor="middle" x="258" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="258" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="258" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.device</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_device&#45;&gt;modm_processing_resumable -->
<g id="edge5" class="edge">
<title>modm_architecture_i2c_device&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M258,-53.2029C258,-61.2113 258,-70.1403 258,-78.6802"/>
<polygon fill="#000000" stroke="#000000" points="254.5001,-78.8159 258,-88.8159 261.5001,-78.8159 254.5001,-78.8159"/>
</g>
<!-- modm_architecture_i2c_multiplexer -->
<g id="node7" class="node">
<title>modm_architecture_i2c_multiplexer</title>
<g id="a_node7"><a xlink:href="../modm-architecture-i2c-multiplexer" xlink:title="modm:&#10;architecture:&#10;i2c.multiplexer">
<polygon fill="#d3d3d3" stroke="#000000" points="418,-53 318,-53 318,0 418,0 418,-53"/>
<text text-anchor="middle" x="368" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="368" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="368" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">i2c.multiplexer</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_multiplexer&#45;&gt;modm_processing_resumable -->
<g id="edge6" class="edge">
<title>modm_architecture_i2c_multiplexer&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M334.9964,-53.2029C323.6844,-62.3553 310.8862,-72.7102 299.0183,-82.3125"/>
<polygon fill="#000000" stroke="#000000" points="296.553,-79.805 290.9803,-88.8159 300.9559,-85.2469 296.553,-79.805"/>
</g>
<!-- modm_architecture_spi -->
<g id="node8" class="node">
<title>modm_architecture_spi</title>
<g id="a_node8"><a xlink:href="../modm-architecture-spi" xlink:title="modm:&#10;architecture:&#10;spi">
<polygon fill="#d3d3d3" stroke="#000000" points="520,-53 436,-53 436,0 520,0 520,-53"/>
<text text-anchor="middle" x="478" y="-37.8" font-family="Times,serif" font-size="14.00" fill="#000000">modm:</text>
<text text-anchor="middle" x="478" y="-22.8" font-family="Times,serif" font-size="14.00" fill="#000000">architecture:</text>
<text text-anchor="middle" x="478" y="-7.8" font-family="Times,serif" font-size="14.00" fill="#000000">spi</text>
</a>
</g>
</g>
<!-- modm_architecture_spi&#45;&gt;modm_processing_resumable -->
<g id="edge7" class="edge">
<title>modm_architecture_spi&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="#000000" d="M435.8977,-48.8928C432.9064,-50.3242 429.9197,-51.7076 427,-53 387.2961,-70.5746 340.9661,-87.4645 306.8666,-99.246"/>
<polygon fill="#000000" stroke="#000000" points="305.4673,-96.0257 297.1441,-102.5813 307.7388,-102.6469 305.4673,-96.0257"/>
</g>
</g>
</svg>

