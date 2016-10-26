### WS2812B / Adafruit NeoPixel RGB LED players and effects

There are many platforms and libraries for controlling WS2812B / Adafruit NeoPixel RGB LEDs. However, they're all slightly different, so code written for one platform and library won't work on any other without modifications. This project defines an abstraction layer that allows separating code that controls WS1212B into "players" and "effects", and implements various components of each type.

**Players:** Players are platform and library specific. Implementing a player for a given platform and library allows the platform to run all effects, without modifying any code in the effects.  

**Effects:** Effects are platform agnostic. An effect implements an animation by describing how the color of each LED should change over time. After implementing an effect, it can be used on any platform for which there is a player.
 
 This project currently contains players for the following platforms:
 
 * Atmel AVR ATtiny85 (Arduino) with the NeoPixel library by Adafruit
   * Ideal for DIY wearables
   
 * STMicroelectronics STM32 with the *** library by ***
   * Ideal for wearables and stationary displays, depending on which STM32 is used
   
 * Raspberry Pi with the ws2811 library by Jeremy Garff
   * Ideal for high-powered stationary displays running many LEDs and/or running animations as part of a larger system, such as dynamically updated stock tickers.
   
 * Linux and Windows PC with LEDs emulated by drawing them on screen using OpenGL
   * Ideal for effects development due to fast compile times 
 
 #### Implementation
 
 Players and effects are implemented in C++ and use regular polymorphism. The abstraction layer is defined in two simple abstract classes. Players and effects each extend one of the classes and override a couple of pure virtual functions, allowing players and effects to communicate via virtual calls.
 
 Effects provide a single function, `calcColors()` and players provide three functions, `get()`, `set()` and `len()`. The player calls `calcColors()` 50 times per second to update the LEDs. The effect then calls `len()` to find the total number of LEDs, and `get()` and `set()` to read and modify the colors of the LEDs. When receiving the calls, the player in turn interacts with the underlying LED library (using the native interface provided by the library) to read or update the LEDs.   
 
 This approach was chosen as opposed to having `calcColors()` return an array of LED colors so that players could be implemented on uCs like the ATtiny85, which have only a few hundred bytes of free RAM and cannot fit a copy of all the LED colors in memory.    
 
 Note that, though this approach makes players and effects interchangeable, it's not a plugin system. That is, the player and effects are compiled together to a single binary.
 