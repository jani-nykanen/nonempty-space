#ifndef NONEMPTY_SPACE_CORE_INTERNAL_SHADER_H
#define NONEMPTY_SPACE_CORE_INTERNAL_SHADER_H


#include "common/types.h"
#include "common/error.h"


typedef struct {

    u32 program;

    i32 unifPos;
    i32 unifSize;
    i32 unifTransform;
    
    i32 unifSampler;

} Shader;


Shader* new_shader(const str vertexSrc, const str fragmentSrc, Error* err);
void dispose_shader(Shader* shader);


#endif // NONEMPTY_SPACE_CORE_INTERNAL_SHADER_H
