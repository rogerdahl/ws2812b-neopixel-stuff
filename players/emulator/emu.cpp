#include "emu.h"

Ws281xStringEmu::Ws281xStringEmu(ColorVec& leds)
  : leds(leds)
{
}

void Ws281xStringEmu::set(u16 pixelIdx, const Color& color)
{
  leds[pixelIdx] = color;
}

Color Ws281xStringEmu::get(u16 pixelIdx)
{
  return leds[pixelIdx];
}

u16 Ws281xStringEmu::len()
{
  return static_cast<u16>(leds.size());
}
