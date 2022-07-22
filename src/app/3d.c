#include "3d.h"

#include <stdio.h>


Renderer3D create_renderer_3D(TriangleBuffer* buffer, TriangleRasterizer* rasterizer) {

    Renderer3D r3d;

    r3d.buffer = buffer;
    r3d.rasterizer = rasterizer;

    r3d.lightDir = vec3(0, 0, 1.0f);
    r3d.lightMag = 1.0f;
    r3d.lightingEnabled = false;

    return r3d;
}


void r3d_draw_triangle(
    TriangleBuffer* buffer, Transformations* transf,
    Bitmap* texture, u8 color,
    Vector4 A, Vector4 B, Vector4 C, 
    Vector4 tA, Vector4 tB, Vector4 tC,
    Vector4 normal) {

    Triangle t;

    A = transf_apply_to_vector(transf, A);
    B = transf_apply_to_vector(transf, B);
    C = transf_apply_to_vector(transf, C);

    if (!create_triangle_3D(texture, color, 0, A, B, C, tA, tB, tC, &t)) {

        return;
    }

    if (!tribuf_push_triangle(buffer, t)) {

        printf("WARNING: Triangle buffer overflow!\n");
        return;
    }
}
