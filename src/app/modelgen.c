#include "modelgen.h"

#include "common/memory.h"

#include <stdio.h>
#include <stdlib.h>


#define FRONT 0
#define RIGHT 1
#define BACK 2
#define LEFT 3
#define BOTTOM 4
#define TOP 5


static void add_vector3(Vector4 v, f32* arr, u32* p) {

    i32 i = (*p);

    arr[i] = v.x;
    arr[i + 1] = v.y;
    arr[i + 2] = v.z;

    *p += 3;
}


static void add_vector3_repeat(Vector4 v, f32* arr, u32*p, i32 repeat) {

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
    const bool wallData[6]) {

    Vector4 left = vec3(sx, 0.0f, 0.0f);
    Vector4 up = vec3(0.0f, sy, 0.0f);
    Vector4 forward = vec3(0.0f, 0.0f, sz);

    // Front wall
    if (wallData[FRONT]) {

        add_plane(mgen, 
            vec3(x - sx/2, y - sy/2, z + sz/2), 
            left, up,
            vec3(0, 0, -1.0f),
            subdivide);
    }

    // Back wall
    if (wallData[BACK]) {

        add_plane(mgen, 
            vec3(x - sx/2, y - sy/2, z - sz/2), 
            left, up, 
            vec3(0, 0, 1.0f),
            subdivide);
    }

     // Left wall
    if (wallData[LEFT]) {

        add_plane(mgen, 
            vec3(x - sx/2, y - sy/2, z - sz/2), 
            forward, up, 
            vec3(-1.0f, 0, 0.0f),
            subdivide);
    }

    // Right wall
    if (wallData[RIGHT]) {

        add_plane(mgen, 
            vec3(x + sx/2, y - sy/2, z - sz/2), 
            forward, up, 
            vec3(1.0f, 0, 0.0f),
            subdivide);
    }

    // Top wall
    if (wallData[TOP]) {

        add_plane(mgen, 
            vec3(x - sx/2, y - sy/2, z - sz/2), 
            left, forward, 
            vec3(0.0f, 1, 0.0f),
            subdivide);
    }

    // Bottom wall
    if (wallData[BOTTOM]) {

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

    const bool WALLS[] = {true, true, true, true, true, true};

    add_cube_general(mgen, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, subdivide, WALLS);

    return mgen_generate_mesh(mgen, err);
}


Mesh* mgen_generate_mesh(ModelGenerator* mgen, Error* err) {

    Mesh* out = new_mesh(
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
