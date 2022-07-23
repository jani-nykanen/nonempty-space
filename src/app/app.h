#ifndef NONEMPTY_SPACE_APP_APP_H
#define NONEMPTY_SPACE_APP_APP_H


#include "core/window.h"

#include "canvas.h"
#include "rasterizer.h"
#include "3d.h"
#include "modelgen.h"


typedef struct {

    Canvas* canvas;
    TriangleRasterizer rasterizer;
    TriangleBuffer* tribuffer;
    Transformations transf;
    Renderer3D r3d;
    ModelGenerator* mgen;

    Bitmap* cubeTextureNoise;
    Mesh* meshCube;

    f32 testAngle;

} Application;


Application* new_application(Window* win, Error* err);
void dispose_application(Application* app);

void app_register_callbacks(Application* app, Window* win);


#endif // NONEMPTY_SPACE_APP_APP_H
