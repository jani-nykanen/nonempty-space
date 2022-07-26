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


static void draw_outer_model(Application* app, bool isShadow, f32 groundHeight, i32 groundHue) {

    transf_push_model(&app->transf);
    transf_rotate(&app->transf, app->testAngle, vec3(1.0f, -1.0f, 0.0f));

    if (!isShadow) {
        
        r3d_draw_mesh(&app->r3d, &app->transf, app->meshThatOneThing, app->textureNoise1, 255);
    }
    else {

        r3d_project_mesh_to_ground(&app->r3d, &app->transf, app->meshThatOneThing, groundHeight, groundHue);
    }

    transf_pop_model(&app->transf);
}


static void draw_inner_model(Application* app, bool isShadow, f32 groundHeight, i32 groundHue) {

    const f32 MODEL_SCALE = 0.33f;

    transf_push_model(&app->transf);
    transf_rotate(&app->transf, M_PI/4.0f, vec3(-1.0f, 1.0f, 0.0f));
    transf_rotate(&app->transf, app->testAngle, vec3(-1.0f, 1.0f, 0.0f));
    transf_scale(&app->transf, vec3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE));

    if (!isShadow) {
        
        r3d_draw_mesh(&app->r3d, &app->transf, app->meshCube, NULL, 190);
    }
    else {

        r3d_project_mesh_to_ground(&app->r3d, &app->transf, app->meshCube, groundHeight, groundHue);
    }

    transf_pop_model(&app->transf);
}


static void draw_models(Application* app, bool isShadow) {

    const f32 GROUND_HEIGHT = 1.5f;
    const i32 GROUND_HUE = 0;

    Vector4 lightDir = vec4_normalize(vec3(0.1f, -0.25f, 1.0f), false);

    if (!isShadow) {

        r3d_toggle_lighting(&app->r3d, true);
        r3d_set_lighting_properties(&app->r3d, lightDir, 0.75f, LIGHT_DARK);
    }

    draw_outer_model(app, isShadow, GROUND_HEIGHT, GROUND_HUE);
    draw_inner_model(app, isShadow, GROUND_HEIGHT, GROUND_HUE);
    tribuf_draw(app->tribuffer, &app->rasterizer, !isShadow);
}


static void redraw_callback(void* pApp, Window* win) {

    Application* app = (Application*) pApp;
    Canvas* canvas = app->canvas;
    f32 ratio = (f32) canvas->width / (f32) canvas->height;

    canvas_clear(canvas, 83);
    
    tribuf_flush(app->tribuffer);

    transf_load_identity(&app->transf);
    transf_translate(&app->transf, vec3(0, 0.25f, 0));
    transf_set_perspective_projection(&app->transf, 60.0f, ratio, 0.05f, 100.0f);
    transf_set_view(&app->transf, vec3(0, 0, -2.75f), vec3(0, 0, 0), vec3(0, 1.0f, 0));

    // Shadows first
    draw_models(app, true);
    draw_models(app, false);

    canvas_update_window_content(canvas, win);
}


Application* new_application(Window* win, Error* err) {

    i32 w, h;

    Application* app = (Application*) calloc(1, sizeof(Application));
    if (app == NULL) {

        *err = memory_error();
        return NULL;
    }

    app->lookup = generate_lookup_tables();

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
    app->rasterizer = create_triangle_rasterizer(app->canvas, &app->lookup);
    app->r3d = create_renderer_3D(app->tribuffer);

    app->textureNoise1 = generate_gaussian_noise_bitmap(
        32, 32, -1.5f, 1.5f, 1, 
        vec3(1.0f, 0.67f, 0.33f), 12345, err);
    if (app->textureNoise1 == NULL) {

        *err = memory_error();
        dispose_application(app);
        return NULL;
    }

    app->textureNoise2 = generate_gaussian_noise_bitmap(
        64, 64, -1.5f, 1.5f, 1, 
        vec3(0.20f, 0.5f, 0.33f), 54321, err);
    if (app->textureNoise2 == NULL) {

        *err = memory_error();
        dispose_application(app);
        return NULL;
    }

    app->meshCube = mgen_generate_unit_cube(app->mgen, 3, err);
    if (app->meshCube == NULL) {

        dispose_application(app);
        return NULL;
    }

    app->meshThatOneThing = mgen_generate_that_specific_thing(app->mgen, 3, err);
    if (app->meshThatOneThing == NULL) {

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

    dispose_bitmap(app->textureNoise1);
    dispose_bitmap(app->textureNoise2);

    dispose_mesh(app->meshCube);
    dispose_mesh(app->meshThatOneThing);

    m_free(app);
}


void app_register_callbacks(Application* app, Window* win) {

    window_register_callbacks(win, update_callback, redraw_callback, (void*) app);
}
