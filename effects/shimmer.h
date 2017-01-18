#pragma once

#include <vector>

#include "shim.h"

class PixelState
{
  public:
  PixelState();
  PixelState(float brightness, float speed, float hsv);
  float pos;
  float brightness;
  float speed;
  float hsv;
};

typedef std::vector<PixelState> PixelStateVec;

class Ws281xEffectShimmer : public Ws281xEffect
{
  public:
  Ws281xEffectShimmer(
      Ws281xString& pixels, float minBrightness, float maxBrightness,
      float minSpeed, float maxSpeed, float minHsv, float maxHsv);
  void refresh();

  private:
  Ws281xString& pixels_;
  float minBrightness_;
  float maxBrightness_;
  float minSpeed_;
  float maxSpeed_;
  float minHsv_;
  float maxHsv_;
  PixelStateVec pixelStateVec_;
};
