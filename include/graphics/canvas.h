#ifndef WAVE_CANVAS_H
#define WAVE_CANVAS_H

#include "efi.h"
#include "vector2.h"


typedef struct {
    void **Children;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

    UINT32* BackBuffer;
    UINT32 BufferSize;

    UINT32 Width;
    UINT32 Height;
} WAVE_CANVAS_MANAGER;

void PutPixel(WAVE_CANVAS_MANAGER *canvas, UINT32 x, UINT32 y, UINT32 color);
void RenderCanvas(WAVE_CANVAS_MANAGER *canvas);

void CanvasAdd(WAVE_CANVAS_MANAGER *canvas, void *child);

void CanvasStep(WAVE_CANVAS_MANAGER *canvas);

WAVE_CANVAS_MANAGER *CreateCanvas(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    EFI_SIMPLE_POINTER_PROTOCOL *MouseProtocol
);

#endif