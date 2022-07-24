#ifndef NONEMPTY_SPACE_APP_MODEL_GENERATOR_H
#define NONEMPTY_SPACE_APP_MODEL_GENERATOR_H


#include "common/vector.h"

#include "mesh.h"


typedef struct {

    f32* vertexBuffer;
    f32* uvBuffer;
    f32* normalBuffer;
    u16* indexBuffer;
    bool *outlineBuffer;

    u32 vertexCount;
    u32 uvCount;
    u32 normalCount;
    u32 indexCount;
    u32 outlineCount;

    u32 bufferSize;

} ModelGenerator;


ModelGenerator* new_model_generator(u32 bufferSize, Error* err);
void dispose_model_generator(ModelGenerator* mgen);

Mesh* mgen_generate_unit_cube(ModelGenerator* mgen, i32 subdivide, Error* err);
// What the heck it is called?
Mesh* mgen_generate_that_specific_thing(ModelGenerator* mgen, i32 holeWidth, Error* err);

Mesh* mgen_generate_mesh(ModelGenerator* mgen, Error* err);


#endif // NONEMPTY_SPACE_APP_MODEL_GENERATOR_H
