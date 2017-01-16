#pragma once

#include "shim.h"

class Ws281xEffectXmasTwinkles : public Ws281xEffect
{
public:
  Ws281xEffectXmasTwinkles(Ws281xString&);
  void refresh();
private:
  Ws281xString& pixels_;
  bool isRedFirst_;
  u8 nSwapFrames_;
  u8 nFlashFrames_;
  u8 nPauseFrames_;
  s16 twinkleLedIdx;
};

