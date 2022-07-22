#ifndef NONEMPTY_SPACE_APP_3D_RENDERER_H
#define NONEMPTY_SPACE_APP_3D_RENDERER_H


#include "trianglebuffer.h"
#include "transformations.h"


typedef struct {

    TriangleBuffer* buffer;
    TriangleRasterizer* rasterizer;

    Vector4 lightDir;
    f32 lightMag;
    bool lightingEnabled;

} Renderer3D;


Renderer3D create_renderer_3D(TriangleBuffer* buffer, TriangleRasterizer* rasterizer);

void r3d_draw_triangle(
    TriangleBuffer* buffer, Transformations* transf,
    Bitmap* texture, u8 color,
    Vector4 A, Vector4 B, Vector4 C, 
    Vector4 tA, Vector4 tB, Vector4 tC,
    Vector4 normal);


#endif // NONEMPTY_SPACE_APP_3D_RENDERER_H
