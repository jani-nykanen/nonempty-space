#include "fpointmatrix.h"


FixedPointMatrix2 mat2_to_fixed_point_mat2(Matrix2 A, i32 precision) {

    FixedPointMatrix2 out;

    f32 unit = (f32) (1 << precision);
    i32 i;

    for (i = 0; i < 4; ++ i) {

        out.m[i] = (i32) (A.m[i] * unit);
    }

    return out;
}


void fpmat2_multiply_vector(
    FixedPointMatrix2 A, i32 precision, 
    i32 x, i32 y, i32* outx, i32* outy) {

    *outx =	A.m[0] * x + A.m[1] * y;
	*outy =	A.m[2] * x + A.m[3] * y;

    *outx >>= precision;
    *outy >>= precision;
}
