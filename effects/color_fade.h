#pragma once

#include "../../../interface/shim.h"

class Ws281xEffectColorFade : public Ws281xEffect
{
public:
    Ws281xEffectColorFade(Ws281xString&, u8 fadeSpeed, u8 fadeStep);
    void calcColors();
private:
    u8 fadeChannel(u8 curChannel, u8 targetChannel);

    Ws281xString& pixels_;
    u8 fadeSpeed_;
    u8 fadeStep_;
    u8 fadeCur_;
    Color curColor_;
    Color targetColor_;
};
