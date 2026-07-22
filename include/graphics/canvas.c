#include <efi.h>
#include <efilib.h>

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
    uefi_call_wrapper(
        canvas->Gop->Blt,
        10,
        canvas->Gop,
        (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)canvas->BackBuffer,
        EfiBltBufferToVideo,
        0, 0,           // SourceX, SourceY
        0, 0,           // DestinationX, DestinationY
        canvas->Gop->Mode->Info->HorizontalResolution,
        canvas->Gop->Mode->Info->VerticalResolution,
        0               // Delta = 0 significa usar HorizontalResolution como stride del buffer fuente
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

void RenderChild(void *child) {
    WAVE_GRAPHIC_OBJECT *CastedChild = (WAVE_GRAPHIC_OBJECT *)child;

    if (!CastedChild)
    {
        return;
    }

    if (CastedChild->Visible)
    {
        CastedChild->Render(child);
    }
    
    for (size_t i = 0; i < arrlen(CastedChild->Children); i++)
    {
        RenderChild(CastedChild->Children[i]);
    }
    
};

void RenderCanvas(WAVE_CANVAS_MANAGER *canvas) {
    ClearCanvas(canvas);

    if (canvas->Children == NULL)
    {
        return;
    }
    

    for(int i = 0; i < arrlen(canvas->Children); i++) {
        RenderChild(canvas->Children[i]);
    }

    PushRender(canvas);
}



void CanvasUpdateKey(WAVE_CANVAS_MANAGER *canvas) {

    EFI_INPUT_KEY Key;

    EFI_STATUS Status = uefi_call_wrapper(
        canvas->SystemTable->ConIn->ReadKeyStroke,
        2,
        canvas->SystemTable->ConIn,
        &Key
    );

    if (Status == EFI_SUCCESS)
    {
        canvas->LastKey = canvas->CurrentKey;
        canvas->CurrentKey = Key.ScanCode;

        canvas->UnicodeLastKey = canvas->UnicodeCurrentKey;
        canvas->UnicodeCurrentKey = Key.UnicodeChar;
    } else if (Status == EFI_NOT_READY)
    {
        canvas->LastKey = canvas->CurrentKey;
        canvas->CurrentKey = SCAN_NULL;

        canvas->UnicodeLastKey = canvas->UnicodeCurrentKey;
        canvas->UnicodeCurrentKey = L"";
    }
    
    
}

void CanvasStep(WAVE_CANVAS_MANAGER *canvas) {
    CanvasUpdateKey(canvas);
}

EFI_STATUS InitCanvas(WAVE_CANVAS_MANAGER *canvas) {

    EFI_STATUS Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

    Status = uefi_call_wrapper(
        canvas->SystemTable->BootServices->LocateProtocol,
        3,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        (void **)&Gop
    );

    if (EFI_ERROR(Status)) {
        Print(L"No se pudo localizar GOP: %r\n", Status);
        return Status;
    }

    if (Gop->Mode->Info->PixelFormat == PixelBltOnly)
    {
        Print(L"Este GOP no soporta acceso directo al framebuffer\n");
        return EFI_ABORTED;
    }


    UINT32 width = Gop->Mode->Info->HorizontalResolution;
    UINT32 height = Gop->Mode->Info->VerticalResolution;

    const UINT32 bufferSize = width * height * sizeof(UINT32);

    canvas->Gop = Gop;
    canvas->Width = (float) width;
    canvas->Height = (float) height;
    
    canvas->BackBuffer = AllocatePool(
        bufferSize
    );

    canvas->BufferSize = bufferSize;

    return EFI_SUCCESS;
}

WAVE_CANVAS_MANAGER *CreateCanvas(
    EFI_SYSTEM_TABLE *SystemTable,
    EFI_HANDLE *ImageHandle
)
{
    WAVE_CANVAS_MANAGER *canvas = AllocatePool(sizeof(WAVE_CANVAS_MANAGER));

    if (!canvas)
        return NULL;

    canvas->SystemTable = SystemTable;
    canvas->ImageHandle = ImageHandle;
    canvas->Children = NULL;

    return canvas;
}
