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



static void add_cube_general(ModelGenerator* mgen, 
    f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 sz, i32 subdivide,
    const bool wallData[6]) {

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


void mgen_add_cube(ModelGenerator* mgen, Vector4 center, Vector4 size, i32 subdivide) {

    const bool WALLS[] = {true, true, true, true, true, true};

    add_cube_general(mgen, 
        center.x, center.y, center.z, 
        size.x, size.y, size.z, subdivide,
        WALLS);
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
