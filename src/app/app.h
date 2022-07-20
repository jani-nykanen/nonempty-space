#ifndef NONEMPTY_SPACE_APP_APP_H
#define NONEMPTY_SPACE_APP_APP_H


#include "core/window.h"

#include "canvas.h"
#include "triangle.h"


typedef struct {

    Canvas* canvas;
    Bitmap* cubeTextureNoise;
    TriangleRasterizer tri;

    f32 testAngle;

} Application;


Application* new_application(Window* win, Error* err);
void dispose_application(Application* app);

void app_register_callbacks(Application* app, Window* win);


#endif // NONEMPTY_SPACE_APP_APP_H
