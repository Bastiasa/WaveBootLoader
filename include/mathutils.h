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

#endif