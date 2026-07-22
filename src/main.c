#include <efi.h>
#include <efilib.h>
#include <efiapi.h>

#include "graphics/canvas.h"
#include "graphics/elements/rectangle.h"
#include "graphics/vector2.h"

#define TARGET_FPS 60
#define FRAME_TIME_US (1000000 / TARGET_FPS)

EFI_STATUS
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_SIMPLE_POINTER_PROTOCOL *MouseProtocol;

    InitializeLib(ImageHandle, SystemTable);

    Status = uefi_call_wrapper(
        SystemTable->BootServices->LocateProtocol,
        3,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        (void **)&Gop
    );

    if (EFI_ERROR(Status)) {
        Print(L"No se pudo localizar GOP: %r\n", Status);
        return Status;
    }

    Status = uefi_call_wrapper(
        SystemTable->BootServices->LocateProtocol,
        3,
        &SimplePointerProtocol,
        NULL,
        (VOID **)&MouseProtocol
    );

    if (EFI_ERROR(Status))
    {
        Print(L"No se pudo localizar el puntero: %r\n", Status);
        return Status;
    }

    uefi_call_wrapper(
        MouseProtocol->Reset,
        2,
        MouseProtocol,
        FALSE
    );

    if (Gop->Mode->Info->PixelFormat == PixelBltOnly)
    {
        Print(L"Este GOP no soporta acceso directo al framebuffer\n");
        return EFI_ABORTED;
    }

    WAVE_CANVAS_MANAGER *canvas = CreateCanvas(Gop, MouseProtocol);


    while (1) {
        uefi_call_wrapper(
            SystemTable->BootServices->Stall,
            1,
            FRAME_TIME_US
        );
        
        CanvasStep(canvas);
        RenderCanvas(canvas);
    }

    return EFI_SUCCESS;
}