#include "trianglebuffer.h"

#include "common/memory.h"

#include <stdlib.h>
#include <stdio.h>


static i32 compare_triangles(const void* a, const void* b) {

    return ((Triangle*)a)->depth > ((Triangle*)b)->depth ? 1 : -1;
}


static Vector4 project(Vector4 P) {

    Vector4 out;

    out.x = P.x / P.z;
    out.y = P.y / P.z;

    out.x += 1.0f;
    out.y += 1.0f;

    out.x /= 2.0f;
    out.y /= 2.0f;

    return out;
}


Triangle create_triangle(
    f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3,
    Bitmap* texture, u8 color,
    i32 tint, f32 depth) {

    // Yes, I know {Triangle} {x1, y1, ..., } exists,
    // but some compilers do not support it.

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
    t.color = color;
    t.tint = tint;
    t.depth = depth;

    return t;
}


bool create_triangle_3D(
    Bitmap* texture, u8 color, u8 tint,
    Vector4 A, Vector4 B, Vector4 C,
    Vector4 tA, Vector4 tB, Vector4 tC,
    Triangle* dest) {

    const f32 NEAR = 0.025f;

    Vector4 pA, pB, pC;
    f32 depth;

    if (A.z < NEAR || B.z < NEAR || C.z < NEAR)
        return false;

    depth = (A.z + B.z + C.z) / 3.0f;

    pA = project(A);
    pB = project(B);
    pC = project(C);

    *dest = create_triangle(
        pA.x, pA.y, pB.x, pB.y, pC.x, pC.y,
        tA.x, tA.y, tB.x, tB.y, tC.x, tC.y,
        texture, color, tint, depth);

    return true;
}


TriangleBuffer* new_triangle_buffer(u32 maxSize, Error* err) {
    
    TriangleBuffer* buffer = (TriangleBuffer*) calloc(1, sizeof(TriangleBuffer));
    if (buffer == NULL) {

        *err = memory_error();
        return NULL;
    }

    buffer->triangles = (Triangle*) calloc(maxSize, sizeof(Triangle));
    if (buffer->triangles == NULL) {

        *err = memory_error();
        free(buffer);

        return NULL;
    }

    buffer->count = 0;
    buffer->maxSize = maxSize;

    return buffer;
}


void dispose_triangle_buffer(TriangleBuffer* buf) {

    if (buf == NULL)
        return;

    m_free(buf->triangles);
    m_free(buf);
}


void tribuf_flush(TriangleBuffer* buf) {

    buf->count = 0;
}


bool tribuf_push_triangle(TriangleBuffer* buf, Triangle triangle) {

    if (buf->count == buf->maxSize) 
        return false;
    
    buf->triangles[buf->count ++] = triangle;
    return true;
}


void tribuf_draw(TriangleBuffer* buf, TriangleRasterizer* tri) {

    i32 i;
    Canvas* canvas = tri->canvas;
    Triangle t;
    i32 x1, y1, x2, y2, x3, y3;

    f32 w = (f32) canvas->width;
    f32 h = (f32) canvas->height;

    if (buf->count == 0) return;

    qsort((void*) buf->triangles, buf->count, sizeof(Triangle), compare_triangles);

    for (i = 0; i < buf->count; ++ i) {

        t = buf->triangles[i];

        tri_set_uv_coordinates(tri, t.u1, t.v1, t.u2, t.v2, t.u3, t.v3);

        x1 = (i32) (t.x1 * w); y1 = (i32) (t.y1 * h);
        x2 = (i32) (t.x2 * w); y2 = (i32) (t.y2 * h);
        x3 = (i32) (t.x3 * w); y3 = (i32) (t.y3 * h);

        tri_draw_triangle(tri, t.texture, t.color, x1, y1, x2, y2, x3, y3);
    }

    tribuf_flush(buf);
}
