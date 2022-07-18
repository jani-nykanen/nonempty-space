#include "app.h"
#include "noisegen.h"

#include "common/memory.h"

#include <stdio.h>
#include <stdlib.h>


static void update_callback(void* pApp, Window* win, f32 timeStep) {

    //...
}


static void redraw_callback(void* pApp, Window* win) {

    Application* app = (Application*) pApp;
    Canvas* canvas = app->canvas;

    canvas_clear(canvas, 182);

    canvas_draw_bitmap_fast(canvas, app->cubeTextureNoise, 0, 0);

    canvas_update_window_content(canvas, win);
}


Application* new_application(Window* win, Error* err) {

    i32 w, h;

    Application* app = (Application*) calloc(1, sizeof(Application));
    if (app == NULL) {

        *err = memory_error();
        return NULL;
    }

    window_get_canvas_size(win, &w, &h);
    app->canvas = new_canvas(w, h, err);
    if (app->canvas == NULL) {

        dispose_application(app);
        return NULL;
    }

    app->cubeTextureNoise = generate_gaussian_noise_bitmap(
        128, 128, -1.33f, 1.33f, 1, 
        vec3(1.0f, 1.0f, 1.0f), 12345, err);
    if (app->cubeTextureNoise == NULL) {

        *err = memory_error();
        dispose_application(app);
        return NULL;
    }

    return app;
}


void dispose_application(Application* app) {

    if (app == NULL)
        return;

    dispose_canvas(app->canvas);
    dispose_bitmap(app->cubeTextureNoise);
    m_free(app);
}


void app_register_callbacks(Application* app, Window* win) {

    window_register_callbacks(win, update_callback, redraw_callback, (void*) app);
}
