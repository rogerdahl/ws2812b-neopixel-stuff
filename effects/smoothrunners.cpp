#include "smoothrunners.h"
#include "util.h"

// Misc effects that are based on additively mixing colored segments. Segment and LED positions are first calculated
// with a virtual string of LEDs that has 256x more LEDs than the physical string. These virtual "superpositions" are
// then averaged to determine the colors for the physical LEDs.

const u32 brightColors[] = { 0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0x00ffff, 0xff00ff };

Segment::Segment()
: color(Color(0))
{
}

Segment::Segment(const Color& color_in, s16 speed_in, u8 lengthPercent_in)
: color(color_in), speed(speed_in), lengthPercent(lengthPercent_in)
{
}

Ws281xEffectSmoothRunners::Ws281xEffectSmoothRunners(Ws281xString& pixels, u8 effectIdx, u8 speedFactor)
: pixels_(pixels), effectIdx_(effectIdx), speedFactor_(speedFactor)
{
    switch (effectIdx) {
    case 0:
    {
        // Old theater running lights.
        numSegments_ = 5;
        for (u8 i = 0; i < numSegments_; ++i) {
            segmentArr[i] = { 0xff6800, 80, 10 };
        }
        break;
    }
    case 1:
    {
        numSegments_ = 5;
        for (u8 i = 0; i < numSegments_; ++i) {
            segmentArr[i] = { randomHsvColor(), 80, 10 };
        }
        break;
    }

    case 2:
    {
        numSegments_ = 6;
        for (u8 i = 0; i < numSegments_; ++i) {
            segmentArr[i] = { randomHsvColor(), 80, 5 };
        }
        break;
    }
    case 3:
    {
        numSegments_ = 12;
        for (u8 i = 0; i < numSegments_; i += 2) {
            Color c = randomHsvColor();
            segmentArr[i] = { c, 40, 5 };
            segmentArr[i + 1] = { c, -40, 5 };
        }
        break;
    }

    case 4:
    {
        numSegments_ = 10;
        for (u8 i = 0; i < numSegments_; ++i) {
            segmentArr[i] = { randomHsvColor(), 80, 2 };
        }
        break;
    }
    case 5:
    {
        numSegments_ = 20;
        for (u8 i = 0; i < numSegments_; i += 2) {
            Color c = randomHsvColor();
            segmentArr[i] = { c, 40, 2 };
            segmentArr[i + 1] = { c, -40, 2 };
        }
        break;
    }

    case 6:
    {
        // Random colors, many short segments, both directions, uneven spacing
        numSegments_ = 12;
        for (u8 i = 0; i < numSegments_; ++i) {
            segmentArr[i] = { randomHsvColor(), (s16)random(-80, 80), 3 };
        }
        break;
    }
    case 7:
    {
        // Random colors, segments a bit longer, both directions, uneven spacing
        numSegments_ = 12;
        for (u8 i = 0; i < numSegments_; ++i) {
            segmentArr[i] = { randomHsvColor(), (s16)random(-80, 80), 8 };
        };
        break;
    }
    case 8:
    {
        // Even rainbow.
        numSegments_ = 3;
        for (u8 i = 0; i < numSegments_; ++i) {
            //segmentArr[i] = { brightColors[i], 100, 75 };
            segmentArr[i] = { randomHsvColor(), 100, 75 };
        }
        break;
    }
    case 9:
    {
        // Uneven rainbow.
        numSegments_ = 3;
        s16 speedArr[] = {-100, -120, -140};
        for (u8 i = 0; i < numSegments_; ++i) {
//            segmentArr[i] = { brightColors[i], speedArr[i], 75 };
            segmentArr[i] = { randomHsvColor(), speedArr[i], 75 };
        }
        break;
    }


    case 10:
    {
        // Darker rainbow.
        numSegments_ = 3;
        s16 speedArr[] = {-60, -70, -80};
        for (u8 i = 0; i < numSegments_; ++i) {
//            segmentArr[i] = { brightColors[i], speedArr[i], 50 };
            segmentArr[i] = { randomHsvColor(), speedArr[i], 50 };
        }
        break;
    }
    case 11:
    {
        // Shimmer 1.
        numSegments_ = 3;
        s16 speedArr[] = {1230, 4560, -7890};
        for (u8 i = 0; i < numSegments_; ++i) {
//            segmentArr[i] = { brightColors[i], speedArr[i], 75 };
            segmentArr[i] = { randomHsvColor(), speedArr[i], 75 };
        }
        break;
    }
    case 12:
    {
        // Shimmer 2.
        numSegments_ = 3;
        s16 speedArr[] = {10000, 20000, -15000};
        for (u8 i = 0; i < numSegments_; ++i) {
//            segmentArr[i] = { brightColors[i], speedArr[i], 75 };
            segmentArr[i] = { randomHsvColor(), speedArr[i], 75 };
        }
        break;
    }
    };

    numSuperPositions = pixels_.len() << 8;
    superPosOffset = numSuperPositions / numSegments_;
    for (u8 i = 0; i < numSegments_; ++i) {
        superPosBuf_[i] = i * superPosOffset;
    }
}

