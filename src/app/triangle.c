#include "triangle.h"
#include "mathext.h"

#include <stdio.h>
#include <math.h>


typedef i32 PointTriplet[6];


static void order_points(
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
    i32* dx1, i32* dy1, i32* dx2, i32* dy2, i32* dx3, i32* dy3) {

    // We could use qsort, but since we have only three points
    // to order, we can do this manually. No need for for loop,
    // either! Possibly a small performance boost...

    // TODO: Check if this really works...
    // TODO 2: Recursion for the other to cases?

    if (y1 <= y2 && y1 <= y3) {

        *dx1 = x1;
        *dy1 = y1;

        if (y2 <= y3) {

            *dx2 = x2;
            *dy2 = y2;
            *dx3 = x3;
            *dy3 = y3;
        }
        else {

            *dx2 = x3;
            *dy2 = y3;
            *dx3 = x2;
            *dy3 = y2;
        }
        return;
    }

    if (y2 <= y1 && y2 <= y3) {

        *dx1 = x2;
        *dy1 = y2;

        if (y1 <= y3) {

            *dx2 = x1;
            *dy2 = y1;
            *dx3 = x3;
            *dy3 = y3;
        }
        else {

            *dx2 = x3;
            *dy2 = y3;
            *dx3 = x1;
            *dy3 = y1;
        }
        return;
    }

    if (y3 <= y1 && y3 <= y2) {

        *dx1 = x3;
        *dy1 = y3;

        if (y2 <= y1) {

            *dx2 = x2;
            *dy2 = y2;
            *dx3 = x1;
            *dy3 = y1;
        }
        else {

            *dx2 = x1;
            *dy2 = y1;
            *dx3 = x2;
            *dy3 = y2;
        }
        return;
    }
}


static void compute_uv_transform(TriangleRasterizer* tri,
    f32 fx1, f32 fy1, f32 fx2, f32 fy2, f32 fx3, f32 fy3) {

    AffineMatrix3 invSpace;

    f32 w = (f32) tri->canvas->width;
    f32 h = (f32) tri->canvas->height;

    fx1 /= w; fy1 /= h;
    fx2 /= w; fy2 /= h;
    fx3 /= w; fy3 /= h;

    invSpace = affmat_compute_inverse(
            create_affine_matrix(
                fx2 - fx1, fx3 - fx1,
                fy2 - fy1, fy3 - fy1,
                fx1, fy1)
        );

    // TODO: Check order
    tri->uvTransform = affmat_multiply(invSpace, tri->uvMatrix);
}


static bool is_in_visible_area(Rectangle area, 
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3) {

    i32 minx = min_i32(x1, min_i32(x2, x3));
    i32 maxx = max_i32(x1, max_i32(x2, x3));
    i32 miny = min_i32(y1, min_i32(y2, y3));
    i32 maxy = max_i32(y1, max_i32(y2, y3));

    return minx < area.x + area.w &&
           maxx >= area.x &&
           miny < area.y + area.h &&
           maxy >= area.y;
}


static void draw_triangle_half(TriangleRasterizer* tri, Bitmap* bmp,
    i32 startx, i32 endx, i32 starty, i32 endy, f32 k1, f32 k2,
    u8 color) {

    // TODO: Implement clipping with the clipping area

    Canvas* canvas = tri->canvas;

    i32 dirx, diry;
    i32 x, y;

    f32 fstartx = (f32) startx;
    f32 fendx = (f32) endx;

    f32 w = (f32) canvas->width;
    f32 h = (f32) canvas->height;

    f32 ftx, fty;
    i32 tx, ty;

    if (starty == endy) return;

    // TEMP
    if (bmp == NULL) return;

    dirx = startx < endx ? 1 : -1;
    diry = starty < endy ? 1 : -1; 

    // TODO: OPTIMIZE!
    for (y = starty; y != endy + diry; 
        y += diry, 
        fendx += k1 * diry,
        fstartx += k2 * diry) {

        if ((diry > 0 && y >= canvas->height) ||
            (diry < 0 && y < 0)) {

            break;
        }
        if (y < 0 || y >= canvas->height)
            continue;

        startx = (i32) fstartx;
        endx = (i32) fendx;

        if ((startx >= canvas->clipArea.x + canvas->clipArea.w &&
             endx >= canvas->clipArea.x + canvas->clipArea.w) ||
             (startx < canvas->clipArea.x &&
             endx < canvas->clipArea.x)) {
            
            continue;
        }

        for (x = startx; x != endx + dirx; x += dirx) {

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

            affmat_multiply_vector(tri->uvTransform, 
                ((f32) x) / w, ((f32) y) / h,
                &ftx, &fty);

            tx = neg_mod_i32((i32) (ftx * (f32) bmp->width), bmp->width);
            ty = neg_mod_i32((i32) (fty * (f32) bmp->height), bmp->height);

            // canvas->pixels[y * canvas->width + x] = color;
            canvas->pixels[y * canvas->width + x] = bmp->pixels[ty * bmp->width + tx];
        }
    }
} 


TriangleRasterizer create_triangle_rasterizer(Canvas* canvas) {

    TriangleRasterizer tri;

    tri.canvas = canvas;

    return tri;
}


void tri_set_uv_coordinates(TriangleRasterizer* tri,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3) {

    tri->uvMatrix = create_affine_matrix(
            u2 - u1, u3 - u1, 
            v2 - v1, v3 - v1,
            u1, v1);
}


void tri_draw_triangle(TriangleRasterizer* tri, 
    Bitmap* texture, u8 color,
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3) {

    f32 k1 = 0.0f;
    f32 k2 = 0.0f;
    f32 k3 = 0.0f;
    f32 fx1, fy1, fx2, fy2, fx3, fy3;
    f32 midx;

    // Outside the render area
    if (!is_in_visible_area(tri->canvas->clipArea, x1, y1, x2, y2, x3, y3))
        return;

    // All the points in the same line
    if ((y1 == y2 && y1 == y3))
        return;

    
    if (texture != NULL) {

        // TODO: Make this look better...
        compute_uv_transform(tri, (f32) x1, (f32) y1, (f32) x2, (f32) y2, (f32) x3, (f32) y3);
    }

    order_points(x1, y1, x2, y2, x3, y3,
        &x1, &y1, &x2, &y2, &x3, &y3);
    
    fx1 = (f32) x1; fy1 = (f32) y1;
    fx2 = (f32) x2; fy2 = (f32) y2;
    fx3 = (f32) x3; fy3 = (f32) y3;

    midx = x3 - ((y3 - y2) * (x3 - x1)) / (y3 - y1);

    if (y1 != y3) {

        k1 = (fx3 - fx1) / (fy3 - fy1);
    }
    if (y1 != y2) {

        k2 = (fx2 - fx1) / (fy2 - fy1);
    }
    if (y2 != y3) {

        k3 = (fx3 - fx2) / (fy3 - fy2) ;
    }
    
    // Top
    draw_triangle_half(tri, texture, x2, (i32) midx, y2, y1, k1, k2, color);
    // Bottom
    draw_triangle_half(tri, texture, x2, (i32) midx, y2, y3, k1, k3, color);

    canvas_draw_line(tri->canvas, x1, y1, x2, y2, 0);
    canvas_draw_line(tri->canvas, x2, y2, x3, y3, 0);
    canvas_draw_line(tri->canvas, x1, y1, x3, y3, 0);
}
