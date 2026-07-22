#ifndef APP_BOOT_IMPL
#define APP_BOOT_IMPL

#include <efi.h>
#include <efilib.h>

void BootWindows(EFI_HANDLE ParentImageHandle);
void BootUbuntu(EFI_HANDLE ParentImageHandle);
void BootGrub(EFI_HANDLE ParentImageHandle);

#endif