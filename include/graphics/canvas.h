#ifndef WAVE_CANVAS_H
#define WAVE_CANVAS_H

#include "efi.h"
#include "vector2.h"


typedef struct {
    void **Children;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_SYSTEM_TABLE *SystemTable;
    EFI_HANDLE *ImageHandle;

    UINT32* BackBuffer;
    UINT32 BufferSize;

    float Width;
    float Height;

    UINT32 CurrentKey;
    UINT32 LastKey;

    CHAR16 UnicodeCurrentKey;
    CHAR16 UnicodeLastKey;
} WAVE_CANVAS_MANAGER;

void PutPixel(WAVE_CANVAS_MANAGER *canvas, UINT32 x, UINT32 y, UINT32 color);
void RenderCanvas(WAVE_CANVAS_MANAGER *canvas);

void CanvasAdd(WAVE_CANVAS_MANAGER *canvas, void *child);

void CanvasStep(WAVE_CANVAS_MANAGER *canvas);

EFI_STATUS InitCanvas(WAVE_CANVAS_MANAGER *canvas);

WAVE_CANVAS_MANAGER *CreateCanvas(
    EFI_SYSTEM_TABLE *SystemTable,
    EFI_HANDLE *ImageHandle
);

#endif