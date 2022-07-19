#include "triangle.h"

#include <stdio.h>
#include <math.h>


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
    i32 startx, i32 endx, i32 starty, i32 endy, f32 k1, f32 k2,
    u8 color) {

    i32 dirx, diry;
    i32 x, y;

    f32 fstartx = (f32) startx;
    f32 fendx = (f32) endx;

    if (starty == endy) return;

    dirx = startx < endx ? 1 : -1;
    diry = starty < endy ? 1 : -1; 

    for (y = starty; y != endy + diry; y += diry) {

        if ((diry > 0 && y >= canvas->height) ||
            (diry < 0 && y < 0)) {

            break;
        }
        if (y < 0 || y >= canvas->height)
            continue;

        for (x = (i32) fstartx; x != ((i32) fendx) + dirx; x += dirx) {

            if ((dirx > 0 && x >= canvas->width) ||
                (dirx < 0 && x < 0)) {

                break;
            }
            if (x < 0) {

                x = 0;
            }
            else if(x >= canvas->width) {

                x = canvas->width - 1;
            }

            canvas->pixels[y * canvas->width + x] = color;
        }

        fendx += k1;
        fstartx -= k2;
    }
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
    i32 midx;
    f32 k1 = 0.0f;
    f32 k2 = 0.0f;

    // To avoid a warning
    p[0] = 0; p[1] = 0;
    p[2] = 0; p[3] = 0;
    p[4] = 0; p[5] = 0;

    order_points(x1, y1, x2, y2, x3, y3, p);

    // No triangle to be drawn
    if (p[1] == p[3] && p[1] == p[5])
        return;


    // Typecast hell
    midx = p[4] - (i32) floorf(((f32) (p[5] - p[3])) * ((f32) (p[4] - p[0])) / ((f32) (p[5] - p[1])));

    if (p[2] != p[0]) {

        k1 = ((f32) (p[3] - p[1])) / ((f32) (p[2] - (f32) p[0]));
    }
    if (p[4] != p[0]) {

        k2 = ((f32) (p[5] - p[1])) / ((f32) (p[4] - p[0]));
    }

    // draw_triangle_half(tri->canvas, texture, p[2], midx, p[3], p[1], -k1, -k2, color);
    draw_triangle_half(tri->canvas, texture, p[2], midx, p[3], p[5], k1, k2, color);
}
