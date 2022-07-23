#ifndef NONEMPTY_SPACE_APP_TRANSFORMATIONS_H
#define NONEMPTY_SPACE_APP_TRANSFORMATIONS_H


#include "common/matrix.h"


#define MODEL_STACK_SIZE 32


typedef struct {

    Matrix4 model;
    Matrix4 view;
    Matrix4 projection;
    Matrix4 rotation;
    Matrix4 product;

    bool productComputed;

    Matrix4 modelStack [MODEL_STACK_SIZE];
    Matrix4 rotationStack [MODEL_STACK_SIZE];
    u32 stackPointer;

} Transformations;


Transformations create_transformations_manager();

void transf_load_identity(Transformations* transf);
void transf_translate(Transformations* transf, Vector4 p);
void transf_scale(Transformations* transf, Vector4 p);
void transf_rotate(Transformations* transf, f32 angle, Vector4 axes);

void transf_push_model(Transformations* transf);
void transf_pop_model(Transformations* transf);

void transf_set_perspective_projection(Transformations* transf, 
    f32 fovY, f32 aspectRatio,
    f32 near, f32 far);

void transf_set_view(Transformations* transf,
    Vector4 eye, Vector4 lookAt, Vector4 up);

Vector4 transf_apply_to_vector(Transformations* transf, Vector4 vec);
Vector4 transf_apply_rotation_to_vector(Transformations* transf, Vector4 vec);


#endif // NONEMPTY_SPACE_APP_TRANSFORMATIONS_H
