#ifndef NONEMPTY_SPACE_APP_FIXED_POINT_MATRIX_H
#define NONEMPTY_SPACE_APP_FIXED_POINT_MATRIX_H


#include "common/matrix.h"


typedef struct {

    i32 m[4];

} FixedPointMatrix2;


FixedPointMatrix2 mat2_to_fixed_point_mat2(Matrix2 A, i32 precision);
void fpmat2_multiply_vector(
    FixedPointMatrix2 A, i32 precision,
    i32 x, i32 y, i32* outx, i32* outy);


#endif // NONEMPTY_SPACE_APP_FIXED_POINT_MATRIX_H
