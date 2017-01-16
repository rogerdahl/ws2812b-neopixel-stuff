#pragma once

#include "shim.h"

class Segment
{
public:
  Segment();
  Segment(const Color& color_in, s16 speed_in, u8 lengthPercent_in);
  Color color;
  s16 speed;
  u8 lengthPercent;
};

const int maxSegments = 20;

class Ws281xEffectSmoothRunners : public Ws281xEffect
{
public:
  Ws281xEffectSmoothRunners(Ws281xString&, u8 effectIdx, u8 speedFactor);
  void refresh();
private:
  void drawTaperedSegment(u16 superPos, u8 numSegmentPixels, const Color&);
  Color calcTaperedSegmentPixel(u8 numSegmentPixels, u8 pos, const Color&);
  void addPixelColor(u16 pixelPos, const Color& addColor);

  Ws281xString& pixels_;
  u8 effectIdx_;
  Segment segmentArr[maxSegments];
  u8 numSegments_;

  u16 superPosBuf_[maxSegments];
  u16 numSuperPositions;
  u16 superPosOffset;

  u8 speedFactor_;
};

