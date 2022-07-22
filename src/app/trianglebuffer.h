#ifndef NONEMPTY_SPACE_APP_TRIANGLE_BUFFER_H
#define NONEMPTY_SPACE_APP_TRIANGLE_BUFFER_H


#include "common/vector.h"

#include "bitmap.h"
#include "rasterizer.h"
#include "transformations.h"


typedef struct {

    f32 x1, y1, x2, y2, x3, y3;
    f32 u1, v1, u2, v2, u3, v3;
    Bitmap* texture;
    u8 color;
    i32 tint;
    f32 depth;

} Triangle;


typedef struct {

    u32 maxSize;   
    u32 count;
    Triangle* triangles;

} TriangleBuffer;


Triangle create_triangle(
    f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3,
    Bitmap* texture, u8 color,
    i32 tint, f32 depth);
bool create_triangle_3D(
    Bitmap* texture, u8 color,
    Transformations* transf,
    Vector4 A, Vector4 B, Vector4 C,
    Vector4 tA, Vector4 tB, Vector4 tC,
    Vector4 normal,
    Triangle* dest);


TriangleBuffer* new_triangle_buffer(u32 maxSize, Error* err);
void dispose_triangle_buffer(TriangleBuffer* buf);

void tribuf_flush(TriangleBuffer* buf);
bool tribuf_push_triangle(TriangleBuffer* buf, Triangle triangle);

void tribuf_draw(TriangleBuffer* buf, TriangleRasterizer* tri);


#endif // NONEMPTY_SPACE_APP_TRIANGLE_BUFFER_H
