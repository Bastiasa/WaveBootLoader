#include "color.h"

UINT32 ColorRGB(UINT32 red, UINT32 green, UINT32 blue) {
    return (red << 16) | (green << 8) | blue;
}