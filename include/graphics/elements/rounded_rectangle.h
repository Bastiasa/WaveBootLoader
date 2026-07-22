#ifndef WAVE_RND_RECT_IMPL
#define WAVE_RND_RECT_IMPL

#include <graphics/object.h>
#include <graphics/vector2.h>

typedef struct {
    WAVE_GRAPHIC_OBJECT Object;
    VEC2 AnchorPoint;
    VEC2 Size;
    UINT32 Color;
    UINT32 Radius;
} WAVE_RND_RECT;

WAVE_RND_RECT *CreateRoundedRectangle(
    WAVE_CANVAS_MANAGER *canvas,
    VEC2 position,
    VEC2 size,
    UINT32 radius,
    UINT32 color
);

#endif