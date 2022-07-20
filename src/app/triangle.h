#ifndef NONEMPTY_SPACE_APP_H
#define NONEMPTY_SPACE_APP_H


#include "canvas.h"


// Or maybe it is 2x3 matrix, I never remember
// the order in the notation
typedef f32 Matrix3x2[6];


typedef struct {

    Canvas* canvas;

    Matrix3x2 uvBaseMatrix;
    Matrix3x2 uvInvMatrix;

} TriangleRasterizer;


TriangleRasterizer create_triangle_rasterizer(Canvas* canvas);

void tri_set_uv_coordinates(TriangleRasterizer* tri,
    f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3);
void tri_draw_triangle(TriangleRasterizer* tri, 
    Bitmap* texture, u8 color,
    i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3);


#endif // NONEMPTY_SPACE_APP_H
