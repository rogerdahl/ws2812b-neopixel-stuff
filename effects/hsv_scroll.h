#pragma once

#include "../../../interface/shim.h"

class Ws281xEffectHsvScroll : public Ws281xEffect
{
public:
    Ws281xEffectHsvScroll(Ws281xString&, float scrollSpeed, float scrollStep);
    void calcColors();
private:
    Ws281xString& pixels_;
    float scrollPos_;
    float scrollSpeed_;
    float scrollStep_;
};
