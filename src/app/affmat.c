#include "affmat.h"

#include <math.h>


AffineMatrix3 create_affine_matrix(
    f32 a11, f32 a12, 
    f32 a21, f32 a22, 
    f32 b1, f32 b2) {

    AffineMatrix3 M;

    M.A[0] = a11; M.A[1] = a21;
    M.A[2] = a12; M.A[3] = a22;

    M.b[0] = b1; M.b[1] = b2;

    return M;
}


AffineMatrix3 affmat_compute_inverse(AffineMatrix3 M) {

    const f32 EPS = 0.0001f;

    AffineMatrix3 out;

    f32 det = M.A[0] * M.A[3] - M.A[2] * M.A[1];
    f32 invDet;

    if (fabsf(det) < EPS) {

        return create_affine_matrix(1, 0, 0, 1, 0, 0);
    }

    invDet = 1.0f / det;

    out.A[0] = invDet * M.A[3]; out.A[1] = -invDet * M.A[1];
    out.A[2] = -invDet * M.A[2]; out.A[3] = -invDet * M.A[0];

    out.b[0] = -(out.A[0] * M.b[0] + out.A[1] * M.b[1]);
    out.b[1] = -(out.A[2] * M.b[2] + out.A[3] * M.b[3]);

    return out;
}


AffineMatrix3 affmat_multiply(AffineMatrix3 M, AffineMatrix3 N) {

    AffineMatrix3 out;

    i32 i, j, k;

    out.A[0] = 0.0f; out.A[1] = 0.0f;
    out.A[2] = 0.0f; out.A[3] = 0.0f;

    for (i = 0; i < 2; ++ i) {

        for (j = 0; j < 2; ++ j) {

            for (k = 0; k < 2; ++ k) {

                out.A[i*2 + j] += M.A[i*2 + k] * N.A[k*2 + j];
            }
        }
    }

    out.b[0] = M.b[0] + N.b[0];
    out.b[1] = M.b[1] + N.b[1];

    return out;
}


void affmat_multiply_vector(AffineMatrix3 M, f32 v1, f32 v2, f32* outx, f32* outy) {

    *outx = M.A[0] * v1 + M.A[1] * v2 + M.b[0];
    *outy = M.A[2] * v1 + M.A[3] * v2 + M.b[1];
}
