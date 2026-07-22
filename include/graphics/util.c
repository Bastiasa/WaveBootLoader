#include "util.h"
#include "mathutils.h"

void Fill32(UINT32 *dst, UINT32 value, UINT32 count)
{
    for (UINT32 i = 0; i < count; i++)
        dst[i] = value;
}


void DrawRectangle(
    WAVE_CANVAS_MANAGER *canvas,
    VEC2 position,
    VEC2 size,
    UINT32 color
)
{
    UINT32 *fb = canvas->BackBuffer;

    UINT32 stride =
        canvas->Gop->Mode->Info->PixelsPerScanLine;

    INT32 width  = WaveRound(size.x);
    INT32 height = WaveRound(size.y);

    INT32 x = WaveRound(position.x);
    INT32 y = WaveRound(position.y);

    UINT32 screenWidth = canvas->Width;
    UINT32 screenHeight = canvas->Height;

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

    for (INT32 j = 0; j < height; j++)
    {
        UINT32 *row = fb + (y + j) * stride + x;
        Fill32(row, color, width);
    }
}