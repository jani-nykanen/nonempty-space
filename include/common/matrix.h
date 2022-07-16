#ifndef NONEMPTY_SPACE_COMMON_MATRIX_H
#define NONEMPTY_SPACE_COMMON_MATRIX_H

#include "types.h"
#include "vector.h"


typedef struct {

    f32 m[16];

} Matrix4;


Matrix4 mat4_zeros();
Matrix4 mat4_identity();

Matrix4 mat4_translate(f32 x, f32 y, f32 z);
Matrix4 mat4_scale(f32 x, f32 y, f32 z);
Matrix4 mat4_rotate(f32 angle, f32 x, f32 y, f32 z);

Matrix4 mat4_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
Matrix4 mat4_ortho2d(f32 left, f32 right, f32 bottom, f32 top);
Matrix4 mat4_perspective(f32 fovY, f32 aspectRatio, f32 near, f32 far);

Matrix4 mat4_look_at(Vector4 eye, Vector4 target, Vector4 upDir);

Matrix4 mat4_multiply(Matrix4 left, Matrix4 right);
Matrix4 mat4_transpose(Matrix4 in);

Vector4 mat4_multiply_vector(Matrix4 A, Vector4 v);


#endif // NONEMPTY_SPACE_COMMON_MATRIX_H