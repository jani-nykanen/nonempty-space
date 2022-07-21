#include "trianglebuffer.h"


Triangle create_triangle(
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3,
    Bitmap* texture, i32 tint, f32 depth) {

}


TriangleBuffer* new_triangle_buffer(u32 maxSize, Error* err) {

}


void dispose_triangle_buffer(TriangleBuffer* buf) {

}


void tribuf_flush(TriangleBuffer* buf) {

}


bool tribuf_push_triangle(TriangleBuffer* buf, Triangle triangle) {

}


void tribuf_draw(TriangleBuffer* buf, TriangleRasterizer* tri) {

}
