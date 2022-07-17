#include "shader.h"

#include "common/memory.h"

#include "glad/gl.h"

#include <stdio.h>
#include <stdlib.h>


#define MAX_LOG_LENGTH 1024 // Should be smaller than ERROR_MAX_LENGTH!


static u32 compile_shader(u32 type, const char* src, Error* err) {

    u32 out = glCreateShader(type);

    i32 result = 0;
    char log [MAX_LOG_LENGTH];
    i32 logLength = 0;

    // TODO: typecast src to (const char* const*)?
    glShaderSource(out, 1, &src, NULL);
	glCompileShader(out);

    glGetShaderiv(out, GL_COMPILE_STATUS, &result);
    glGetShaderiv(out, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 0) {

        if (logLength > MAX_LOG_LENGTH-1)   
            logLength = MAX_LOG_LENGTH-1;

        glGetShaderInfoLog(out, logLength, NULL, log);
        *err = create_error_str_param_1("Shader compilation error: ", log);
        return 0;
    }

    return out;
}


static u32 link_program(u32 vertex, u32 frag, Error* err) {

    u32 program = glCreateProgram();
    i32 result = 0;
    char log [MAX_LOG_LENGTH];
    i32 logLength = 0;

    glAttachShader(program, vertex);
    glAttachShader(program, frag);

    glBindAttribLocation(program, 0, "vertexPos");
    glBindAttribLocation(program, 1, "vertexUV");

    glLinkProgram(program);

    glGetShaderiv(program, GL_LINK_STATUS, &result);
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 0) {

        if (logLength > MAX_LOG_LENGTH-1)   
            logLength = MAX_LOG_LENGTH-1;

        glGetShaderInfoLog(program, logLength, NULL, log);
        *err = create_error_str_param_1("Shader linking error: ", log);
        return 0;
    }

    return program;
}


static void get_uniform_locations(Shader* shader) {

    shader->unifPos = glGetUniformLocation(shader->program, "pos");
    shader->unifSize = glGetUniformLocation(shader->program, "size");
    // shader->unifTransform = glGetUniformLocation(shader->program, "transform");
    shader->unifSampler = glGetUniformLocation(shader->program, "texSampler");
}


static void set_default_uniforms(Shader* shader) {


    glUniform3f(shader->unifPos, 0.0f, 0.0f, 0.0f);
    glUniform3f(shader->unifSize, 1.0f, 1.0f, 1.0f);
    // glUniformMatrix4fv(shader->unifTransform, 1, false, (const f32*)id.m);
    glUniform1i(shader->unifSampler, 0);
}


static i32 build_shader(Shader* s, const str vertexSrc, const str fragmentSrc, Error* err) {

    u32 vertex, frag;

    vertex = compile_shader(GL_VERTEX_SHADER, vertexSrc, err);
    if (vertex == 0) {

        return 1;
    }

    frag = compile_shader(GL_FRAGMENT_SHADER, fragmentSrc, err);
    if (frag == 0) {

        return 1;
    }

    s->program = link_program(vertex, frag, err);
    if (s->program == 0) {

        return 1;
    }

    glDetachShader(s->program, vertex);
	glDetachShader(s->program, frag);
	
	glDeleteShader(vertex);
	glDeleteShader(frag);

    glUseProgram(s->program);
    get_uniform_locations(s);
    set_default_uniforms(s);

    return 0;
}


Shader* new_shader(const str vertexSrc, const str fragmentSrc, Error* err) {

    Shader* s = (Shader*) calloc(1, sizeof(Shader));
    if (s == NULL) {

        *err = memory_error();
        return NULL;
    }

    if (build_shader(s, vertexSrc, fragmentSrc, err) != 0) {

        dispose_shader(s);
        return NULL;
    }
    
    return s;
}


void dispose_shader(Shader* shader) {

    if (shader == NULL)
        return; 

    if (shader->program != 0)
        glDeleteShader(shader->program);

    m_free(shader);
}


void shader_pass_vertex_shader_properties(Shader* s, f32 x, f32 y, f32 w, f32 h) {

    glUniform2f(s->unifPos, x, y);
    glUniform2f(s->unifSize, w, h);
}
