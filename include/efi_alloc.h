#include <efi.h>
#include <efilib.h>
#include <stddef.h>

typedef struct {
    UINTN size;
} EfiAllocHeader;

static inline void *EfiReallocWrapper(void *ptr, UINTN newSize) {
    EfiAllocHeader *oldHeader = NULL;
    UINTN oldSize = 0;

    if (ptr) {
        oldHeader = (EfiAllocHeader *)((UINT8 *)ptr - sizeof(EfiAllocHeader));
        oldSize = oldHeader->size;
    }

    UINTN totalNewSize = newSize + sizeof(EfiAllocHeader);
    void *newRaw = AllocatePool(totalNewSize);
    if (!newRaw) return NULL;

    EfiAllocHeader *newHeader = (EfiAllocHeader *)newRaw;
    newHeader->size = newSize;

    void *newPtr = (UINT8 *)newRaw + sizeof(EfiAllocHeader);

    if (ptr) {
        UINTN copySize = (oldSize < newSize) ? oldSize : newSize;
        CopyMem(newPtr, ptr, copySize);
        FreePool(oldHeader);
    }

    return newPtr;
}

static inline void EfiFreeWrapper(void *ptr) {
    if (ptr) {
        EfiAllocHeader *header = (EfiAllocHeader *)((UINT8 *)ptr - sizeof(EfiAllocHeader));
        FreePool(header);
    }
}

static inline void *EfiMemMove(void *dest, const void *src, size_t len) {
    CopyMem(dest, (void *)src, (UINTN)len);
    return dest;
}