#include "app.h"

#include "common/memory.h"

#include <stdio.h>
#include <stdlib.h>


static void update_callback(void* pApp, Window* win, f32 timeStep) {

    //...
}


static void redraw_callback(void* pApp, Window* win) {

    Application* app = (Application*) pApp;
    Canvas* canvas = app->canvas;

    canvas_clear(canvas, rand () % 255);

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

    return app;
}


void dispose_application(Application* app) {

    if (app == NULL)
        return;

    dispose_canvas(app->canvas);
    m_free(app);
}


void app_register_callbacks(Application* app, Window* win) {

    window_register_callbacks(win, update_callback, redraw_callback, (void*) app);
}
