#### WS1812B Emulator and Effect Player

This is a cross-platform player and development tool for effects for [WS2812B / Adafruit NeoPixel RGB LEds](https://www.adafruit.com/datasheets/WS2812B.pdf).
 
Instead of writing to actual WS2812B LEDs, this program renders the LEDs on screen using OpenGL. This allows effects to be created, compiled, linked and debugged directly on a PC. This speeds up the development cycle and allows for easy debugging of WS2812B blinkenlight effects. The finished effects can then be played on any of the other platform specific players included with this project.

Strip, Grid and Ring layouts are supported for the on-screen rendering of the LEDs. For instance a layout resembling the [24 LED Adafruit NeoPixel Ring](https://www.adafruit.com/products/1586), can be set up by selecting the Ring layout with 24 pixels.

This was used for developing some NeoPixel patterns for a DigiSpark ATtiny85. The DigiSpark is a convenient platform for running battery powered blinky lights but is not so convenient for development as it must be unplugged and plugged back into the USB port each time the code is updated (which can easily be hundreds of times when tweaking parameters for animated effects, etc). 

#### Setup and Use

The app should compile and run on any platform where GLFW is available, such as Linux, Mac and Windows.     

#### Building on Linux 

* Tested on Linux Mint 18 64-bit.

* Should also work on Debian, Ubuntu and other Debian based distributions. 

##### Packaged dependencies

    $ sudo apt-get install build-essential freeglut3-dev

##### Emulator source

Get the code directly from this repository.

    $ git clone <copy and paste the "Clone with HTTPS " URL from the top of this page>
    $ cd WS2812B-NeoPixel-Emulator

##### OpenGL

You also need OpenGL drivers. These are specific to your graphics card and you probably already have them. If they appear to be missing, it's worth a try to set up the Mesa drivers:

    # Install these only if you need them
    $ sudo apt-get install mesa-common-dev

Now you should be able to compile and run:

    $ mkdir -p cmake-build-release
    $ cd cmake-build-release
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    $ make
    $ ./emulator
    
* Watch the blinkies on screen. Then exit by closing the window or hitting Ctrl-C in the shell.

* Now start making your own effects :)

#### Notes

* The `setBrightness()` call is ignored by the emulator and the `getBrightness()` call always returns the maximum brightness value of 255. As described on the [NeoPixel library home page](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library), `setBrightness()` is "lossy" and intended for use only in `setup()`, not in animations. Since physical LEDs are brighter than a computer monitor even on low brightness settings, the LEDs are most accurately represented by rendering them with maximum brightness. By having the emulator ignore `setBrightness()`, the call can be used in the sketch to set the brightness of the physical LEDs without affecting the accuracy of the representation in the emulator and without the developer having to adjust the value when switching between the emulator and physical LEDs.

* `delay()` should always be called in the animation loop, typically after each call to `pixels.show()`. A delay of 20ms corresponds to a maximum refresh rate of 50Hz (1 / 0.02) and is typically a good value to start with. Refreshing the LEDs more often (delay value lower than 20ms) will probably not cause animations to look any smoother. To speed up the animation it's probably better to increase the step size than the refresh rate.     

* The emulator does not take into account that code runs more slowly on the device than on the PC. On the PC, virtually all the time will be spent in `delay()` calls but on the device, a non-significant portion of time is typically spent executing the program. The difference causes the sketch to run faster on the PC. The possible difference becomes larger the shorter the `delay()` is.

#### Troubleshooting
        
If the sketch works in the emulator but not on the device, some possible reasons are:
 
* The NEOPIXEL_PIN value is ignored by the emulator but determines which pin on the MCU is used for communicating with the physical LEDs. It must be set to the pin to which the LEDs are connected.
  
* The supported MCUs are memory limited, some severely so. For instance, the Atmel ATtiny85 has only 512 bytes of RAM. When controlling 60 LEDs with the ATtiny85 (with the LED RGB values alone taking up 180 bytes (3 * 60)), the included sketches use almost all available memory. Adding more functionality without removing something will typically cause them to run out of memory and crash on the device. There's no practical limit to how much memory the sketch can use when running on the PC.

* If the colors are different between emulator and device, try changing the color channel ordering in the NeoPixel driver object instantiation. Possibilities are NEO_RGB, NEO_GRB and NEO_BRG. 

* When compiling for the PC, the size of `int` is 32-bit (on both 32- and 64-bit CPUs and OSes). When compiling for the Atmel AVR processor architecture, [it's 16-bit](https://gcc.gnu.org/wiki/avr-gcc). This causes `int` types and expressions where types are implicitly promoted to `int` to be much more likely to overflow on the MCU. For instance, `uint8_t a = 100; uint32_t b = a * 1000;` will give the correct result of `b = 100,000` on the PC but a truncated result of `b = 31,072` on the MCU, even though a 32-bit `int` is used for holding the result on both platforms. This happens because, before taking part in an arithmetic operation, both operands must be of the same size and the result is calculated using that size. If the operands are not of the same size, they're promoted to the size of the largest operand or to `int` if the largest operand is smaller than `int`. In this example, the literal `1000` is implicitly `int`, so `a` is promoted to `int`. The multiplication is then performed using the resolution of `int`. On the PC, the result of the operation fits in a 32-bit `int` and is then directly stored in `b`. On the MCU, the result does not fit in a 16-bit `int`, causing an overflow. The truncated result is then expanded to 32-bits and stored in `b`. The fix for this is to make sure that at least one of the operands is the same size or larger than the size required for holding the result. In the example, this can be done by specifying `1000` as a 32-bit `int` by appending `UL` to the number or by casting one of the operands to 32-bit.

    The compiler breaks expressions involving multiple operands into subexpressions and the rules above apply to each subexpression. For instance, `uint32_t a = 50; uint8_t b = 100; uint32_t c = a + b * 1000;` will, on a machine with 16-bit `int`, overflow in the `b * 1000` subexpression even though `a` is a 32-bit `int`. First, `b` is promoted to `int` and the multiplication is performed with `int` resolution, yielding a truncated `int` result. Then the truncated result is promoted to 32-bit to match `a`, then added to `a`, yielding a 32-bit result which is stored directly in `c`.
