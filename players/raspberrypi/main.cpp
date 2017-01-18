#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

#include <wiringPi.h>
#include <ws2811.h>

#include "../../../interface/shim.h"
#include "color_fade.h"
#include "hsv_scroll.h"
#include "shimmer.h"
#include "smoothrunners.h"
#include "xmas_twinkles.h"

#include "util.h"

using namespace std;

const int TARGET_FREQ = WS2811_TARGET_FREQ;
const int GPIO_PIN_CHAN_0 = 18;
const int GPIO_PIN_CHAN_1 = 13;
const int DMA = 5;
const int NUM_LEDS = 60;
const int BRIGHTNESS_PERCENT = 50;
const int BRIGHTNESS = BRIGHTNESS_PERCENT * 255 / 100;

ws2811_t leds;
const u16 numFramesPerSecond = 60;

static void setup_handlers(void);

static void ctrl_c_handler(int signum);

class Ws281xStringRPi : public Ws281xString
{
  public:
  Ws281xStringRPi(u16 channelIdx);

  void set(u16 pixelIdx, const Color&);

  Color get(u16 pixelIdx);

  u16 len();

  private:
  u16 channelIdx_;
};

Ws281xStringRPi::Ws281xStringRPi(u16 channelIdx)
{
  channelIdx_ = channelIdx;
}

void Ws281xStringRPi::set(u16 pixelIdx, const Color& color)
{
  leds.channel[channelIdx_].leds[pixelIdx] = color.rgb();
}

Color Ws281xStringRPi::get(u16 pixelIdx)
{
  return Color((u32)(leds.channel[channelIdx_].leds[pixelIdx]));
}

u16 Ws281xStringRPi::len()
{
  return NUM_LEDS;
}

typedef vector<Ws281xEffect*> EffectPtrVec;

void initLines();
void selectLine(int lineIdx);
void selectAllLines();
void runEffect(Ws281xEffect& effect, u16 runSec);
void runEffectVec(EffectPtrVec& effectPtrVec, Ws281xString& strip, u16 runSec);
void runEffectCopy(Ws281xEffect& effect, u16 runSec);
void clampLight(u8 stringIdx, int clamp);
void dimLight(u8 stringIdx, float light_factor);
void reverse(Ws281xString& effect);

// rsync -av . pi6w:ws281x && ssh pi6w "pkill -e main; true" && ssh pi6w "cd
// ws281x/rpi_ws281x && ./make.sh && ./main"

int main(int argc, char** argv)
{
  leds.freq = TARGET_FREQ;
  leds.dmanum = DMA;

  leds.channel[0].gpionum = GPIO_PIN_CHAN_0;
  leds.channel[0].count = NUM_LEDS;
  leds.channel[0].invert = 1;
  leds.channel[0].strip_type = WS2811_STRIP_GRB;
  leds.channel[0].brightness = BRIGHTNESS;

  leds.channel[1].gpionum = GPIO_PIN_CHAN_1;
  leds.channel[1].count = NUM_LEDS;
  leds.channel[1].invert = 1;
  leds.channel[1].strip_type = WS2811_STRIP_GRB;
  leds.channel[1].brightness = BRIGHTNESS;

  srand(time(NULL));
  setup_handlers();
  initLines();

  if (ws2811_init(&leds)) {
    printf("ws2811 library init failed\n");
    return 1;
  }

  if (argc == 2 && argv[1] == string("off")) {
    selectAllLines();
    for (int i = 0; i < NUM_LEDS; ++i) {
      leds.channel[0].leds[i] = 0;
    }
    ws2811_render(&leds);
    ws2811_fini(&leds);
    return 0;
  }

  // If RGB values are provided on the command line, set all LEDs to that color
  // and exit.
  if (argc == 4) {
    int r = atoi(argv[1]);
    int g = atoi(argv[2]);
    int b = atoi(argv[3]);
    for (int i = 0; i < NUM_LEDS; ++i) {
      leds.channel[0].leds[i] = r << 16 | g << 8 | b;
      leds.channel[1].leds[i] = r << 16 | g << 8 | b;
    }
    //    clampLight(0, 128);
    selectAllLines();
    ws2811_render(&leds);
    ws2811_fini(&leds);
    return 0;
  }

  EffectPtrVec v;

  while (1) {
    u16 runSec = 1 * 60;
    //    u16 runSec = 5;

    Ws281xStringRPi strip(1);

    // Shimmer

    for (auto e : v) {
      delete e;
    }

    v.clear();

    float hsvWidth = random_f(0.0f, 360.0f / 2.0f);
//    float limitHsv = 319.9f - hsvWidth;
    float minHsv = random_f(0.0f, 2.0f * 360.0f);
    float maxHsv = minHsv + hsvWidth;

    for (int j = 0; j < 4; ++j) {
      auto shimmerEffect = new Ws281xEffectShimmer(
          strip, 0.0f, 1.0f, // brightness
          0.01f, 0.02f, // speed
          minHsv, maxHsv // hsv
          );
      v.push_back(shimmerEffect);
    }

    runEffectVec(v, strip, runSec);

    continue;

    // Xmas twinkles
    EffectPtrVec v;
    for (int j = 0; j < 4; ++j) {
      v.push_back(new Ws281xEffectXmasTwinkles(strip));
    }
    runEffectVec(v, strip, runSec);

    // Slow color fade
    Ws281xEffectColorFade effectColorFade0(strip, 5, 1);
    runEffectCopy(effectColorFade0, runSec);

    // Fast color fade
    Ws281xEffectColorFade effectColorFade1(strip, 1, 2);
    runEffectCopy(effectColorFade1, runSec);

    // HSV scroll 1
    Ws281xEffectHsvScroll effectHsvScroll1(strip, 1.0f, 15.0f);
    runEffectCopy(effectHsvScroll1, runSec);

    // HSV scroll 2
    Ws281xEffectHsvScroll effectHsvScroll2(strip, 1.0f, 3.0f);
    runEffectCopy(effectHsvScroll2, runSec);

    // Smooth Runners
    for (int i = 0; i < 10; ++i) {
      EffectPtrVec v;
      for (int j = 0; j < 4; ++j) {
        v.push_back(new Ws281xEffectSmoothRunners(strip, i, 4));
      }
      runEffectVec(v, strip, runSec);
      for (int j = 0; j < 4; ++j) {
        delete v[j];
      }
    }

    //    // Emergency
    //    for (int i = 1; i <= 8; ++i) {
    //      Ws281xEffectEmergency effectEmergency0(string0, i, 0);
    //      runEffect(effectEmergency0, runSec);
    //    }
  }

  return 0;
}

