#include "util.h"
#include "mathutils.h"



void Fill32(UINT32 *dst, UINT32 value, UINT32 count)
{
    for (UINT32 i = 0; i < count; i++)
        dst[i] = value;
}

void DrawImage(
    WAVE_CANVAS_MANAGER *canvas,
    UINT32 *pixels,
    VEC2 sourceSize,
    VEC2 position,
    VEC2 size
)
{
    UINT32 *fb = (UINT32 *)canvas->BackBuffer;
    UINT32 stride = canvas->Gop->Mode->Info->PixelsPerScanLine;

    INT32 x = WaveRound(position.x);
    INT32 y = WaveRound(position.y);

    UINT32 srcWidth  = (UINT32) sourceSize.x;
    UINT32 srcHeight = (UINT32) sourceSize.y;

    UINT32 dstWidth  = (UINT32) WaveRound(size.x);
    UINT32 dstHeight = (UINT32) WaveRound(size.y);

    if (dstWidth == 0 || dstHeight == 0 || srcWidth == 0 || srcHeight == 0)
    {
        return;
    }

    for (UINT32 j = 0; j < dstHeight; j++)
    {
        INT32 py = y + j;

        if (py < 0 || py >= (INT32)canvas->Height)
        {
            continue;
        }

        UINT32 srcY = (j * srcHeight) / dstHeight;

        for (UINT32 i = 0; i < dstWidth; i++)
        {
            INT32 px = x + i;

            if (px < 0 || px >= (INT32)canvas->Width)
            {
                continue;
            }

            UINT32 srcX = (i * srcWidth) / dstWidth;
            UINT32 color = pixels[srcY * srcWidth + srcX];

            UINT8 alpha = (UINT8)((color >> 24) & 0xFF);

            if (alpha == 0)
            {
                continue;
            }

            UINT32 *dst = &fb[py * stride + px];

            if (alpha == 255)
            {
                *dst = color & 0x00FFFFFF; // descarta el byte alpha al escribir
                continue;
            }

            // Color ya viene premultiplicado, solo atenuamos el fondo
            UINT8 srcR = (UINT8)((color >> 16) & 0xFF);
            UINT8 srcG = (UINT8)((color >> 8) & 0xFF);
            UINT8 srcB = (UINT8)(color & 0xFF);

            UINT32 bg = *dst;
            UINT8 dstR = (UINT8)((bg >> 16) & 0xFF);
            UINT8 dstG = (UINT8)((bg >> 8) & 0xFF);
            UINT8 dstB = (UINT8)(bg & 0xFF);

            UINT8 invAlpha = 255 - alpha;

            UINT8 outR = srcR + (UINT8)((dstR * invAlpha) / 255);
            UINT8 outG = srcG + (UINT8)((dstG * invAlpha) / 255);
            UINT8 outB = srcB + (UINT8)((dstB * invAlpha) / 255);

            *dst = (outR << 16) | (outG << 8) | outB;
        }
    }
}
static inline void FillSpan(
    UINT32 *dst,
    INT32 count,
    UINT32 color
)
{
    for (INT32 i = 0; i < count; i++)
    {
        dst[i] = color;
    }
}

void DrawRoundedRect(
    WAVE_CANVAS_MANAGER *canvas,
    VEC2 position,
    VEC2 size,
    UINT32 color,
    UINT32 radius
)
{
    UINT32 *fb = (UINT32 *)canvas->BackBuffer;
    UINT32 stride = canvas->Gop->Mode->Info->PixelsPerScanLine;

    INT32 x = WaveRound(position.x);
    INT32 y = WaveRound(position.y);

    INT32 width  = WaveRound(size.x);
    INT32 height = WaveRound(size.y);

    if (width <= 0 || height <= 0)
    {
        return;
    }

    INT32 r = (INT32)radius;

    INT32 maxRadius =
        ((width < height) ? width : height) / 2;

    if (r > maxRadius)
    {
        r = maxRadius;
    }

    if (r <= 0)
    {
        DrawFilledRect(
            canvas,
            position,
            size,
            color
        );
        return;
    }

    UINT32 screenWidth  = canvas->Width;
    UINT32 screenHeight = canvas->Height;

    INT32 startX = x;
    INT32 startY = y;
    INT32 endX   = x + width;
    INT32 endY   = y + height;

    if (startX >= (INT32)screenWidth ||
        startY >= (INT32)screenHeight)
    {
        return;
    }

    if (endX <= 0 || endY <= 0)
    {
        return;
    }

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    if (endX > (INT32)screenWidth)
        endX = screenWidth;

    if (endY > (INT32)screenHeight)
        endY = screenHeight;

    INT32 rSquared = r * r;

    for (INT32 py = startY; py < endY; py++)
    {
        INT32 localY = py - y;

        INT32 inset = 0;

        if (localY < r)
        {
            INT32 dy = r - localY - 1;

            INT32 dx = 0;

            while ((dx * dx + dy * dy) < rSquared)
            {
                dx++;
            }

            inset = r - dx;
        }
        else if (localY >= height - r)
        {
            INT32 dy =
                localY - (height - r);

            INT32 dx = 0;

            while ((dx * dx + dy * dy) < rSquared)
            {
                dx++;
            }

            inset = r - dx;
        }

        INT32 left  = x + inset;
        INT32 right = x + width - inset;

        if (left < startX)
            left = startX;

        if (right > endX)
            right = endX;

        INT32 spanWidth = right - left;

        if (spanWidth <= 0)
        {
            continue;
        }

        UINT32 *row =
            fb + py * stride + left;

        FillSpan(
            row,
            spanWidth,
            color
        );
    }
}


void DrawFilledRect(WAVE_CANVAS_MANAGER *canvas, VEC2 position, VEC2 size, UINT32 color)
{
    UINT32 *fb = (UINT32 *)canvas->BackBuffer;
    UINT32 stride = canvas->Gop->Mode->Info->PixelsPerScanLine;

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

    for (UINT32 j = 0; j < height; j++)
    {
        UINT32 *row = fb + (y + j) * stride + x;
        Fill32(row, color, width);
    }
}