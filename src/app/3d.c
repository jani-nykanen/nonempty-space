#include "3d.h"

#include <stdio.h>


Renderer3D create_renderer_3D(TriangleBuffer* buffer) {

    Renderer3D r3d;

    r3d.buffer = buffer;

    r3d.lightDir = vec3(0, 0, 1.0f);
    r3d.lightMag = 1.0f;
    r3d.lightingEnabled = false;

    return r3d;
}


void r3d_draw_triangle(Renderer3D* r3d,
    Transformations* transf,
    Bitmap* texture, u8 color, i32 tint,
    Vector4 A, Vector4 B, Vector4 C, 
    Vector4 tA, Vector4 tB, Vector4 tC,
    Vector4 normal) {

    Triangle t;

    A = transf_apply_to_vector(transf, A);
    B = transf_apply_to_vector(transf, B);
    C = transf_apply_to_vector(transf, C);

    if (!create_triangle_3D(texture, color, tint, A, B, C, tA, tB, tC, &t)) {

        return;
    }

    if (!tribuf_push_triangle(r3d->buffer, t)) {

        printf("WARNING: Triangle buffer overflow!\n");
        return;
    }
}


void r3d_draw_mesh(Renderer3D* r3d, Transformations* transf, Mesh* mesh, Bitmap* texture, u8 color) {

    u32 i, j;

    Vector4 A, B, C;
    Vector4 tA, tB, tC;

    for (i = 0; i < mesh->indexCount; i += 3) {

        j = mesh->indices[i];
        A = vec3(mesh->vertices[j*3], mesh->vertices[j*3 + 1], mesh->vertices[j*3 + 2]);
        tA = vec2(mesh->uvs[j*2], mesh->uvs[j*2 + 1]);

        j = mesh->indices[i+1];
        B = vec3(mesh->vertices[j*3], mesh->vertices[j*3 + 1], mesh->vertices[j*3 + 2]);
        tB = vec2(mesh->uvs[j*2], mesh->uvs[j*2 + 1]);

        j = mesh->indices[i+2];
        C = vec3(mesh->vertices[j*3], mesh->vertices[j*3 + 1], mesh->vertices[j*3 + 2]);
        tC = vec2(mesh->uvs[j*2], mesh->uvs[j*2 + 1]);
        
        // TODO: Get normal

        r3d_draw_triangle(r3d, transf, 
            texture, color, 0,
            A, B, C, tA, tB, tC, 
            vec4_zero());
    }
}
