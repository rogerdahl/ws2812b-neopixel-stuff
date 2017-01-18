#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>

#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include <fmt/format.h>

#include "emu.h"
#include "shim.h"
#include <int_types.h>

#include "util.h"

#include "color_fade.h"
#include "emergency.h"
#include "hsv_scroll.h"
#include "shimmer.h"
#include "smoothrunners.h"
#include "xmas_twinkles.h"

const int WINDOW_W = 1000;
const int WINDOW_H = 1000;

enum PixelLayout
{
  Strip,
  Ring,
  Grid,
};

// const PixelLayout PIXEL_LAYOUT = Strip;
const PixelLayout PIXEL_LAYOUT = Ring;
// const PixelLayout PIXEL_LAYOUT = Grid;

static void errorCallback(int error, const char* description);
static void keyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods);
void drawLeds(ColorVec& leds);
void drawLedStrip(ColorVec& leds);
void drawLedRing(ColorVec& leds);
void drawLedGrid(ColorVec& leds);
void drawFilledCircle(float x, float y, float r, u8 R, u8 G, u8 B);

int main()
{
  srand(time(NULL));

  glfwInit();
  glfwSetErrorCallback(errorCallback);

  GLFWwindow* window =
      glfwCreateWindow(WINDOW_W, WINDOW_H, "NeoPixel Emulator", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return 1;
  }

  //  /*resizable*/true, /*fullscreen*/false, /*colorBits*/8,
  //  /*alphaBits*/8, /*depthBits*/24, /*stencilBits*/8,
  //  /*nSamples*/4, /*glMajor*/3, /*glMinor*/3)

  glfwSetKeyCallback(window, keyCallback);
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glViewport(0, 0, (GLsizei)WINDOW_W, (GLsizei)WINDOW_H);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WINDOW_W, WINDOW_H, 0, 0, 1);

  glfwSwapInterval(1);
  glEnable(GL_SMOOTH);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);

  ColorVec leds(60);
  Ws281xStringEmu emu(leds);

  Ws281xEffectColorFade colorFadeEffect(
      emu, 1 /* fadeSpeed */, 1 /* fadeStep */);
  Ws281xEffectEmergency emergencyEffect(emu, 1 /* numSections */, 1 /* param */);
  Ws281xEffectHsvScroll hsvScrollEffect(
      emu, 1.0f /* scrollSpeed */, 1.0f /* scrollStep */);
  Ws281xEffectSmoothRunners SmoothRunnersEffect(emu, 0, 4);
  Ws281xEffectXmasTwinkles xmasTwinklesEffect(emu);

  Ws281xEffectShimmer* shimmerEffect =
      new Ws281xEffectShimmer(emu, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

  int m = 1000;
  int c = 0;

  while (!glfwWindowShouldClose(window)) {
    if (!c--) {
      c = m;

      float hsvWidth = random_f(0.0f, 310.0f);
      float limitHsv = 319.9f - hsvWidth;
      float minHsv = random_f(0.0f, limitHsv);
      float maxHsv = minHsv + hsvWidth;

      delete shimmerEffect;

      shimmerEffect = new Ws281xEffectShimmer(
          emu, 0.0f, 1.0f, // brightness
          0.01f, 0.02f, // speed
          minHsv, maxHsv // hsv
          );
    }

    shimmerEffect->refresh();

    drawLeds(leds);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void drawLeds(ColorVec& leds)
{
  glClear(GL_COLOR_BUFFER_BIT);
  switch (PIXEL_LAYOUT) {
  case Strip:
    drawLedStrip(leds);
    break;
  case Ring:
    drawLedRing(leds);
    break;
  case Grid:
    drawLedGrid(leds);
    break;
  default:
    assert(false);
  }
}

void drawLedStrip(ColorVec& leds)
{
  float xCenter = 500.0f;
  float yCenter = 500.0f;
  float maxLedRadius = 50.0f;
  float maxSize = 760.0f;
  float ledToSpaceRatio = 1.5f;

  float ledRadius = maxLedRadius;
  float ledAndSpaceSize = ledRadius * ledToSpaceRatio;
  float size = leds.size() * ledAndSpaceSize;
  if (size > maxSize) {
    size = maxSize;
    ledRadius = size / leds.size() / ledToSpaceRatio;
    ledAndSpaceSize = ledRadius * ledToSpaceRatio;
  }
  for (int x = 0; x < static_cast<int>(leds.size()); ++x) {
    auto c = leds[x];
    drawFilledCircle(
        xCenter - size / 2.0f + (ledAndSpaceSize / 2.0f) + x * ledAndSpaceSize,
        yCenter, ledRadius, c.r, c.g, c.b);
  }
}

void drawLedRing(ColorVec& leds)
{
  float xCenter = 500.0f;
  float yCenter = 500.0f;
  float maxCircleRadius = 380.0f;
  float maxLedRadius = 50.0f;
  float ledToSpaceRatio = 2.0f;

  float ledRadius = maxLedRadius;
  float circleRadius =
      leds.size() * (ledRadius * ledToSpaceRatio) / (2.0f * M_PI);
  if (circleRadius > maxCircleRadius) {
    circleRadius = maxCircleRadius;
    float c = circleRadius * 2.0f * M_PI;
    ledRadius = c / leds.size() / ledToSpaceRatio;
  }
  for (int i = 0; i < static_cast<int>(leds.size()); ++i) {
    auto c = leds[i];
    drawFilledCircle(
        xCenter + (circleRadius * cos(i * 2.0f * M_PI / leds.size())),
        yCenter + (circleRadius * sin(i * 2.0f * M_PI / leds.size())),
        ledRadius, c.r, c.g, c.b);
  }
}

// Draw LED grid where the LEDs are ordered in a continuous, back-and-forth
// sequence.
void drawLedGrid(ColorVec& leds)
{
  float xCenter = 500.0f;
  float yCenter = 500.0f;
  float maxLedRadius = 50.0f;
  float maxDimSize = 760.0f;
  float ledToSpaceRatio = 1.5f;

  int numLedsEachDim = sqrt(leds.size());
  float ledRadius = maxLedRadius;
  float ledAndSpaceSize = ledRadius * ledToSpaceRatio;
  float dimSize = numLedsEachDim * ledAndSpaceSize;
  if (dimSize > maxDimSize) {
    dimSize = maxDimSize;
    ledRadius = dimSize / numLedsEachDim / ledToSpaceRatio;
    ledAndSpaceSize = ledRadius * ledToSpaceRatio;
  }

  int numLedsX = numLedsEachDim;
  int numLedsY = numLedsEachDim;

  for (int y = 0; y < numLedsY; ++y) {
    for (int x = 0; x < numLedsX; ++x) {
      Color c;
      if (y & 1) {
        c = leds[x + y * numLedsX];
      }
      else {
        c = leds[numLedsX - x - 1 + y * numLedsX];
      }
      drawFilledCircle(
          xCenter - dimSize / 2.0f + (ledAndSpaceSize / 2.0f)
              + x * ledAndSpaceSize,
          yCenter - dimSize / 2.0f + (ledAndSpaceSize / 2.0f)
              + y * ledAndSpaceSize,
          ledRadius, c.r, c.g, c.b);
    }
  }
}

void drawFilledCircle(float x, float y, float r, u8 R, u8 G, u8 B)
{
  glColor3ub(R, G, B);
  glPointSize(r);
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  glEnd();
}

void errorCallback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
