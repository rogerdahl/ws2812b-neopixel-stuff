#pragma once

#include "ws281x_shim.h"

class Ws281xEffectSmoothRunners : public Ws281xEffect
{
public:
    Ws281xEffectSmoothRunners(Ws281xString&);
    void calcColors();
private:
    Ws281xString& pixels_;
};

