#include <stdlib.h>

#include "color_fade.h"
#include "util.h"


Ws281xEffectColorFade::Ws281xEffectColorFade(Ws281xString& pixels, u8 fadeSpeed, u8 fadeStep)
    : pixels_(pixels), fadeSpeed_(fadeSpeed), fadeStep_(fadeStep),
    curColor_(Color((u8)0, (u8)0, (u8)0)), targetColor_(Color((u8)0, (u8)0, (u8)0)), fadeCur_(0)
{
}

void Ws281xEffectColorFade::refresh()
{
    if (!fadeCur_--) {
        fadeCur_ = fadeSpeed_;
        curColor_.r = fadeChannel(curColor_.r, targetColor_.r);
        curColor_.g = fadeChannel(curColor_.g, targetColor_.g);
        curColor_.b = fadeChannel(curColor_.b, targetColor_.b);
    }

    for (u16 i = 0; i < pixels_.len(); ++i) {
        pixels_.set(i, curColor_);
    }

    if (curColor_ == targetColor_) {
        targetColor_ = randomBrightColor();
    }

}

u8 Ws281xEffectColorFade::fadeChannel(u8 curChannel, u8 targetChannel) {
    if (abs(curChannel - targetChannel) < fadeStep_) {
        return targetChannel;
    }
    else if (curChannel < targetChannel) {
        return curChannel + fadeStep_;
    }
    else {
        return curChannel - fadeStep_;
    }
}

