#pragma once

#include <stdio.h>

#include "int_types.h"
#include "shim.h"

#define PI_D 3.1415926535897932385
#define PI_F 3.1415926535897932385f

s16 wrap(s16 val, s16 maxVal);
u16 wrapAdd(s16 val, s16 addVal, s16 maxVal);

float clamp(float n, float min, float max);

Color randomBrightColor();
Color randomDimColor();
Color randomColorChannelBlank(const Color& c);
Color randomTwoColor();
Color randomHsvColor();
Color fromHsv(float h, float s, float v);
Color colorWeightedAvg(const Color& c1, const Color& c2, u8 weight);
u8 channelWeightedAvg(u8 a, u8 b, u8 weight);

void delay(u32 ms);
s32 random(s32 min, s32 max);
float random_f(float min, float max);
float random_norm_f();

u32 millis();
s32 max(s32 a, s32 b);
u32 rol(u32 n, u8 c);
u32 ror(u32 n, u8 c);
