#include "xmas_twinkles.h"
#include "util.h"

// Alternate red and green. White flashes.

const u8 N_PAUSE_FRAMES_MIN = 10;
const u8 N_PAUSE_FRAMES_MAX = 60;

const u8 N_FLASH_FRAMES = 1;
const u8 N_FRAMES_PER_SWAP = 60;

Ws281xEffectXmasTwinkles::Ws281xEffectXmasTwinkles(Ws281xString& pixels)
  : pixels_(pixels),
    isRedFirst_(false),
    nSwapFrames_(0),
    nFlashFrames_(0),
    nPauseFrames_(0)
{
}

void Ws281xEffectXmasTwinkles::refresh()
{
  if (!nSwapFrames_--) {
    nSwapFrames_ = N_FRAMES_PER_SWAP;
    isRedFirst_ = !isRedFirst_;
  }

  if (!nPauseFrames_--) {
    nPauseFrames_ = random(N_PAUSE_FRAMES_MIN, N_PAUSE_FRAMES_MAX);
    twinkleLedIdx = random(0, pixels_.len() - 1);
    nFlashFrames_ = N_FLASH_FRAMES;
  }

  if (nFlashFrames_) {
    --nFlashFrames_;
  }
  else {
    twinkleLedIdx = -1;
  }

  for (s16 i = 0; i < pixels_.len(); ++i) {
    Color c;
    if (i == twinkleLedIdx) {
      c = Color(255, 255, 255);
    }
    else if ((i + isRedFirst_) & 1) {
      c = Color(255, 0, 0);
    }
    else {
      c = Color(0, 255, 0);
    }
    pixels_.set(i, c);
  }
}
