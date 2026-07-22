#ifndef WAVE_IMAGE_METADATA_IMPL
#define WAVE_IMAGE_METADATA_IMPL

#include <efi.h>
#include <graphics/vector2.h>



typedef struct
{
    UINT32 Width;
    UINT32 Height;
    UINT32 *Pixels;
} WAVE_IMAGE_METADATA;


static inline VEC2 MetaToVector2(WAVE_IMAGE_METADATA *meta) {
    return WV2(meta->Width, meta->Height);
}



#endif