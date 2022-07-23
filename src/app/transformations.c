#include "transformations.h"


static void transf_compute_product(Transformations* transf) {

    if (transf->productComputed) 
        return;

    transf->product = mat4_multiply(
        transf->projection, mat4_multiply(
            transf->view, transf->model)
        );

    transf->productComputed = true;
}


Transformations create_transformations_manager() {

    Transformations transf;

    transf.model = mat4_identity();
    transf.view = mat4_identity();
    transf.projection = mat4_identity();
    transf.rotation = mat4_identity();
    transf.product = mat4_identity();

    transf.productComputed = true;

    transf.stackPointer = 0;

    return transf;
}


void transf_load_identity(Transformations* transf) {

    transf->model = mat4_identity();
    transf->rotation = mat4_identity();

    transf->productComputed = false;
}


void transf_translate(Transformations* transf, Vector4 p) {

    transf->model = mat4_multiply(transf->model, 
        mat4_translate(p.x, p.y, p.z));

    transf->productComputed = false;
}


void transf_scale(Transformations* transf, Vector4 p) {

    transf->model = mat4_multiply(transf->model, 
        mat4_scale(p.x, p.y, p.z));

    transf->productComputed = false;
}


void transf_rotate(Transformations* transf, f32 angle, Vector4 axes) {

    Matrix4 rot = mat4_rotate(angle, axes.x, axes.y, axes.z);

    transf->model = mat4_multiply(transf->model, rot);
    transf->rotation = mat4_multiply(transf->rotation, rot);

    transf->productComputed = false;
}


void transf_push_model(Transformations* transf) {

    if (transf->stackPointer == MODEL_STACK_SIZE) 
        return;

    transf->modelStack[transf->stackPointer] = transf->model;
    transf->rotationStack[transf->stackPointer] = transf->rotation;

    ++ transf->stackPointer;
}


void transf_pop_model(Transformations* transf) {

    if (transf->stackPointer == 0)
        return;

    transf->model = transf->modelStack[transf->stackPointer];
    transf->rotation = transf->rotationStack[transf->stackPointer];
    transf->productComputed = false;

    -- transf->stackPointer;
}


void transf_set_perspective_projection(Transformations* transf, 
    f32 fovY, f32 aspectRatio,
    f32 near, f32 far) {

    transf->projection = mat4_perspective(fovY, aspectRatio, near, far);

    transf->productComputed = false;
}


void transf_set_view(Transformations* transf,
    Vector4 eye, Vector4 lookAt, Vector4 up) {

    transf->view = mat4_look_at(eye, lookAt, up);

    transf->productComputed = false;
}


Vector4 transf_apply_to_vector(Transformations* transf, Vector4 vec) {

    transf_compute_product(transf);

    return mat4_multiply_vector(transf->product, vec);
}


Vector4 transf_apply_rotation_to_vector(Transformations* transf, Vector4 vec) {

    return mat4_multiply_vector(transf->rotation, vec);
}
