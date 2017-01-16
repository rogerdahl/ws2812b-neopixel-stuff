#include <stdlib.h>
#include <iostream>

#include "shimmer.h"
#include "util.h"

#include <cmath>

PixelState::PixelState()
  : pos(10.0f), brightness(0.0f), speed(0.0f), hsv(0.0f)
{
}

PixelState::PixelState(float brightness, float speed, float hsv)
: pos(0.0f), brightness(brightness), speed(speed), hsv(hsv)
{
}

Ws281xEffectShimmer::Ws281xEffectShimmer(
  Ws281xString& pixels,
  float minBrightness, float maxBrightness,
  float minSpeed, float maxSpeed,
  float minHsv, float maxHsv)
:
  pixels_(pixels),
  minBrightness_(minBrightness), maxBrightness_(maxBrightness),
  minSpeed_(minSpeed), maxSpeed_(maxSpeed),
  minHsv_(minHsv), maxHsv_(maxHsv)
{
  pixelStateVec_ = PixelStateVec(pixels.len());
}

void Ws281xEffectShimmer::refresh()
{
  for (u16 i = 0; i < pixels_.len(); ++i) {
    auto& state = pixelStateVec_[i];

    state.pos += state.speed;

    if (state.pos > 2.0f) {
      state.pos = 0.0f;
      state.brightness = random_f(minBrightness_, maxBrightness_);
      state.speed = random_f(minSpeed_, maxSpeed_);
      state.hsv = random_f(minHsv_, maxHsv_);
    }

    float brightness;
    if (state.pos > 1.0f) {
      brightness = 0.0f;
    }
    else {
      brightness = sinf(state.pos * PI_F);
    }

//    std::cout << brightness << std::endl;
    auto color = fromHsv(state.hsv, 1.0f, brightness);
    pixels_.set(i, color);
  }
}
