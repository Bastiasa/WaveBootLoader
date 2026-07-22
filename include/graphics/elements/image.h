#ifndef WAVE_IMAGE_IMPL
#define WAVE_IMAGE_IMPL

#include <graphics/object.h>
#include <graphics/vector2.h>
#include <image_metadata.h>

typedef struct 
{
    WAVE_GRAPHIC_OBJECT Object;
    VEC2 AnchorPoint;
    VEC2 Size;
    WAVE_IMAGE_METADATA *ImageData;
} WAVE_IMAGE;


WAVE_IMAGE *CreateImage(
    WAVE_CANVAS_MANAGER *canvas,
    WAVE_IMAGE_METADATA *meta,
    VEC2 position,
    VEC2 size
);


#endif