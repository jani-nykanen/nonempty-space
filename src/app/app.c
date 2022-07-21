#include "app.h"
#include "noisegen.h"

#include "common/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


static void handle_default_key_shortcuts(Application* app, Window* win) {

    if ((window_get_key_state(win, KEY_LEFT_CONTROL) & STATE_DOWN_OR_PRESSED) == 1 &&
        window_get_key_state(win, KEY_Q) == STATE_PRESSED) {

        window_terminate(win);
    }

    if (((window_get_key_state(win, KEY_LEFT_ALT) & STATE_DOWN_OR_PRESSED) == 1 &&
        window_get_key_state(win, KEY_ENTER) == STATE_PRESSED) ||
        window_get_key_state(win, KEY_F4) == STATE_PRESSED) {

        window_toggle_fullscreen(win);
    }
}


static void update_callback(void* pApp, Window* win, f32 timeStep) {

    Application* app = (Application*) pApp;
    handle_default_key_shortcuts(app, win);

    app->testAngle = fmodf(app->testAngle - 0.05f * timeStep, M_PI * 2.0f);
}


static void draw_spinning_triangle(Application* app, Canvas* canvas) {

    const f32 RADIUS = 72.0f;
    const f32 CENTER_RADIUS = 32.0f;

    f32 cx = (f32) (canvas->width/2)  + cosf(-app->testAngle) * CENTER_RADIUS;
    f32 cy = (f32) (canvas->height/2) + sinf(-app->testAngle) * CENTER_RADIUS;

    i32 x1 = (i32) (cx + cosf(app->testAngle) * RADIUS);
    i32 y1 = (i32) (cy + sinf(app->testAngle) * RADIUS);

    i32 x2 = (i32) (cx + cosf(app->testAngle + M_PI*2.0f / 3.0f) * RADIUS);
    i32 y2 = (i32) (cy + sinf(app->testAngle + M_PI*2.0f / 3.0f) * RADIUS);

    i32 x3 = (i32) (cx + cosf(app->testAngle + M_PI*4.0f / 3.0f) * RADIUS);
    i32 y3 = (i32) (cy + sinf(app->testAngle + M_PI*4.0f / 3.0f) * RADIUS);

    tri_set_uv_coordinates(&app->tri, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 1.0f);
    tri_draw_triangle(&app->tri, 
        app->cubeTextureNoise, 255, 
        x1, y1, x2, y2, x3, y3);
}


static void redraw_callback(void* pApp, Window* win) {

    Application* app = (Application*) pApp;
    Canvas* canvas = app->canvas;

    canvas_clear(canvas, 182);

    canvas_draw_bitmap_fast(canvas, app->cubeTextureNoise, 0, 0);

    draw_spinning_triangle(app, canvas);

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
        vec3(1.0f, 0.67f, 0.33f), 12345, err);
    if (app->cubeTextureNoise == NULL) {

        *err = memory_error();
        dispose_application(app);
        return NULL;
    }

    app->tri = create_triangle_rasterizer(app->canvas);

    app->testAngle = 0.0f;

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
