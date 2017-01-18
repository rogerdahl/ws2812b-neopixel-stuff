#include "emergency.h"
#include "util.h"

// Misc effects that are based on additively mixing colored segments. Segment
// and LED positions are first calculated
// with a virtual string of LEDs that has 256x more LEDs than the physical
// string. These virtual "superpositions" are
// then averaged to determine the colors for the physical LEDs.

const u16 numOnFrames = 4;
const u16 numOffFrames = 2;
const u16 numCyclesPerColor = 6;

Ws281xEffectEmergency::Ws281xEffectEmergency(
    Ws281xString& pixels, u8 numSections, u8 param)
  : pixels_(pixels), numSections_(numSections), param_(param), numCurFrame(0)
{
  numPixelsPerSection_ = pixels_.len() / numSections;
}

void Ws281xEffectEmergency::refresh()
{
  Color black = Color((u8)0, (u8)0, (u8)0);
  Color red = Color((u8)0xff, (u8)0, (u8)0);
  Color blue = Color((u8)0, (u8)0, (u8)0xff);

  bool isRed = numCurFrame < numCyclesPerColor * (numOnFrames + numOffFrames);
  bool isBlack = numCurFrame % (numOnFrames + numOffFrames) >= numOnFrames;

  for (u16 i = 0; i < pixels_.len(); ++i) {
    if (isBlack) {
      pixels_.set(i, black);
    }
    else if (i % (numPixelsPerSection_ * 2) < numPixelsPerSection_) {
      pixels_.set(i, isRed ? red : blue);
    }
    else {
      pixels_.set(i, isRed ? blue : red);
    }
  }

  if (++numCurFrame == 2 * numCyclesPerColor * (numOnFrames + numOffFrames)) {
    numCurFrame = 0;
  }
}
