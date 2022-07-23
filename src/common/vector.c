#include "common/vector.h"

#include <math.h>


Vector4 vec2(f32 x, f32 y)  {
    
    return vec4(x, y, 0.0f, 0.0f);
}


Vector4 vec3(f32 x, f32 y, f32 z)  {

    return vec4(x, y, z, 0.0f);
}


Vector4 vec4(f32 x, f32 y, f32 z, f32 w)  {

    Vector4 v;

    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;

    return v;  
}


Vector4 vec4_zero()  {
    
    return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}


Vector4 vec4_cross(Vector4 u, Vector4 v) {

    return vec3(
        u.y * v.z - u.z * v.y,
        -(u.x * v.z - u.z * v.x),
        u.x * v.y - u.y * v.x);
}


f32 vec4_dot(Vector4 u, Vector4 v) {

    return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
}


f32 vec4_length(Vector4 v) {

    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}


Vector4 vec4_normalize(Vector4 v, bool forceUnit) {

    const f32 EPS = 0.0001f;

    f32 len = vec4_length(v);

    if (len < EPS) {

        if (forceUnit) {

            return vec4(1.0f, 0.0f, 0.0f, 0.0f);
        }
        return vec4_zero();
    }

    v.x /= len;
    v.y /= len;
    v.z /= len;
    v.w /= len;

    return v;
}


f32 vec4_distance(Vector4 a, Vector4 b) {
    
    Vector4 d = vec4(b.x - a.x, b.y - a.y, b.z - a.z, b.w - a.w);

    return vec4_length(d);
}


Vector4 vec4_direction(Vector4 a, Vector4 b) {

    Vector4 d = vec4(b.x - a.x, b.y - a.y, b.z - a.z, b.w - a.w);

    return vec4_normalize(d, false);
}


Vector4 vec4_scalar_multiply(Vector4 v, f32 scalar) {

    return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}


Vector4 vec4_add(Vector4 a, Vector4 b) {

    return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
