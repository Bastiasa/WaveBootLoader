#include <graphics/util.h>
#include "rounded_rectangle.h"

void OnRndRectForRender(void *object) {

    OnObjectRender(object);

    WAVE_RND_RECT *rndRect = (WAVE_RND_RECT *)object;

    VEC2 scaledSize = VectorMultiply(rndRect->Size, rndRect->Object.Scale);
    VEC2 anchoredPosition = ANCHORED_POSITION(rndRect->Object.FinalPosition, rndRect->AnchorPoint, scaledSize);

    DrawRoundedRect(
        rndRect->Object.Canvas,
        anchoredPosition,
        scaledSize,
        rndRect->Color,
        rndRect->Radius
    );

}

WAVE_RND_RECT *CreateRoundedRectangle(
    WAVE_CANVAS_MANAGER *canvas,
    VEC2 position,
    VEC2 size,
    UINT32 radius,
    UINT32 color
) {

    WAVE_GRAPHIC_OBJECT object = CreateEmptyObject(
        canvas,
        position,
        OnRndRectForRender
    );

    WAVE_RND_RECT *rndRect = AllocatePool(sizeof(WAVE_RND_RECT));

    rndRect->Object = object;
    rndRect->Color = color;
    rndRect->Radius = radius;
    rndRect->Size = size;
    rndRect->AnchorPoint = VECTOR2_ZERO;

    return rndRect;
}