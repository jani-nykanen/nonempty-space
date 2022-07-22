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



static void add_plane(ModelGenerator* mgen, 
    Vector4 start, Vector4 dirx, Vector4 diry,
    f32 ushift, f32 vshift, f32 uwidth, f32 vwidth,
    i32 subdivide) {

    i32 i, j;

    for (j = 0; j < subdivide; ++ j) {

        for (i = 0; i < subdivide; ++ i) {

            // TODO: Everything
        }
    }
}


static void add_cube_general(ModelGenerator* mgen, 
    f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 sz, i32 subdivide,
    f32 ushift, f32 vshift, f32 uwidth, f32 vwidth,
    const bool wallData[6]) {

    Vector4 left = vec3(1.0f, 0.0f, 0.0f);
    Vector4 up = vec3(0.0f, 1.0f, 0.0f);

    // Front wall
    if (wallData[FRONT]) {

        add_plane(mgen, 
            vec3(x - sx/2, y - sy/2, z + sz/2), 
            left, up, 
            ushift, vshift, uwidth, vwidth, 
            subdivide);
    }
}


ModelGenerator* new_model_generator(u32 bufferSize, Error* err) {

    ModelGenerator* mgen = (ModelGenerator*) calloc(1, sizeof(ModelGenerator));
    if (mgen == NULL) {

        *err = memory_error();
        return NULL;
    }

    mgen->vertexCount = bufferSize * 3;
    mgen->uvCount = bufferSize * 2;
    mgen->normalCount = bufferSize * 3;
    mgen->indexCount = bufferSize;

    if ((mgen->vertexBuffer = (f32*) calloc(mgen->vertexCount, sizeof(f32))) == NULL ||
        (mgen->uvBuffer = (f32*) calloc(mgen->uvCount, sizeof(f32))) == NULL ||
        (mgen->normalBuffer = (f32*) calloc(mgen->normalCount, sizeof(f32))) == NULL ||
        (mgen->indexBuffer = (u16*) calloc(mgen->indexCount, sizeof(u16))) == NULL) {

        *err = memory_error();
        dispose_model_generator(mgen);

        return NULL;
    }

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
        0.5f, 0.5f, 0.5f, subdivide,
        0.5f, 0.5f, 1.0f, 1.0f, WALLS);

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
