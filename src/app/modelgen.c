#include "modelgen.h"

#include "common/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FRONT  1  // 0b000001
#define RIGHT  2  // 0b000010
#define BACK   4  // 0b000100
#define LEFT   8  // 0b001000
#define BOTTOM 16 // 0b010000
#define TOP    32 // 0b100000
#define ALL    63 // 0b111111


static void add_vector3(Vector4 v, f32* arr, u32* p) {

    i32 i = (*p);

    arr[i] = v.x;
    arr[i + 1] = v.y;
    arr[i + 2] = v.z;

    *p += 3;
}


static void add_vector3_repeat(Vector4 v, f32* arr, u32* p, i32 repeat) {

    i32 i;

    for (i = 0; i < repeat; ++ i) {

        add_vector3(v, arr, p);
    }
}


static void add_values_2(f32 x, f32 y, f32* arr, u32* p) {

    i32 i = (*p);

    arr[i] = x;
    arr[i + 1] = y;

    *p += 2;
}


static void add_plane(ModelGenerator* mgen, 
    Vector4 start, Vector4 dirx, Vector4 diry, 
    Vector4 normal, i32 subdivide) {

    i32 i, j;
    Vector4 A, B, C, D;

    f32 step = 1.0f / ((f32) subdivide);

    Vector4 scaledDirX = vec4_scalar_multiply(dirx, step);
    Vector4 scaledDirY = vec4_scalar_multiply(diry, step);

    f32 tstep = 1.0f / ((f32) (subdivide));
    f32 tx = 0.0f;
    f32 ty = 0.0f;

    u32 k;

    for (j = 0; j < subdivide; ++ j, ty += tstep) {

        tx = 0.0f;
        for (i = 0; i < subdivide; ++ i, tx += tstep) {

            A = vec4_add(start,
                    vec4_add(
                        vec4_scalar_multiply(dirx, i*step),
                        vec4_scalar_multiply(diry, j*step)
                    ));
            B = vec4_add(A, scaledDirX);
            C = vec4_add(A, vec4_add(scaledDirX, scaledDirY));
            D = vec4_add(A, scaledDirY);
            
            add_vector3(A, mgen->vertexBuffer, &mgen->vertexCount);
            add_vector3(B, mgen->vertexBuffer, &mgen->vertexCount);
            add_vector3(C, mgen->vertexBuffer, &mgen->vertexCount);

            add_vector3(C, mgen->vertexBuffer, &mgen->vertexCount);
            add_vector3(D, mgen->vertexBuffer, &mgen->vertexCount);
            add_vector3(A, mgen->vertexBuffer, &mgen->vertexCount);

            add_values_2(tx, ty, mgen->uvBuffer, &mgen->uvCount);
            add_values_2(tx + tstep, ty, mgen->uvBuffer, &mgen->uvCount);
            add_values_2(tx + tstep, ty + tstep, mgen->uvBuffer, &mgen->uvCount);

            add_values_2(tx + step, ty + tstep, mgen->uvBuffer, &mgen->uvCount);
            add_values_2(tx, ty + tstep, mgen->uvBuffer, &mgen->uvCount);
            add_values_2(tx, ty, mgen->uvBuffer, &mgen->uvCount);

            add_vector3_repeat(normal, mgen->normalBuffer, &mgen->normalCount, 6);

            for (k = mgen->indexCount; k < mgen->indexCount + 6; ++ k) {

                mgen->indexBuffer[k] = (u16) k;
            }
            mgen->indexCount += 6;
        }
    }
}


static void add_cube_general(ModelGenerator* mgen, 
    f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 sz, i32 subdivide,
    u8 walls) {

    Vector4 left = vec3(sx, 0.0f, 0.0f);
    Vector4 up = vec3(0.0f, sy, 0.0f);
    Vector4 forward = vec3(0.0f, 0.0f, sz);
 
    // Front wall
    if ((walls & FRONT) == FRONT) {

        add_plane(mgen, 
            vec3(x - sx/2, y - sy/2, z + sz/2), 
            left, up,
            vec3(0, 0, -1.0f),
            subdivide);
    }

    // Back wall
    if ((walls & BACK) == BACK) {

        add_plane(mgen, 
            vec3(x - sx/2, y - sy/2, z - sz/2), 
            left, up, 
            vec3(0, 0, 1.0f),
            subdivide);
    }

     // Left wall
    if ((walls & LEFT) == LEFT) {

        add_plane(mgen, 
            vec3(x - sx/2, y - sy/2, z - sz/2), 
            forward, up, 
            vec3(1.0f, 0, 0.0f),
            subdivide);
    }

    // Right wall
    if ((walls & RIGHT) == RIGHT) {

        add_plane(mgen, 
            vec3(x + sx/2, y - sy/2, z - sz/2), 
            forward, up, 
            vec3(-1.0f, 0, 0.0f),
            subdivide);
    }

    // Top wall
    if ((walls & TOP) == TOP) {

        add_plane(mgen, 
            vec3(x - sx/2, y - sy/2, z - sz/2), 
            left, forward, 
            vec3(0.0f, 1.0f, 0.0f),
            subdivide);
    }

    // Bottom wall
    if ((walls & BOTTOM) == BOTTOM) {

        add_plane(mgen, 
            vec3(x - sx/2, y + sy/2, z - sz/2), 
            left, forward, 
            vec3(0.0f, -1.0f, 0.0f),
            subdivide);
    }

}


