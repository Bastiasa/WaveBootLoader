#include "efi.h"
#include "efilib.h"
#include "stb_ds.h"
#include "canvas.h"
#include "object.h"
#include "util.h"


void PutPixel(WAVE_CANVAS_MANAGER *canvas,
              UINT32 x,
              UINT32 y,
              UINT32 color)
{
    UINT32 width =
        canvas->Gop->Mode->Info->HorizontalResolution;

    UINT32 height =
        canvas->Gop->Mode->Info->VerticalResolution;

    if (x >= width || y >= height)
        return;

    UINT32 *fb = canvas->BackBuffer;

    fb[y * canvas->Gop->Mode->Info->PixelsPerScanLine + x]
        = color;
}

void PushRender(WAVE_CANVAS_MANAGER *canvas) {
    CopyMem(
        (void *)canvas->Gop->Mode->FrameBufferBase,
        canvas->BackBuffer,
        canvas->BufferSize
    );
}

void ClearCanvas(WAVE_CANVAS_MANAGER *canvas)
{
    UINT32 *fb =
        (UINT32 *)canvas->BackBuffer;

    UINT32 pixels =
        canvas->Gop->Mode->Info->PixelsPerScanLine *
        canvas->Gop->Mode->Info->VerticalResolution;

    SetMem(fb, pixels * sizeof(UINT32), 0);
}

void CanvasAdd(WAVE_CANVAS_MANAGER *canvas, void *child) {
    arrput(canvas->Children, child);
}


void RenderCanvas(WAVE_CANVAS_MANAGER *canvas) {
    ClearCanvas(canvas);

    if (canvas->Children == NULL)
    {
        return;
    }
    

    for(int i = 0; i < arrlen(canvas->Children); i++) {
        void *normalChild = canvas->Children[i];
        WAVE_GRAPHIC_OBJECT *child = (WAVE_GRAPHIC_OBJECT *) canvas->Children[i];
        child->Render(normalChild);
    }

    PushRender(canvas);
}

void CanvasStep(WAVE_CANVAS_MANAGER *canvas) {
}

WAVE_CANVAS_MANAGER *CreateCanvas(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    EFI_SIMPLE_POINTER_PROTOCOL *MouseProtocol
)
{

    WAVE_CANVAS_MANAGER *canvas = AllocatePool(sizeof(WAVE_CANVAS_MANAGER));

    if (!canvas)
        return NULL;


    UINT32 width = Gop->Mode->Info->HorizontalResolution;
    UINT32 height = Gop->Mode->Info->VerticalResolution;
    const UINT32 bufferSize = width * height * sizeof(UINT32);

    canvas->Gop = Gop;
    canvas->Children = NULL;

    canvas->Width = width;
    canvas->Height = height;
    
    canvas->BackBuffer = AllocatePool(
        bufferSize
    );

    canvas->BufferSize = bufferSize;

    return canvas;
}
