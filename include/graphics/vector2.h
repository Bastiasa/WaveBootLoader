#ifndef WAVE_VECTOR2
#define WAVE_VECTOR2


#include "efi.h"
#include "efilib.h"


typedef struct {
    float x;
    float y;
} VEC2;

#define VECTOR2_ONE (VEC2) {.x = 1.0f, .y = 1.0f}
#define VECTOR2_ZERO (VEC2) {.x = 0.0f, .y = 0.0f}
#define WV2(px, py) ((VEC2){ .x = (px), .y = (py) })


VEC2 VectorSum(VEC2 Vector1, VEC2 Vector2);
VEC2 VectorNegative(VEC2 Vector);

VEC2 VectorMultiplyScalar(VEC2 Vector, float num);
VEC2 VectorMultiply(VEC2 Vector1, VEC2 Vector2);

VEC2 VectorDivideScalar(VEC2 Vector, float num);
VEC2 VectorDivide(VEC2 Vector1, VEC2 Vector2);

#endif