ModelGenerator* new_model_generator(u32 bufferSize, Error* err) {

    u32 vertexCount = bufferSize * 3;
    u32 uvCount = bufferSize * 2;
    u32 normalCount = bufferSize * 3;
    u32 indexCount = bufferSize;

    ModelGenerator* mgen = (ModelGenerator*) calloc(1, sizeof(ModelGenerator));
    if (mgen == NULL) {

        *err = memory_error();
        return NULL;
    }

    if ((mgen->vertexBuffer = (f32*) calloc(vertexCount, sizeof(f32))) == NULL ||
        (mgen->uvBuffer = (f32*) calloc(uvCount, sizeof(f32))) == NULL ||
        (mgen->normalBuffer = (f32*) calloc(normalCount, sizeof(f32))) == NULL ||
        (mgen->indexBuffer = (u16*) calloc(indexCount, sizeof(u16))) == NULL) {

        *err = memory_error();
        dispose_model_generator(mgen);

        return NULL;
    }

    mgen->vertexCount = 0;
    mgen->uvCount = 0;
    mgen->normalCount = 0;
    mgen->indexCount = 0;

    mgen->bufferSize = bufferSize;

    return mgen;
}


void dispose_model_generator(ModelGenerator* mgen) {

    if (mgen == NULL)
        return;

    m_free(mgen->vertexBuffer);
    m_free(mgen->uvBuffer);
    m_free(mgen->normalBuffer);
    m_free(mgen->indexBuffer);

    m_free(mgen);
}


Mesh* mgen_generate_unit_cube(ModelGenerator* mgen, i32 subdivide, Error* err) {

    add_cube_general(mgen, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, subdivide, ALL);

    return mgen_generate_mesh(mgen, err);
}


Mesh* mgen_generate_that_specific_thing(ModelGenerator* mgen, i32 holeWidth, Error* err) {

    const i32 CORNER_COUNT = 8;

    const u8 CORNERS[] = {
        FRONT  | TOP | LEFT,
        FRONT  | TOP | RIGHT,
        BACK   | TOP | LEFT,
        BACK   | TOP | RIGHT,

        FRONT  | BOTTOM | LEFT,
        FRONT  | BOTTOM | RIGHT,
        BACK   | BOTTOM | LEFT,
        BACK   | BOTTOM | RIGHT
    };
    const f32 CORNER_X[] = {
        -1.0f, 1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f};
    const f32 CORNER_Y[] = {
        -1.0f, -1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f, 1.0f};
    const f32 CORNER_Z[] = {
        1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f, -1.0f
    };

    // Poor naming, I know
    const u8 VERTICAL_BAR = FRONT | BACK | LEFT | RIGHT;
    const u8 HORIZONTAL_BAR = FRONT | BACK | TOP | BOTTOM;
    // Yes, "longitudinal" is a word, I checked the dictionary
    const u8 LONGITUDINAL_BAR = TOP | BOTTOM | LEFT | RIGHT;

    const f32 VBAR_X[] = {-1.0f, -1.0f, 1.0f, 1.0f};
    const f32 VBAR_Z[] = {-1.0f, 1.0f, -1.0f, 1.0f};

    const f32 HBAR_Y[] = {-1.0f, -1.0f, 1.0f, 1.0f};
    const f32 HBAR_Z[] = {-1.0f, 1.0f, -1.0f, 1.0f};

    const f32 LBAR_X[] = {-1.0f, -1.0f, 1.0f, 1.0f};
    const f32 LBAR_Y[] = {-1.0f, 1.0f, -1.0f, 1.0f};

    i32 i, j, k;
    i32 w = holeWidth + 2;

    f32 unitSize = 1.0f / ((f32) w);
    f32 start = 0.5f - unitSize/2.0f;

    // Corners
    for (i = 0; i < CORNER_COUNT; ++ i) {

        add_cube_general(mgen, 
            start * CORNER_X[i], 
            start * CORNER_Y[i], 
            start * CORNER_Z[i], 
            unitSize, unitSize, unitSize, 1, 
            CORNERS[i]);
    }

    // Bars
    for (k = 0; k < holeWidth; ++ k) {

        for (j = 0; j < 4; ++ j) {

            // Vertical
            add_cube_general(mgen,
                start * VBAR_X[j],
                start - (k + 1) * unitSize,
                start * VBAR_Z[j],
                unitSize, unitSize, unitSize, 1, 
                VERTICAL_BAR);

            // Horizontal
            add_cube_general(mgen,
                start - (k + 1) * unitSize,
                start * HBAR_Y[j],
                start * HBAR_Z[j],
                unitSize, unitSize, unitSize, 1, 
                HORIZONTAL_BAR);

             // Longitudinal
            add_cube_general(mgen, 
                start * LBAR_X[j],
                start * LBAR_Y[j],
                start - (k + 1) * unitSize,
                unitSize, unitSize, unitSize, 1, 
                LONGITUDINAL_BAR);
        }
    }

    return mgen_generate_mesh(mgen, err);
}


Mesh* mgen_generate_mesh(ModelGenerator* mgen, Error* err) {

    Mesh* out;

    out = new_mesh(
        (const f32*) mgen->vertexBuffer,
        (const f32*) mgen->uvBuffer,
        (const f32*) mgen->normalBuffer,
        (const u16*) mgen->indexBuffer,
        mgen->vertexCount,
        mgen->uvCount,
        mgen->normalCount,
        mgen->indexCount,
        err);
    if (out == NULL) {

        return NULL;
    }

    mgen->vertexCount = 0;
    mgen->uvCount = 0;
    mgen->normalCount = 0;
    mgen->indexCount = 0;

    return out;
}
