#include "trianglebuffer.h"

#include "common/memory.h"

#include <stdlib.h>
#include <stdio.h>


static i32 compare_triangles(const void* a, const void* b) {

    return ((Triangle*)a)->depth > ((Triangle*)b)->depth ? 1 : -1;
}


Triangle create_triangle(
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3,
    Bitmap* texture, i32 tint, f32 depth) {

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
    t.tint = tint;
    t.depth = depth;

    return t;
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

    if (buf->count == buf->maxSize) {

        // Temporary warning
        // TODO: Remove or replace with better warning handling?
        printf("WARNING: Triangle buffer overflow!\n");
        return false;
    }

    buf->triangles[buf->count ++] = triangle;

    return true;
}


void tribuf_draw(TriangleBuffer* buf, TriangleRasterizer* tri) {

    i32 i;

    if (buf->count == 0) return;

    qsort((void*) buf->triangles, buf->count, sizeof(Triangle), compare_triangles);

    for (i = 0; i < buf->count; ++ i) {

        // ...
    }

    tribuf_flush(buf);
}
