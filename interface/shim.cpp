#include "shim.h"
#include "stdio.h"
//#include "print_stacktrace.h"

Color::Color()
{
}

Color::Color(u8 r_in, u8 g_in, u8 b_in)
  : r(r_in)
  , g(g_in)
  , b(b_in)
{
}

Color::Color(int r_in, int g_in, int b_in)
  : r(r_in)
  , g(g_in)
  , b(b_in)
{
}

Color::Color(float r_in, float g_in, float b_in)
  : r(r_in * 255)
  , g(g_in * 255)
  , b(b_in * 255)
{
  //    print_stacktrace();
}

Color::Color(u32 rgb)
{
  b = rgb;
  g = rgb >> 8;
  r = rgb >> 16;
}

Color::Color(const Color& c)
{
  copy(c);
}

bool Color::operator==(const Color& c)
{
  return c.r == r && c.g == g && c.b == b;
}

Color& Color::operator=(const Color& c)
{
  copy(c);
  return *this;
}

u32 Color::rgb() const
{
  return r << 16 | g << 8 | b;
}

void Color::additiveMix(const Color& c)
{
  r = colorChannelClampedAdd(r, c.r);
  g = colorChannelClampedAdd(g, c.g);
  b = colorChannelClampedAdd(b, c.b);
}

void Color::copy(const Color& c)
{
  r = c.r;
  g = c.g;
  b = c.b;
}

u8 Color::colorChannelClampedAdd(u8 a, u8 b)
{
  u16 c = a + b;
  if (c > 255) {
    c = 255;
  }
  return c;
}

// For some effects, such as rainbows, one can get more brightness by clamping
// the combined light output from the red,
// green and blue channels instead of reducing the overall brightness. If the
// combined value for the red, green and blue
// channels would be above the clamp value, the channels are adjusted so that
// their combined output is equal to the
// clamp value without altering the relative brightnesses of colors, which
// preserves the combined color.
//
// E.g., if one channel is fully on (value is 255) and the other two are fully
// off, clamping the light output to 255
// will allow the single channel to display at full brightness. Any combination
// of the three channels will get clamped
// to a combined value of 255, which is 1/3 of the possible combined value from
// the three channels. The difference is
// that if regular brightness is set to 1/3, the maximum possible value from any
// single channel is 256/3 = 85.
//
// This has the nice effect of also clamping the total power consumption of a
// LED (or string of LEDs), while (if the
// clamp value is 255), also allowing a single channel to be displayed at its
// maximum possible brightness. This function
// can be used in combination with the regular brightness setting.
Color Color::clampBrightness(const Color& c, u16 clamp)
{
  u16 combined = c.r + c.g + c.b;
  if (combined <= clamp) {
    return Color(c);
  }
  float clamp_factor = (float)clamp / combined;
  return Color(
    (u8)(c.r * clamp_factor), (u8)(c.g * clamp_factor),
    (u8)(c.b * clamp_factor));
}

Color Color::dimLightOutput(const Color& c, float light_factor)
{
  return Color(
    (u8)(c.r * light_factor), (u8)(c.g * light_factor),
    (u8)(c.b * light_factor));
}
