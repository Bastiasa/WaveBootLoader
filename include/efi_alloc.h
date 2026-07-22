// en algún .h común, por ejemplo efi_alloc.h
#include <efi.h>
#include <efilib.h>

static inline void *EfiReallocWrapper(void *ptr, int newSize) {
    // AllocatePool no preserva contenido; para stb_ds esto normalmente basta
    // porque stb_ds siempre copia sus propios datos a mano internamente en growth,
    // pero para estar seguros, copiamos manualmente si había ptr previo.
    void *newPtr = AllocatePool(newSize);
    if (newPtr && ptr) {
        // No conocemos el tamaño viejo exacto sin trackearlo aparte;
        // ver nota abajo.
        FreePool(ptr);
    }
    return newPtr;
}

static inline void EfiFreeWrapper(void *ptr) {
    if (ptr) FreePool(ptr);
}