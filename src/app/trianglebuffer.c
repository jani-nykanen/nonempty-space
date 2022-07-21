#include "trianglebuffer.h"


Triangle create_triangle(
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3,
    Bitmap* texture, i32 tint, f32 depth) {

    Triangle t;

    t.x1 = x1;
    t.y1 = y1;
    t.x2 = x2;
    t.y2 = y2;
    t.x3 = x3;
    t.y3 = y3;

    t.u1 = u1;
    t.v1 = v1;
    t.u2 = u2;
    t.v2 = v2;
    t.u3 = u3;
    t.v3 = v3;

    t.texture = texture;
    t.tint = tint;
    t.depth = depth;

    return t;
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
