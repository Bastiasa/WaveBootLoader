#include "rectangle.h"
#include "mathutils.h"
#include "efi.h"
#include "efilib.h"
#include <graphics/util.h>


void OnRectangleForRender(void *object)
{
    WAVE_RECTANGLE *rectangle = (WAVE_RECTANGLE *)object;

    UINT32 *fb =
        (UINT32 *)rectangle->Object.Canvas->BackBuffer;

    UINT32 stride =
        rectangle->Object.Canvas->Gop->Mode->Info->PixelsPerScanLine;

    INT32 width  = WaveRound(rectangle->Size.x);
    INT32 height = WaveRound(rectangle->Size.y);

    INT32 x = WaveRound(rectangle->Object.Position.x);
    INT32 y = WaveRound(rectangle->Object.Position.y);

    UINT32 screenWidth = rectangle->Object.Canvas->Width;
    UINT32 screenHeight = rectangle->Object.Canvas->Height;

    if (x >= (INT32)screenWidth ||
        y >= (INT32)screenHeight)
    {
        return;
    }

    if (x + width <= 0 ||
        y + height <= 0)
    {
        return;
    }

    if (x < 0)
    {
        width += x;
        x = 0;
    }

    if (y < 0)
    {
        height += y;
        y = 0;
    }

    if (x + width > (INT32)screenWidth)
    {
        width = screenWidth - x;
    }

    if (y + height > (INT32)screenHeight)
    {
        height = screenHeight - y;
    }

    if (width <= 0 || height <= 0)
    {
        return;
    }

    for (UINT32 j = 0; j < height; j++)
    {
        UINT32 *row = fb + (y + j) * stride + x;
        Fill32(row, rectangle->Color, width);
    }
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

    return rectangle;
}