void Ws281xEffectSmoothRunners::calcColors()
{
    for (u16 i = 0; i < pixels_.len(); ++i) {
        pixels_.set(i, Color(0x000000));
    }

    for (u8 i = 0; i < numSegments_; ++i) {
        u16 superPos = superPosBuf_[i];
        superPos = wrapAdd(superPos, segmentArr[i].speed / speedFactor_, numSuperPositions);
        superPosBuf_[i] = superPos;
        u8 numSegmentPixels = segmentArr[i].lengthPercent * pixels_.len() / 100;
        if (numSegmentPixels < 2) {
            numSegmentPixels = 2;
        }
        drawTaperedSegment(superPos, numSegmentPixels, segmentArr[i].color);
    }
}

void Ws281xEffectSmoothRunners::drawTaperedSegment(u16 superPos, u8 numSegmentPixels, const Color& c)
{
    u8 shiftPos = superPos & 0xff;
    u8 pixelPos = superPos >> 8;
    for (u8 i = 0; i <= numSegmentPixels; ++i) {
        Color c1 = calcTaperedSegmentPixel(numSegmentPixels, i, c);
        Color c2 = calcTaperedSegmentPixel(numSegmentPixels, i + 1, c);
        Color avgColor = colorWeightedAvg(c1, c2, 255 - shiftPos);
        addPixelColor(pixelPos, avgColor);
        pixelPos = wrapAdd(pixelPos, 1, pixels_.len() - 1);
    }
}

// Example for 7 pixels.
//
//        X
//       X X
//      X   X
//     X     X
//
// Pixel 3 is center and will be set to brightestVal.
// There are 4 pixels on each side, sharing the center pixel.
// The values of the 4 pixels are assigned in such a way that an imagined 5th pixel on each side is 0.
// So, for 4 pixels:
//   single step = 255 / 4 = 63
//   steps for 7 pixels: 64, 127, 191, 255, 191, 127, 64
Color Ws281xEffectSmoothRunners::calcTaperedSegmentPixel(u8 numSegmentPixels, u8 pos, const Color& centerColor)
{
    if (pos == 0 || pos == numSegmentPixels + 1) {
        return Color(0);
    }

    u8 centerPos = numSegmentPixels / 2;
    u8 rStep = centerColor.r / (centerPos + 1);
    u8 gStep = centerColor.g / (centerPos + 1);
    u8 bStep = centerColor.b / (centerPos + 1);

    u8 segmentIndex;
    if (pos <= centerPos) {
        segmentIndex = pos + 1;
    }
    else {
        segmentIndex = numSegmentPixels - pos;
    }

    return Color((u8)(rStep * segmentIndex), (u8)(gStep * segmentIndex), (u8)(bStep * segmentIndex));
}

void Ws281xEffectSmoothRunners::addPixelColor(u16 pixelPos, const Color& addColor)
{
    Color c = pixels_.get(pixelPos);
    c.additiveMix(addColor);
    pixels_.set(pixelPos, c);
}
