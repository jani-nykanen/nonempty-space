#ifndef NONEMPTY_SPACE_APP_H
#define NONEMPTY_SPACE_APP_H


#include "canvas.h"

#include "affmat.h"


typedef struct {

    Canvas* canvas;

    AffineMatrix3 uvMatrix;
    AffineMatrix3 uvTransform;

} TriangleRasterizer;


TriangleRasterizer create_triangle_rasterizer(Canvas* canvas);

void tri_set_uv_coordinates(TriangleRasterizer* tri,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3);
void tri_draw_triangle(TriangleRasterizer* tri, 
    Bitmap* texture, u8 color,
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3);


#endif // NONEMPTY_SPACE_APP_H
