#include <graphics/util.h>
#include "rectangle.h"

#include "mathutils.h"
#include "efi.h"
#include "efilib.h"


void OnRectangleForRender(void *object)
{
    WAVE_RECTANGLE *rectangle = (WAVE_RECTANGLE *)object;

    OnObjectRender(&rectangle->Object);

    VEC2 scaledSize = VectorMultiply(
        rectangle->Size, 
        rectangle->Object.FinalScale
    );

    VEC2 anchoredPosition = ANCHORED_POSITION(
        rectangle->Object.FinalPosition,
        rectangle->AnchorPoint,
        scaledSize
    );
    
    DrawFilledRect(
        rectangle->Object.Canvas,
        anchoredPosition,
        scaledSize,
        rectangle->Color
    );
}

WAVE_RECTANGLE *CreateRectangle(
    WAVE_CANVAS_MANAGER *canvas, 
    VEC2 position,
    VEC2 size,
    UINT32 color
) {

    WAVE_GRAPHIC_OBJECT object = CreateEmptyObject(
        canvas,
        position,
        OnRectangleForRender
    );

    WAVE_RECTANGLE *rectangle = AllocatePool(sizeof(WAVE_RECTANGLE));

    if (rectangle == NULL)
    {
        return NULL;
    }

    rectangle->Object = object;
    rectangle->Color = color;
    rectangle->Size = size;
    rectangle->AnchorPoint = WV2(0, 0);

    return rectangle;
}
