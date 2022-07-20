#ifndef NONEMPTY_SPACE_APP_AFFINE_MATRIX_H
#define NONEMPTY_SPACE_APP_AFFINE_MATRIX_H


#include "common/types.h"


typedef struct {
    
    f32 A[4];
    f32 b[2];
} AffineMatrix3;


AffineMatrix3 create_affine_matrix(
    f32 a11, f32 a21, 
    f32 a12, f32 a22, 
    f32 b1, f32 b2);

AffineMatrix3 affmat_compute_inverse(AffineMatrix3 M);
AffineMatrix3 affmat_multiply(AffineMatrix3 M, AffineMatrix3 N);

void affmat_multiply_vector(AffineMatrix3 M, f32 v1, f32 v2, f32* outx, f32* outy);


#endif // NONEMPTY_SPACE_APP_AFFINE_MATRIX_H
