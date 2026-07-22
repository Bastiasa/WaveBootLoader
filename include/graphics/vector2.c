#include "vector2.h"

VEC2 VectorSum(VEC2 Vector1, VEC2 Vector2)
{
    return (VEC2) {
        .x = Vector1.x + Vector2.x,
        .y = Vector1.y + Vector2.y
    };
}

VEC2 VectorNegative(VEC2 vector)
{
    return (VEC2){
        .x = -vector.x,
        .y = -vector.y
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
    return (VEC2) {
        .x = Vector1.x * Vector2.x,
        .x = Vector1.y * Vector2.y
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
        .y = Vector2.y / Vector2.y
    };
}
