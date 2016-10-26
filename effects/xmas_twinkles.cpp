#include "xmas_twinkles.h"

void xmasRedGreenTwinkles(u16 runSec)
{
    const u8 flashDelayMsec = 20;
    const u8 flashPauseMsec = 150;
    const u8 numTwinklesPerSwap = 3;
    bool redOrGreenFirst = false;
    u32 startMsec = millis();
    u8 numTwinklesToNextSwap = 0;
    while (millis() < startMsec + runSec * 1000UL) {
        if (!numTwinklesToNextSwap--) {
            numTwinklesToNextSwap = numTwinklesPerSwap;
            redOrGreenFirst = !redOrGreenFirst;
        }
        u8 twinkleLedIdx = random(0, pixels.numPixels() - 1);
        pixels.setPixelColor(twinkleLedIdx, 0xffffff);
        pixels.show();
        delay(flashDelayMsec);
        for (u8 i = 0; i < pixels.numPixels(); ++i) {
            u32 c = (i + redOrGreenFirst) & 1 ? 0xff0000 : 0x00ff00;
            pixels.setPixelColor(i, c);
        }
        pixels.show();
        delay(flashPauseMsec);
    }
}
