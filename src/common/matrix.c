#include "common/matrix.h"

#include <math.h>


Matrix2 mat2(f32 a11, f32 a21, f32 a12, f32 a22) {

	Matrix2 out;

	out.m[0] = a11; out.m[1] = a21;
	out.m[2] = a12; out.m[3] = a22;

	return out;
}


Matrix2 mat2_zeros() {

	Matrix2 A;
    i32 i;
    
    for (i = 0; i < 4; ++ i) {

        A.m[i] = 0.0f;
    }

    return A; 
}


Matrix2 mat2_identity() {

	Matrix2 A;

	A.m[0] = 1.0f; A.m[1] = 0.0f;
	A.m[2] = 0.0f; A.m[3] = 1.0f;

	return A;
}


Matrix2 mat2_inverse(Matrix2 A) {

	const f32 EPS = 0.0001f;

    Matrix2 out;

    f32 det = A.m[0] * A.m[3] - A.m[2] * A.m[1];
    f32 invDet;

    if (fabsf(det) < EPS) {

        return mat2_identity();
    }

    invDet = 1.0f / det;

    out.m[0] =  invDet * A.m[3]; out.m[1] = -invDet * A.m[1];
    out.m[2] = -invDet * A.m[2]; out.m[3] =  invDet * A.m[0];

	return out;
}


Matrix2 mat2_multiply(Matrix2 left, Matrix2 right) {

	Matrix2 out = mat2_zeros();
	i32 i, j, k;

    for (i = 0; i < 2; ++ i) {

        for (j = 0; j < 2; ++ j) {

            for (k = 0; k < 2; ++ k) {

                out.m[i*2 + j] += left.m[i*2 + k] * right.m[k*2 + j];
            }
        }
    }
	return out;
}


void mat2_multiply_vector(Matrix2 A, f32 x, f32 y, f32* outx, f32* outy) {

	*outx =	A.m[0] * x + A.m[1] * y;
	*outy =	A.m[2] * x + A.m[3] * y;
}


Matrix4 mat4_zero() {

    Matrix4 A;
    i32 i;
    
    for (i = 0; i < 4*4; ++ i) {

        A.m[i] = 0.0f;
    }

    return A;   
}


Matrix4 mat4_identity() {

    Matrix4 A = mat4_zero();
    i32 i;

    for (i = 0; i < 4; ++ i) {

        A.m[i*4 + i] = 1.0f;
    }

    return A;
}


Matrix4 mat4_translate(f32 x, f32 y, f32 z) {

    Matrix4 A = mat4_identity();

    A.m[3] = x;
    A.m[7] = y;
    A.m[11] = z;

    return A;
}


Matrix4 mat4_scale(f32 x, f32 y, f32 z) {

    Matrix4 A = mat4_zero();

    A.m[0] = x;
    A.m[5] = y;
    A.m[10] = z;
    A.m[15] = 1.0f;

    return A;
}


Matrix4 mat4_rotate(f32 angle, f32 x, f32 y, f32 z) {

	f32 ca = cosf(angle*x);
	f32 sa = sinf(angle*x);

	f32 cb = cosf(angle*y);
	f32 sb = sinf(angle*y);

	f32 cc = cosf(angle*z);
	f32 sc = sinf(angle*z);
		
	Matrix4 A = mat4_identity();

	A.m[0] = cb * cc; 
	A.m[1] = -cb * sc; 
	A.m[2] = sb;

	A.m[4] = sa * sb * cc + ca * sc; 
	A.m[4 + 1] = -sa * sb * sc + ca * cc; 
	A.m[4 + 2] = -sa * cb;

	A.m[8] = -ca * sb * cc; 
	A.m[8 + 1] = ca * sb * sc + sa * cc; 
    A.m[8 + 2] = ca * cb;   
		
	return A;    
}


Matrix4 mat4_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {

    Matrix4 A = mat4_zero();

    A.m[0] = 2.0f / (right - left);
	A.m[3] = -(right + left) / (right - left);

	A.m[4 + 1] = 2.0f / (top - bottom);
	A.m[4 + 3] = -(top + bottom) / (top-bottom);

	A.m[8 + 2] = -2.0f / (far - near);
	A.m[8 + 3] = -(far + near) / (far - near);

	A.m[12 + 3] = 1.0f;

    return A;
}


Matrix4 mat4_ortho2d(f32 left, f32 right, f32 bottom, f32 top) {

    return mat4_ortho(left, right, bottom, top, -1.0f, 1.0f);
}


Matrix4 mat4_perspective(f32 fovY, f32 aspectRatio, f32 near, f32 far) {

	Matrix4 A = mat4_zero();
		
	f32 f = 1.0f / tanf( (fovY / 180.0f * M_PI) / 2.0f);

	A.m[0] = f / aspectRatio;
    A.m[5] = f;
    A.m[10] = -(far + near) / (far - near);
    A.m[11] = -2.0f * far * near / (far - near);
    A.m[14] = -1.0f;
		
	return A;
}


Matrix4 mat4_look_at(Vector4 eye, Vector4 target, Vector4 upDir) {

	Matrix4 A = mat4_identity();
		
	Vector4 forward = 
		vec4_normalize(vec3(eye.x - target.x, 
			eye.y - target.y, 
			eye.z - target.z), true);

	Vector4 left = vec4_normalize(vec4_cross(forward, upDir), true);
	Vector4 up = vec4_normalize(vec4_cross(forward, left), true);

	A.m[0] = left.x; A.m[1] = left.y; A.m[2] = left.z;
	A.m[4] = up.x; A.m[5] = up.y; A.m[6] = up.z;
	A.m[8] = forward.x; A.m[9] = forward.y; A.m[10] = forward.z;

	A.m[3] = -left.x * eye.x - left.y * eye.y - left.z * eye.z;
	A.m[7] = -up.x * eye.x - up.y * eye.y - up.z * eye.z;
	A.m[11] = -forward.x * eye.x - forward.y * eye.y - forward.z * eye.z;
		
	return A;	
}


Matrix4 mat4_multiply(Matrix4 left, Matrix4 right) {

    Matrix4 out = mat4_zero();
    i32 i, j, k;
		
	for (i = 0; i < 4; ++ i) {

		for (j = 0; j < 4; ++ j) {

			for (k = 0; k < 4; ++ k) {

				out.m[i*4 + j] += left.m[i*4 + k] * right.m[k*4 + j];
			}
		}
	}
		
	return out;
}


Matrix4 mat4_transpose(Matrix4 in) {

    Matrix4 out = mat4_zero();
    i32 i, j;
		
	for (j = 0; j < 4; ++ j) {
			
		for (i = 0; i < 4; ++ i) {
				
			out.m[i*4 + j] = in.m[j*4 + i];
		}
	}
		
	return out;
}



Vector4 mat4_multiply_vector(Matrix4 A, Vector4 v) {

    Vector4 out;

    out.x = A.m[0] * v.x + A.m[1] * v.y + A.m[2] * v.z + A.m[3];
    out.y = A.m[4] * v.x + A.m[5] * v.y + A.m[6] * v.z + A.m[7];
    out.z = A.m[8] * v.x + A.m[9] * v.y + A.m[10] * v.z + A.m[11];

    return out;
}
