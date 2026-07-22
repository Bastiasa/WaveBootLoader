#ifndef WAVE_GRAPHICS_UTILITIES
#define WAVE_GRAPHICS_UTILITIES

#include "canvas.h"

void DrawRectangle(
    WAVE_CANVAS_MANAGER *canvas,
    VEC2 position,
    VEC2 size,
    UINT32 color
);

void Fill32(UINT32 *dst, UINT32 value, UINT32 count);

#endif