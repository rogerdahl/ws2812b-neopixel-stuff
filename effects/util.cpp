#include "util.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <cmath>
#include <cassert>

//void clear(u32 color, u16 clearMsec)
//{
//    u16 delayMsec = clearMsec / pixels.numPixels();
//    for (u16 i = 0; i < pixels.numPixels(); ++i) {
//        pixels.setPixelColor(i, color);
//        if (delayMsec) {
//            pixels.show();
//            delay(delayMsec);
//        }
//    }
//}

s16 wrap(s16 val, s16 maxVal)
{
    if (val < 0) {
        return val + maxVal;
    }
    else if (val >= maxVal) {
        return val - maxVal;
    }
    else {
        return val;
    }
}

u16 wrapAdd(s16 val, s16 addVal, s16 maxVal)
{
    s16 t = val + addVal;
    if (t > maxVal) {
        return t - maxVal - 1;
    }
    else if (t < 0) {
        return maxVal + t + 1;
    }
    else {
        return t;
    }
}

Color randomBrightColor() {
    u8 r = random(0, 255);
    u8 g = random(0, 255);
    u8 b = random(0, 255);
    u8 m = max(max(r, g), b);
    float mf = 255.0 / m;
    r = int(r * mf);
    g = int(g * mf);
    b = int(b * mf);
    return Color(r, g, b);
}

Color randomDimColor() {
    // Useful in effects where there's a lot of additive mixing.
    Color c = randomBrightColor();
    c.r /= 2;
    c.g /= 2;
    c.b /= 2;
    return c;
}

Color randomColorChannelBlank(const Color& c) {
    u32 rgb = c.rgb();
    u8 skipChannel = random(0, 2);
    rgb = ror(rgb, skipChannel * 8);
    rgb &= 0xffff00;
    rgb = rol(rgb, skipChannel * 8);
    return Color(rgb);
}

Color randomTwoColor() {
    Color c = randomBrightColor();
    return randomColorChannelBlank(c);
}

Color randomHsvColor() {
    return fromHsv(random(0, 360), 1.0f, 1.0f);
}

Color fromHsv(float h, float s, float v)
{
    assert(h >= 0.0f && h <= 360.0f);
    assert(s >= 0.0f && s <= 1.0f);
    assert(v >= 0.0f && v <= 1.0f);

    float c = v * s;
    float x = c * (1.0f - fabs(fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;
    if (h >= 300.0) {
        return Color(c + m, m, x + m);
    }
    if (h >= 240.0) {
        return Color(x + m, m, c + m);
    }
    if (h >= 180.0) {
        return Color(m, x + m, c + m);
    }
    if (h >= 120.0) {
        return Color(m, c + m, x + m);
    }
    if (h >= 60.0) {
        return Color(x + m, c + m, m);
    }
    return Color(c + m, x + m, m);
}

Color colorWeightedAvg(const Color& c1, const Color& c2, u8 weight)
{
    return Color(
        channelWeightedAvg(c1.r, c2.r, weight),
        channelWeightedAvg(c1.g, c2.g, weight),
        channelWeightedAvg(c1.b, c2.b, weight)
    );
}

u8 channelWeightedAvg(u8 a, u8 b, u8 weight)
{
    u16 aa = a * (256 - weight);
    u16 bb = b * weight;
    return (aa + bb) >> 8;
}

void delay(int ms)
{
    usleep(ms * 1000);
}

int random(int min, int max)
{
    return (rand() % (max + 1 - min)) + min;
}

uint32_t millis()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000 + t.tv_usec / 1000;
}

s32 max(s32 a, s32 b) {
    return a > b ? a : b;
}

u32 rol(u32 n, u8 c)
{
  const u8 mask = (8 * sizeof(n) - 1);
  return (n << c) | (n >> ((-c)&mask));
}

u32 ror(u32 n, u8 c)
{
  const u8 mask = (8 * sizeof(n) - 1);
  return (n >> c) | (n << ((-c)&mask));
}
