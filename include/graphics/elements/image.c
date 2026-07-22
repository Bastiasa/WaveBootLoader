#include "image.h"
#include <graphics/util.h>

void OnImageForDraw(void *object) {

    OnObjectRender(object);

    WAVE_IMAGE *image = (WAVE_IMAGE *) object;

    VEC2 scaledSize = VectorMultiply(image->Size, image->Object.FinalScale);
    
    VEC2 anchoredPosition = ANCHORED_POSITION(
        image->Object.FinalPosition,
        image->AnchorPoint,
        scaledSize
    );

    DrawImage(
        image->Object.Canvas,
        image->ImageData->Pixels,
        WV2(
            image->ImageData->Width,
            image->ImageData->Height
        ),
        anchoredPosition,
        scaledSize
    );

}



WAVE_IMAGE *CreateImage(
    WAVE_CANVAS_MANAGER *canvas,
    WAVE_IMAGE_METADATA *meta,
    VEC2 position,
    VEC2 size
) {

    WAVE_GRAPHIC_OBJECT object = CreateEmptyObject(
        canvas,
        position,
        OnImageForDraw
    );

    WAVE_IMAGE *image = AllocatePool(sizeof(WAVE_IMAGE));

    if (image == NULL)
    {
        return;
    }

    image->AnchorPoint = DIAGONAL_V2(0);
    image->Object = object;
    image->Size = size;
    image->ImageData = meta;

    return image;
}