#ifndef NONEMPTY_SPACE_APP_TRIANGLE_BUFFER_H
#define NONEMPTY_SPACE_APP_TRIANGLE_BUFFER_H


#include "common/vector.h"


#include "bitmap.h"
#include "rasterizer.h"


typedef struct {

    i32 x1, y1, x2, y2, x3, y3;
    f32 u1, v1, u2, v2, u3, v3;
    Bitmap* texture;
    i32 tint;
    f32 depth;

} Triangle;


typedef struct {

    u32 maxSize;   
    u32 count;
    Triangle* triangles;

} TriangleBuffer;


Triangle create_triangle(
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3,
    Bitmap* texture, i32 tint, f32 depth);


TriangleBuffer* new_triangle_buffer(u32 maxSize, Error* err);
void dispose_triangle_buffer(TriangleBuffer* buf);

void tribuf_flush(TriangleBuffer* buf);
bool tribuf_push_triangle(TriangleBuffer* buf, Triangle triangle);

void tribuf_draw(TriangleBuffer* buf, TriangleRasterizer* tri);


#endif // NONEMPTY_SPACE_APP_TRIANGLE_BUFFER_H
