#include "rasterizer.h"
#include "mathext.h"

#include <stdio.h>
#include <math.h>


#define FIXED_POINT_PRECISION 12


// Some macros for shorter code in triangle pixel functions.
// We use macros to minimize function calls in per-pixel operations
#define TINT(arr, x, y, color) rasterizer->lookup->arr[rasterizer->lookup->dither[hue][x % 2 == y % 2]][color]

#define TEX_COORD(x, y, tx, ty) fpmat2_multiply_vector( \
                rasterizer->uvTransform, \
                FIXED_POINT_PRECISION, \
                x - rasterizer->uvtx, \
                y - rasterizer->uvty,\
                &tx, &ty);\
    tx = neg_mod_i32(tx + rasterizer->uvx, bmp->width);\
    ty = neg_mod_i32(ty + rasterizer->uvy, bmp->height);


typedef void (PixelFunction)(TriangleRasterizer* canvas, 
    Bitmap* bmp, u8 color, i32 hue, i32 x, i32 y);


typedef i32 PointTriplet[6];


static void order_points(
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
    i32* dx1, i32* dy1, i32* dx2, i32* dy2, i32* dx3, i32* dy3) {

    // We could use qsort, but since we have only three points
    // to order, we can do this manually. No need for for loop,
    // either! Possibly a small performance boost...

    // TODO: Maybe qsort after all?

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


static void compute_uv_transform(
    TriangleRasterizer* tri, Bitmap* bmp,
    i32 fx1, i32 fy1, i32 fx2, i32 fy2, i32 fx3, i32 fy3) {

    Matrix2 uvMatrix;
    Matrix2 invSpace;

    f32 u1 = tri->u1 * bmp->width;
    f32 v1 = tri->v1 * bmp->height;
    f32 u2 = tri->u2 * bmp->width;
    f32 v2 = tri->v2 * bmp->height;
    f32 u3 = tri->u3 * bmp->width;
    f32 v3 = tri->v3 * bmp->height;

    uvMatrix = mat2(
        u2 - u1, u3 - u1, 
        v2 - v1, v3 - v1);

    tri->uvx = (i32) u1;
    tri->uvy = (i32) v1;

    invSpace = mat2_inverse(
        mat2((f32) (fx2 - fx1), (f32) (fx3 - fx1), 
             (f32) (fy2 - fy1), (f32) (fy3 - fy1))
        );

    tri->uvtx = fx1;
    tri->uvty = fy1;

    tri->uvTransform = mat2_to_fixed_point_mat2(
        mat2_multiply(uvMatrix, invSpace), FIXED_POINT_PRECISION);
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


static void ppfunc_textured(TriangleRasterizer* rasterizer, 
    Bitmap* bmp, u8 color, i32 hue, i32 x, i32 y) {

    i32 tx, ty;
    TEX_COORD(x, y, tx, ty)

    rasterizer->canvas->pixels[y * rasterizer->canvas->width + x] = bmp->pixels[ty * bmp->width + tx];
}


static void ppfunc_textured_tint_black(TriangleRasterizer* rasterizer, 
    Bitmap* bmp, u8 color, i32 hue, i32 x, i32 y) {

    i32 tx, ty;
    TEX_COORD(x, y, tx, ty)

    //  TODO: Replace x % 2 == y % 2 with bitwise operators, if possible!
    rasterizer->canvas->pixels[y * rasterizer->canvas->width + x] = 
        TINT(tintBlack, x, y, bmp->pixels[ty * bmp->width + tx]);
}


static void ppfunc_textured_tint_white(TriangleRasterizer* rasterizer, 
    Bitmap* bmp, u8 color, i32 hue, i32 x, i32 y) {

    i32 tx, ty;
    TEX_COORD(x, y, tx, ty)

    rasterizer->canvas->pixels[y * rasterizer->canvas->width + x] = 
        TINT(tintWhite, x, y, bmp->pixels[ty * bmp->width + tx]);
}


static void ppfunc_colored(TriangleRasterizer* rasterizer, 
    Bitmap* bmp, u8 color, i32 hue, i32 x, i32 y) {

    rasterizer->canvas->pixels[y * rasterizer->canvas->width + x] = color;
}


static void ppfunc_colored_tint_black(TriangleRasterizer* rasterizer, 
    Bitmap* bmp, u8 color, i32 hue, i32 x, i32 y) {

    rasterizer->canvas->pixels[y * rasterizer->canvas->width + x] = TINT(tintBlack, x, y, color);
}


static void ppfunc_colored_tint_white(TriangleRasterizer* rasterizer, 
    Bitmap* bmp, u8 color, i32 hue, i32 x, i32 y) {

    rasterizer->canvas->pixels[y * rasterizer->canvas->width + x] = TINT(tintWhite, x, y, color);
}


static void draw_triangle_half(TriangleRasterizer* rasterizer, Bitmap* bmp,
    i32 startx, i32 endx, i32 starty, i32 endy, i32 k1, i32 k2,
    u8 color, i32 hue, PixelFunction ppfunc) {

    // TODO: Implement clipping with the clipping area

    Canvas* canvas = rasterizer->canvas;

    i32 diry;
    i32 x, y;

    i32 fstartx = startx << FIXED_POINT_PRECISION;
    i32 fendx = endx << FIXED_POINT_PRECISION;

    i32 minx, maxx;

    if (starty == endy) return;

    diry = starty < endy ? 1 : -1; 

    for (y = starty; y != endy + diry; 
        y += diry, 
        fendx += k1 * diry,
        fstartx += k2 * diry) {

        if ((diry > 0 && y >= canvas->height) || (diry < 0 && y < 0))
            break;
        
        if (y < 0 || y >= canvas->height)
            continue;

        startx = fstartx >> FIXED_POINT_PRECISION;
        endx = fendx >> FIXED_POINT_PRECISION;

        minx = max_i32(canvas->clipArea.x, min_i32(startx, endx));
        maxx = min_i32(canvas->clipArea.x + canvas->clipArea.w-1, max_i32(startx, endx));

        for (x = minx; x <= maxx; ++ x) {

            ppfunc(rasterizer, bmp, color, hue, x, y);
        }
    }
} 


TriangleRasterizer create_triangle_rasterizer(Canvas* canvas, LookUpTables* lookup) {

    TriangleRasterizer tri;

    tri.canvas = canvas;
    tri.lookup = lookup;

    return tri;
}


void tri_set_uv_coordinates(TriangleRasterizer* tri,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3) {

    tri->u1 = u1;
    tri->v1 = v1;
    tri->u2 = u2;
    tri->v2 = v2;
    tri->u3 = u3;
    tri->v3 = v3;
}


void tri_draw_triangle(TriangleRasterizer* tri, 
    Bitmap* texture, u8 color, i32 hue,
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3) {

    i32 k1 = 0;
    i32 k2 = 0;
    i32 k3 = 0;

    i32 midx;

    PixelFunction* ppfunc;
    i32 originalHue = hue;

    // Outside the render area
    if (!is_in_visible_area(tri->canvas->clipArea, x1, y1, x2, y2, x3, y3))
        return;

    // All the points in the same line
    if ((y1 == y2 && y1 == y3))
        return; 

    if (originalHue < 0) {

        hue *= -1;
    }
    hue = min_i32(hue, (TINT_MAX-1) * 2);

    if (texture != NULL) {

        compute_uv_transform(tri, texture, x1, y1, x2, y2, x3, y3);
        ppfunc = ppfunc_textured;

        if (originalHue < 0) 
            ppfunc = ppfunc_textured_tint_black;
        else if (originalHue > 0) 
            ppfunc = ppfunc_textured_tint_white;
        
    }
    else {

        ppfunc = ppfunc_colored;

        if (originalHue < 0) 
            ppfunc = ppfunc_colored_tint_black;
        else if (originalHue > 0) 
            ppfunc = ppfunc_colored_tint_white;
    }

    order_points(x1, y1, x2, y2, x3, y3,
        &x1, &y1, &x2, &y2, &x3, &y3);
    
    midx = ((y3 - y2) * (x3 - x1)) << FIXED_POINT_PRECISION;
    midx /= (y3 - y1);
    midx = x3 - (midx >> FIXED_POINT_PRECISION);

    if (y1 != y3) {

        k1 = (x3 - x1) << FIXED_POINT_PRECISION;
        k1 /= (y3 - y1);
    }
    if (y1 != y2) {

        k2 = (x2 - x1) << FIXED_POINT_PRECISION;
        k2 /= (y2 - y1);
    }
    if (y2 != y3) {

        k3 = (x3 - x2) << FIXED_POINT_PRECISION;
        k3 /= (y3 - y2);
    }
    
    // Top
    draw_triangle_half(tri, texture, x2, midx, y2, y1, k1, k2, color, hue, ppfunc);
    // Bottom
    draw_triangle_half(tri, texture, x2, midx, y2, y3, k1, k3, color, hue, ppfunc);
}
