#include <stdlib.h>

#include "hsv_scroll.h"
#include "util.h"

// Spin around the HSV wheel.

Ws281xEffectHsvScroll::Ws281xEffectHsvScroll(Ws281xString& pixels,
    float scrollSpeed, float scrollStep)
  : pixels_(pixels), scrollSpeed_(scrollSpeed), scrollStep_(scrollStep),
    scrollPos_(0.0f)
{
}

void Ws281xEffectHsvScroll::refresh()
{
  float p = scrollPos_;
  for (u16 i = 0; i < pixels_.len(); ++i) {
    pixels_.set(i, fromHsv(p, 1.0f, 1.0f));
    p += scrollStep_;
    if (p > 360.0f) {
      p -= 360.0f;
    }
  }

  scrollPos_ += scrollSpeed_;
  if (scrollPos_ >= 360.0f) {
    scrollPos_ -= 360.0f;
  }
}
