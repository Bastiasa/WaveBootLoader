#ifndef WAVE_MATHUTILS_H
#define WAVE_MATHUTILS_H

#include "efi.h"

static inline float WaveAbs(float value)
{
    return value < 0.0f ? -value : value;
}

static inline float WaveMin(float a, float b)
{
    return a < b ? a : b;
}

static inline float WaveMax(float a, float b)
{
    return a > b ? a : b;
}

static inline float WaveClamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

static inline INT32 WaveFloor(float value)
{
    INT32 i = (INT32)value;

    if (value < 0.0f && value != (float)i)
        return i - 1;

    return i;
}

static inline INT32 WaveCeil(float value)
{
    INT32 i = (INT32)value;

    if (value > 0.0f && value != (float)i)
        return i + 1;

    return i;
}

static inline INT32 WaveRound(float value)
{
    return (INT32)(value >= 0.0f ? value + 0.5f : value - 0.5f);
}

static inline float WaveLerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

static inline UINT32 WaveLerpColor(UINT32 colorA, UINT32 colorB, float t)
{
    UINT32 ar = (colorA >> 16) & 0xFF;
    UINT32 ag = (colorA >> 8)  & 0xFF;
    UINT32 ab =  colorA        & 0xFF;

    UINT32 br = (colorB >> 16) & 0xFF;
    UINT32 bg = (colorB >> 8)  & 0xFF;
    UINT32 bb =  colorB        & 0xFF;

    UINT32 r = (UINT32)(WaveLerp((float)ar, (float)br, t) + 0.5f);
    UINT32 g = (UINT32)(WaveLerp((float)ag, (float)bg, t) + 0.5f);
    UINT32 b = (UINT32)(WaveLerp((float)ab, (float)bb, t) + 0.5f);

    return (r << 16) | (g << 8) | b;
}

#endif