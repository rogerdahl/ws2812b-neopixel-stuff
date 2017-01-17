### WS2812B Effect Player for Atmel ATtiny85

This is a player for effects that use [WS2812B / Adafruit NeoPixel RGB LEds](https://www.adafruit.com/datasheets/WS2812B.pdf). The player is based on the [NeoPixel library by Adafruit](https://github.com/adafruit/Adafruit_NeoPixel).

See the [main page](https://github.com/rogerdahl/ws2812b-neopixel-stuff) for more information on players, effects, and how to build platforms that can run these.

#### Building and flashing

* Download

    * https://www.arduino.cc/en/Main/Software
    * Linux 64 bits

    * `File` > `Preferences` > `Additional Board Manager URLs:` `http://digistump.com/package_digistump_index.json`

    * Tools > Board > Boards Manager > Type > Contributed
    
        * If Contributed is not there, wait for a bit then try again
  
    * Digistump AVR Boards > Install

* $ sudo pico /etc/udev/rules.d/49-micronucleus.rules

    * SUBSYSTEMS=="usb", ATTRS{idVendor}=="16d0", ATTRS{idProduct}=="0753", MODE:="0666"

    * KERNEL=="ttyACM*", ATTRS{idVendor}=="16d0", ATTRS{idProduct}=="0753", MODE:="0666", ENV{ID_MM_DEVICE_IGNORE}="1"

    * $ sudo udevadm control --reload-rules
    
    * From the Tools menu select Board > Digispark (Default - 16.5Mhz)
    
        * (The Tools > Programmer selection does not matter)
    
    * Write some code, open your code, or open a Digispark example.
    
    * You do not need to plug in your Digispark before invoking upload
    
    * Hit the upload button. The bottom status box will now ask you to plug in your Digispark - at this point you need to plug it in - or unplug and replug it.
    
    * You'll see the upload progress and then it will immediately run your code on the Digispark.
    
    * If you unplug the Digispark and plug it back in or attach it to another power source there will be a delay of 5 seconds before the code you programmed will run. This 5 second delay is the Digispark Pro checking to see if you are trying to program it.
        
        * File > Examples > DigiSpark CDC > CDC_LED
