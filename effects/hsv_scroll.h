#pragma once

#include "shim.h"

class Ws281xEffectHsvScroll : public Ws281xEffect
{
public:
    Ws281xEffectHsvScroll(Ws281xString&, float scrollSpeed, float scrollStep);
    void refresh();
private:
    Ws281xString& pixels_;
    float scrollPos_;
    float scrollSpeed_;
    float scrollStep_;
};
