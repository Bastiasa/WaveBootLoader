
#ifndef STB_DS_IMPLEMENTATION

#include "efi_alloc.h"

#define memmove EfiMemMove

#define STBDS_REALLOC(c, p, s) EfiReallocWrapper(p, s)
#define STBDS_FREE(c, p) EfiFreeWrapper(p)

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#endif