#include "3d.h"
#include "mathext.h"

#include <stdio.h>
#include <math.h>


static i32 compute_hue(Renderer3D* r3d, Vector4 normal, f32 isDark) {

    f32 d, light, hue;

    d = clamp_f32((1.0f + vec4_dot(normal, r3d->lightDir)) / 2.0f, 0.0f, 1.0f);
    if (isDark) {

        light = (1.0f - d) * r3d->lightMag;
    }
    else {

        light = r3d->lightMag * d;
    }

    hue = (i32) clamp_f32(roundf(light * (TINT_MAX-1) * 2.0f), 0.0f, (TINT_MAX-1) * 2.0f);
    if (isDark) {

        hue *= -1;
    }

    return hue;
}


Renderer3D create_renderer_3D(TriangleBuffer* buffer) {

    Renderer3D r3d;

    r3d.buffer = buffer;

    r3d.lightDir = vec3(0, 0, 1.0f);
    r3d.lightMag = 1.0f;
    r3d.lightType = LIGHT_DARK;
    r3d.lightingEnabled = false;

    return r3d;
}


void r3d_draw_triangle(Renderer3D* r3d,
    Transformations* transf,
    Bitmap* texture, u8 color, i32 tint,
    Vector4 A, Vector4 B, Vector4 C, 
    Vector4 tA, Vector4 tB, Vector4 tC,
    bool outline1, bool outline2, bool outline3,
    Vector4 normal) {

    Triangle t;

    A = transf_apply_to_vector(transf, A);
    B = transf_apply_to_vector(transf, B);
    C = transf_apply_to_vector(transf, C);

    if (!create_triangle_3D(
        texture, color, tint, 
        A, B, C, tA, tB, tC,  
        outline1, outline2, outline3,
        &t)) {

        return;
    }

    if (!tribuf_push_triangle(r3d->buffer, t)) {

        printf("WARNING: Triangle buffer overflow!\n");
        return;
    }
}


void r3d_draw_mesh(Renderer3D* r3d, Transformations* transf, Mesh* mesh, Bitmap* texture, u8 color) {

    u32 i, j;
    i32 hue = 0;

    Vector4 A, B, C;
    Vector4 tA, tB, tC;

    bool outline1, outline2, outline3;

    for (i = 0; i < mesh->indexCount; i += 3) {

        j = mesh->indices[i];
        A = vec3(mesh->vertices[j*3], mesh->vertices[j*3 + 1], mesh->vertices[j*3 + 2]);
        tA = vec2(mesh->uvs[j*2], mesh->uvs[j*2 + 1]);
        outline1 = mesh->outlines[j];

        j = mesh->indices[i+1];
        B = vec3(mesh->vertices[j*3], mesh->vertices[j*3 + 1], mesh->vertices[j*3 + 2]);
        tB = vec2(mesh->uvs[j*2], mesh->uvs[j*2 + 1]);
        outline2 = mesh->outlines[j];

        j = mesh->indices[i+2];
        C = vec3(mesh->vertices[j*3], mesh->vertices[j*3 + 1], mesh->vertices[j*3 + 2]);
        tC = vec2(mesh->uvs[j*2], mesh->uvs[j*2 + 1]);
        outline3 = mesh->outlines[j];
        
        if (r3d->lightingEnabled) {

            hue = compute_hue(r3d, 
                transf_apply_rotation_to_vector(transf, 
                    vec3(mesh->normals[j*3], mesh->normals[j*3 + 1], mesh->normals[j*3 + 2])
                ), r3d->lightType == LIGHT_DARK);
        }

        r3d_draw_triangle(r3d, transf, 
            texture, color, hue,
            A, B, C, tA, tB, tC, 
            outline1, outline2, outline3,
            vec4_zero());
    }
}


void r3d_toggle_lighting(Renderer3D* r3d, bool state) {

    r3d->lightingEnabled = state;
}


void r3d_set_lighting_properties(Renderer3D* r3d,
    Vector4 lightDir, f32 lightMag, LightType lightType) {

    r3d->lightDir = lightDir;
    r3d->lightMag = lightMag;
    r3d->lightType = lightType;
}