void setup_handlers(void)
{
  struct sigaction sa;
  sa.sa_handler = ctrl_c_handler;
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);
}

void ctrl_c_handler(int signum)
{
  ws2811_fini(&leds);
}

void runEffect(Ws281xEffect& effect, u16 runSec)
{
  for (int i = 0; i < runSec * numFramesPerSecond; ++i) {
    effect.refresh();
    // clampLight(0);
    // dimLight(1, 0.5f);
    if (ws2811_render(&leds)) {
      ws2811_fini(&leds);
      return;
    }
    usleep(1000000 / numFramesPerSecond);
  }
}

void runEffectVec(EffectPtrVec& effectPtrVec, Ws281xString& strip, u16 runSec)
{
  for (int i = 0; i < runSec * numFramesPerSecond / 4; ++i) {
    for (int j = 0; j < effectPtrVec.size(); ++j) {
      ws2811_wait(&leds);
      effectPtrVec[j]->refresh();
      if (j == 1 || j == 2) {
        reverse(strip);
      }
      // clampLight(0);
      // dimLight(1, 0.5f);
      selectLine(j);
      if (ws2811_render(&leds)) {
        ws2811_fini(&leds);
        return;
      }
    }
    usleep(1000000 / numFramesPerSecond);
  }
}

void runEffectCopy(Ws281xEffect& effect, u16 runSec)
{
  selectAllLines();
  for (int i = 0; i < runSec * numFramesPerSecond; ++i) {
    effect.refresh();
    // clampLight(0);
    // dimLight(1, 0.5f);
    if (ws2811_render(&leds)) {
      ws2811_fini(&leds);
      return;
    }
    usleep(1000000 / numFramesPerSecond);
  }
}

void initLines()
{
  wiringPiSetup();
  pinMode(0, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void selectLine(int lineIdx)
{
  digitalWrite(0, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  if (lineIdx == 0) {
    digitalWrite(0, HIGH);
  }
  else if (lineIdx == 1) {
    digitalWrite(4, HIGH);
  }
  else if (lineIdx == 2) {
    digitalWrite(5, HIGH);
  }
  else if (lineIdx == 3) {
    digitalWrite(6, HIGH);
  }
}

void selectAllLines()
{
  digitalWrite(0, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
}

// Post effects

void clampLight(u8 stringIdx, int clamp)
{
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds.channel[stringIdx].leds[i] =
        Color::clampBrightness(Color(leds.channel[stringIdx].leds[i]), clamp)
            .rgb();
  }
}

void dimLight(u8 stringIdx, float light_factor)
{
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds.channel[stringIdx].leds[i] =
        Color::dimLightOutput(
            Color(leds.channel[stringIdx].leds[i]), light_factor)
            .rgb();
  }
}

void reverse(Ws281xString& effect)
{
  for (int i = 0; i < effect.len() / 2; ++i) {
    Color c = effect.get(i);
    effect.set(i, effect.get(effect.len() - i - 1));
    effect.set(effect.len() - i - 1, c);
  }
}
