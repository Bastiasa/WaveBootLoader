#include <efi.h>
#include <efilib.h>
#include <efiapi.h>

#include "graphics/canvas.h"
#include "graphics/elements/rectangle.h"
#include "graphics/vector2.h"
#include "app/init.h"

#define TARGET_FPS 60
#define FRAME_TIME_US (1000000 / TARGET_FPS)



EFI_STATUS
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);

    WAVE_CANVAS_MANAGER *canvas = CreateCanvas(SystemTable, ImageHandle);

    EFI_STATUS initResult = InitCanvas(canvas);

    if (EFI_ERROR(initResult))
    {
        Print("La inicialización del Canvas ha fallado.\n");
        return;
    }
    

    InitUI(canvas);

    while (1) {
        /*uefi_call_wrapper(
            SystemTable->BootServices->Stall,
            1,
            FRAME_TIME_US
        );*/
        
        OnBeforeStep();
        CanvasStep(canvas);
        OnAfterStep();

        OnBeforeRender();
        RenderCanvas(canvas);
        OnAfterRender();

        OnUpdate();
    }

    return EFI_SUCCESS;
}