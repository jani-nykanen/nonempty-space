#include "triangle.h"

typedef i32 PointTriplet[6];


static void order_points(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, PointTriplet out) {

    // We could use qsort, but since we have only three points
    // to order, we can do this manually. No need for for loop,
    // either! Possibly a small performance boost...

    // TODO: Check if this really works...
    // TODO 2: Recursion for the other to cases?

    if (y1 <= y2 && y1 <= y3) {

        out[0] = x1;
        out[1] = y1;

        if (y2 <= y3) {

            out[2] = x2;
            out[3] = y2;
            out[4] = x3;
            out[5] = y3;
        }
        else {

            out[2] = x3;
            out[3] = y3;
            out[4] = x2;
            out[5] = y2;
        }
        return;
    }

    if (y2 <= y1 && y2 <= y3) {

        out[0] = x2;
        out[1] = y2;

        if (y1 <= y3) {

            out[2] = x1;
            out[3] = y1;
            out[4] = x3;
            out[5] = y3;
        }
        else {

            out[2] = x3;
            out[3] = y3;
            out[4] = x1;
            out[5] = y1;
        }
        return;
    }

    if (y3 <= y1 && y3 <= y2) {

        out[0] = x3;
        out[1] = y3;

        if (y2 <= y1) {

            out[2] = x2;
            out[3] = y2;
            out[4] = x1;
            out[5] = y1;
        }
        else {

            out[2] = x1;
            out[3] = y1;
            out[4] = x2;
            out[5] = y2;
        }
        return;
    }
}


static void draw_triangle_half(Canvas* canvas, Bitmap* bmp,
    i32 minx, i32 maxx, i32 starty, i32 endy, f32 k1, f32 k2,
    u8 color) {

    if (starty == endy) return;
} 


TriangleRasterizer create_triangle_rasterizer(Canvas* canvas) {

    TriangleRasterizer tri;

    tri.canvas = canvas;

    return tri;
}


void tri_set_uv_coordinates(TriangleRasterizer* tri,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3) {

    // ...
}


void tri_draw_triangle(TriangleRasterizer* tri, 
    Bitmap* texture, u8 color,
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3) {

    PointTriplet p;

    order_points(x1, y2, x2, y2, x3, y3, p);

    // TODO: Implement
}
