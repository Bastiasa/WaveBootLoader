#ifndef WAVE_GRAPHICS_UTILITIES
#define WAVE_GRAPHICS_UTILITIES

#define ANCHORED_POSITION(position, anchorPoint, size) \
    VectorSum( \
        (position), \
        VectorNegative( \
            VectorMultiply( \
                (size), \
                (anchorPoint) \
            ) \
        ) \
    )


#include "canvas.h"

void DrawFilledRect(
    WAVE_CANVAS_MANAGER *canvas,
    VEC2 position,
    VEC2 size,
    UINT32 color
);

void DrawRoundedRect(
    WAVE_CANVAS_MANAGER *canvas,
    VEC2 position,
    VEC2 size,
    UINT32 color,
    UINT32 radius
);


void DrawImage(
    WAVE_CANVAS_MANAGER *canvas,
    UINT32 *pixels,
    VEC2 sourceSize,   // dimensiones originales de la imagen (ancho x alto reales del array pixels)
    VEC2 position,      // dónde dibujar en pantalla
    VEC2 size           // tamaño de destino (al que se debe escalar)
);


void Fill32(UINT32 *dst, UINT32 value, UINT32 count);

#endif