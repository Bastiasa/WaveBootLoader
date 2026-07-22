#include "boot.h"

#define WINDOWS_BOOT_PATH L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi"
#define UBUNTU_BOOT_PATH L"\\EFI\\ubuntu\\shimx64.efi"

void Boot(CHAR16 *EfiFilepath, EFI_HANDLE ParentImageHandle) {
    EFI_STATUS Status;
    EFI_HANDLE ChildImageHandle;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;

    Status = uefi_call_wrapper(
        BS->HandleProtocol,
        3,
        ParentImageHandle,
        &LoadedImageProtocol,
        (void **)&LoadedImage
    );

    if (EFI_ERROR(Status))
        return;

    EFI_DEVICE_PATH *Path = FileDevicePath(
        LoadedImage->DeviceHandle,
        EfiFilepath
    );

    Status = uefi_call_wrapper(
        BS->LoadImage,
        6,
        FALSE,
        ParentImageHandle,
        Path,
        NULL,
        0,
        &ChildImageHandle
    );

    if (EFI_ERROR(Status))
        return;

    Status = uefi_call_wrapper(
        BS->StartImage,
        3,
        ChildImageHandle,
        NULL,
        NULL
    );
}

EFI_STATUS SetDirectBootFlag(EFI_HANDLE ImageHandle, BOOLEAN Enable)
{
    EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    EFI_FILE_PROTOCOL *Root;
    EFI_FILE_PROTOCOL *FlagFile;

    Status = uefi_call_wrapper(
        BS->HandleProtocol,
        3,
        ImageHandle,
        &LoadedImageProtocol,
        (void **)&LoadedImage
    );
    if (EFI_ERROR(Status)) return Status;

    Status = uefi_call_wrapper(
        BS->HandleProtocol,
        3,
        LoadedImage->DeviceHandle,
        &FileSystemProtocol,
        (void **)&FileSystem
    );
    if (EFI_ERROR(Status)) return Status;

    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
    if (EFI_ERROR(Status)) return Status;

    if (Enable) {
        // Crear/tocar el archivo -> GRUB arranca directo, sin menú
        Status = uefi_call_wrapper(
            Root->Open,
            5,
            Root,
            &FlagFile,
            L"\\waveboot_direct.flag",
            EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
            0
        );
        if (!EFI_ERROR(Status)) {
            uefi_call_wrapper(FlagFile->Close, 1, FlagFile);
        }
    } else {
        // Borrar el archivo -> GRUB muestra el menú normal
        Status = uefi_call_wrapper(
            Root->Open,
            5,
            Root,
            &FlagFile,
            L"\\waveboot_direct.flag",
            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
            0
        );
        if (!EFI_ERROR(Status)) {
            uefi_call_wrapper(FlagFile->Delete, 1, FlagFile);
            // Delete() ya cierra el handle internamente
        }
        // EFI_NOT_FOUND aquí es normal si el archivo no existía, no es error real
    }

    uefi_call_wrapper(Root->Close, 1, Root);
    return EFI_SUCCESS;
}


void BootWindows(EFI_HANDLE ParentImageHandle)
{
   Boot(WINDOWS_BOOT_PATH, ParentImageHandle);
}

void BootUbuntu(EFI_HANDLE ParentImageHandle) {
    SetDirectBootFlag(ParentImageHandle, TRUE);
    Boot(UBUNTU_BOOT_PATH, ParentImageHandle);
}

void BootGrub(EFI_HANDLE ParentImageHandle) {
    SetDirectBootFlag(ParentImageHandle, FALSE);
    Boot(UBUNTU_BOOT_PATH, ParentImageHandle);
}