#ifndef NONEMPTY_SPACE_APP_H
#define NONEMPTY_SPACE_APP_H


#include "common/matrix.h"

#include "canvas.h"


typedef struct {

    Canvas* canvas;

    f32 u1, v1, u2, v2, u3, v3;

    Matrix2 uvMatrix;
    Matrix2 uvTransform;

    // TODO: Rename?
    f32 uvx, uvy;
    f32 uvtx, uvty;

} TriangleRasterizer;


TriangleRasterizer create_triangle_rasterizer(Canvas* canvas);

void tri_set_uv_coordinates(TriangleRasterizer* tri,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3);
void tri_draw_triangle(TriangleRasterizer* tri, 
    Bitmap* texture, u8 color,
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3);


#endif // NONEMPTY_SPACE_APP_H
