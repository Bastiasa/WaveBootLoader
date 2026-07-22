#ifndef WAVE_RECTANGLE_OBJECT
#define WAVE_RECTANGLE_OBJECT

#include <graphics/object.h>
#include <graphics/vector2.h>
#include <graphics/canvas.h>

typedef struct 
{
    WAVE_GRAPHIC_OBJECT Object;
    VEC2 Size;
    VEC2 AnchorPoint;
    UINT32 Color;
} WAVE_RECTANGLE;

WAVE_RECTANGLE *CreateRectangle(
    WAVE_CANVAS_MANAGER *canvas, 
    VEC2 position,
    VEC2 size,
    UINT32 color
);

#endif
