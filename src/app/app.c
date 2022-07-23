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

    app->testAngle = fmodf(app->testAngle - 0.025f * timeStep, M_PI * 2.0f);
}


static void redraw_callback(void* pApp, Window* win) {

    Application* app = (Application*) pApp;
    Canvas* canvas = app->canvas;

    f32 ratio = (f32) canvas->width / (f32) canvas->height;

    canvas_clear(canvas, 182);
    
    tribuf_flush(app->tribuffer);

    transf_load_identity(&app->transf);
    transf_rotate(&app->transf, app->testAngle, vec3(1.0f, -1.0f, 0.0f));
    transf_set_perspective_projection(&app->transf, 60.0f, ratio, 0.05f, 100.0f);
    transf_set_view(&app->transf, vec3(0, 0, -3.0f), vec3(0, 0, 0), vec3(0, 1.0f, 0));
/*
    r3d_draw_triangle(&app->r3d, &app->transf, app->cubeTextureNoise, 0,
        vec3(-0.5f, -0.5f, 0.0f),
        vec3(0.5f, -0.5f, 0.0f),
        vec3(0.0f, 0.5f, 0.0f),
        vec2(0, 0), vec2(1.0f, 0.5f), vec2(0.5f, 1.0f),
        vec3(0, 0, -1));
*/
    r3d_draw_mesh(&app->r3d, &app->transf, app->meshCube, app->cubeTextureNoise, 0);

    tribuf_draw(app->tribuffer, &app->rasterizer);

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

    app->tribuffer = new_triangle_buffer(2048, err);
    if (app->tribuffer == NULL) {

        dispose_application(app);
        return NULL;
    }

    app->mgen = new_model_generator(2048, err);
    if (app->mgen == NULL) {

        dispose_application(app);
        return NULL;
    }

    app->transf = create_transformations_manager();
    app->rasterizer = create_triangle_rasterizer(app->canvas);
    app->r3d = create_renderer_3D(app->tribuffer);

    app->cubeTextureNoise = generate_gaussian_noise_bitmap(
        64, 64, -1.33f, 1.33f, 1, 
        vec3(1.0f, 0.67f, 0.33f), 12345, err);
    if (app->cubeTextureNoise == NULL) {

        *err = memory_error();
        dispose_application(app);
        return NULL;
    }

    app->meshCube = mgen_generate_unit_cube(app->mgen, 2, err);
    if (app->meshCube == NULL) {

        dispose_application(app);
        return NULL;
    }

    app->testAngle = 0.0f;

    return app;
}


void dispose_application(Application* app) {

    if (app == NULL)
        return;

    dispose_canvas(app->canvas);
    dispose_triangle_buffer(app->tribuffer);
    dispose_model_generator(app->mgen);

    dispose_bitmap(app->cubeTextureNoise);
    dispose_mesh(app->meshCube);

    m_free(app);
}


void app_register_callbacks(Application* app, Window* win) {

    window_register_callbacks(win, update_callback, redraw_callback, (void*) app);
}
