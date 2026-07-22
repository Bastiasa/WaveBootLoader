#include "vector2.h"
#include <mathutils.h>


VEC2 VectorSum(VEC2 Vector1, VEC2 Vector2)
{
    return (VEC2) {
        .x = Vector1.x + Vector2.x,
        .y = Vector1.y + Vector2.y
    };
}

VEC2 VectorNegative(VEC2 Vector)
{
    return (VEC2){
        .x = -Vector.x,
        .y = -Vector.y
    };
}

VEC2 VectorLerp(VEC2 Current, VEC2 Target, float Weight)
{
    return (VEC2) {
        .x = WaveLerp(Current.x, Target.x, Weight),
        .y = WaveLerp(Current.y, Target.y, Weight)
    };
}

VEC2 VectorMultiplyScalar(VEC2 Vector, float num)
{
    return (VEC2) {
        .x = Vector.x * num,
        .y = Vector.y * num
    };
}

VEC2 VectorMultiply(VEC2 Vector1, VEC2 Vector2)
{
    /*Print(L"MULTIPLYING VECTOR (%ld, %ld) WITH (%ld, %ld)\n",
        (INT32)Vector1.x,
        (INT32)Vector1.y,
        (INT32)Vector2.x,
        (INT32)Vector2.y
    );*/

    return (VEC2) {
        .x = Vector1.x * Vector2.x,
        .y = Vector1.y * Vector2.y
    };
}

VEC2 VectorDivideScalar(VEC2 Vector, float num)
{
    return (VEC2) {
        .x = Vector.x / num,
        .y = Vector.y / num
    };
}

VEC2 VectorDivide(VEC2 Vector1, VEC2 Vector2)
{
    return (VEC2) {
        .x = Vector1.x / Vector2.x,
        .y = Vector1.y / Vector2.y
    };
}
