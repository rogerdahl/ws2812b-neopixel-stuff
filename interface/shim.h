#pragma once

#include "int_types.h"

// Style: http://geosoft.no/development/cppstyle.html

// Abstraction for colors.
class Color
{
public:
    Color(u8 r, u8 g, u8 b);
//    Color(unsigned int r, unsigned int g, unsigned int b);
    Color(float r, float g, float b);
    Color(u32 rgb);
    Color(const Color& c);
    bool operator==(const Color&);
    Color& operator=(const Color &c);
    u32 rgb() const; // 0x00RRGGBB
    void additiveMix(const Color&);
    static Color clampLightOutput(const Color&, u16 clamp);
    static Color dimLightOutput(const Color& c, float light_factor);
    u8 r;
    u8 g;
    u8 b;
private:
    void copy(const Color&);
    u8 colorChannelClampedAdd(u8 a, u8 b);
};

// This class is passed to effect algorithms so that they can manipulate pixel colors in a library independent way.
class Ws281xString
{
public:
//    virtual Ws281xString(u16 numPixels) = 0;
    virtual void set(u16 pixelIdx, const Color&) = 0;
    virtual Color get(u16 pixelIdx) = 0;
    virtual u16 len() = 0;
private:
};

// Algorithms that calculate pixel colors for effects for pixel strips, rings and grids derive from this class and
// implement the virtual functions.
class Ws281xEffect
{
public:
//    virtual Ws281xEffect(Ws281xString&) = 0;
    virtual void calcColors() = 0;
private:
};
