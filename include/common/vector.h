#ifndef NONEMPTY_SPACE_COMMON_VECTOR_H
#define NONEMPTY_SPACE_COMMON_VECTOR_H


#include "types.h"


typedef struct {

    f32 x;
    f32 y;
    f32 z;
    f32 w;

} Vector4;


Vector4 vec2(f32 x, f32 y);
Vector4 vec3(f32 x, f32 y, f32 z);
Vector4 vec4(f32 x, f32 y, f32 z, f32 w);

Vector4 vec4_zero();

Vector4 vec4_cross(Vector4 u, Vector4 v);
f32 vec4_dot(Vector4 u, Vector4 v);

f32 vec4_length(Vector4 v);
Vector4 vec4_normalize(Vector4 v, bool forceUnit);
f32 vec4_distance(Vector4 a, Vector4 b);
Vector4 vec4_direction(Vector4 a, Vector4 b);

Vector4 vec4_scalar_multiply(Vector4 v, f32 scalar);
Vector4 vec4_add(Vector4 a, Vector4 b);


#endif // NONEMPTY_SPACE_COMMON_VECTOR_H
