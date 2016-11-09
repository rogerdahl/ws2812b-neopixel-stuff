#pragma once

#include "shim.h"

class Ws281xEffectEmergency : public Ws281xEffect
{
public:
    Ws281xEffectEmergency(Ws281xString&, u8 numSections, u8 param);
    void refresh();
private:
    Ws281xString& pixels_;
    u8 numSections_;
    u8 param_;
    u16 numCurFrame;
    u16 numPixelsPerSection_;
};
