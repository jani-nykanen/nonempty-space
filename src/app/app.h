#ifndef NONEMPTY_SPACE_APP_APP_H
#define NONEMPTY_SPACE_APP_APP_H


#include "core/window.h"

#include "canvas.h"
#include "rasterizer.h"
#include "3d.h"
#include "modelgen.h"
#include "lookup.h"


typedef struct {
    
    Canvas* canvas;
    Canvas* maskedCanvas;

    TriangleRasterizer rasterizer;
    TriangleBuffer* tribuffer;
    Transformations transf;
    Renderer3D r3d;
    ModelGenerator* mgen;
    LookUpTables lookup;

    Bitmap* textureNoise1;
    Bitmap* textureNoise2;
    Bitmap* forestBackground;

    Mesh* meshCube;
    Mesh* meshThatOneThing;

    f32 modelAngle;
    f32 groundPos;
    f32 wind;

} Application;


Application* new_application(Window* win, Error* err);
void dispose_application(Application* app);

void app_register_callbacks(Application* app, Window* win);


#endif // NONEMPTY_SPACE_APP_APP_H
