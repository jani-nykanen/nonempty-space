#ifndef NONEMPTY_SPACE_CORE_INTERNAL_SHADER_H
#define NONEMPTY_SPACE_CORE_INTERNAL_SHADER_H


#include "common/types.h"
#include "common/error.h"


typedef struct {

    u32 program;

    i32 unifPos;
    i32 unifSize;
    // i32 unifTransform;
    
    i32 unifSampler;

} Shader;


Shader* new_shader(str vertexSrc, str fragmentSrc, Error* err);
void dispose_shader(Shader* shader);

void shader_pass_vertex_shader_properties(Shader* s, f32 x, f32 y, f32 w, f32 h);


#endif // NONEMPTY_SPACE_CORE_INTERNAL_SHADER_H
