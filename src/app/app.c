#include "app.h"
#include "noisegen.h"
#include "floor.h"
#include "background.h"
#include "font.h"

#include "common/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


static void handle_default_key_shortcuts(Application* app, Window* win) {

    if (window_get_key_state(win, KEY_ESCAPE) == STATE_PRESSED ||
        ((window_get_key_state(win, KEY_LEFT_CONTROL) & STATE_DOWN_OR_PRESSED) == 1 &&
        window_get_key_state(win, KEY_Q) == STATE_PRESSED)) {

        window_terminate(win);
    }

    if (((window_get_key_state(win, KEY_LEFT_ALT) & STATE_DOWN_OR_PRESSED) == 1 &&
        window_get_key_state(win, KEY_ENTER) == STATE_PRESSED) ||
        window_get_key_state(win, KEY_F4) == STATE_PRESSED) {

        window_toggle_fullscreen(win);
    }
}


static void update_callback(void* pApp, Window* win, f32 timeStep) {

    const f32 MODEL_ROTATION = 0.025f;
    const f32 GROUND_SPEED = 0.005f;
    const f32 WIND_SPEED = 0.05f;
    const f32 TEXT_ALPHA_SPEED = 0.1f;
    const f32 SKY_WAVE_SPEED = 0.05f;

    Application* app = (Application*) pApp;
    handle_default_key_shortcuts(app, win);

    app->modelAngle = fmodf(app->modelAngle - MODEL_ROTATION * timeStep, M_PI * 2.0f);
    app->groundPos += GROUND_SPEED * timeStep;

    app->groundPos = fmodf(app->groundPos, 1.0f);
    app->wind = fmodf(app->wind + WIND_SPEED*timeStep, M_PI*2.0f);
    app->skyWave = fmodf(app->skyWave + SKY_WAVE_SPEED*timeStep, M_PI*2.0f);
    app->textAlpha = fmodf(app->textAlpha + TEXT_ALPHA_SPEED*timeStep, M_PI*2.0f);

    app->framerate = (i32) roundf(60.0f / timeStep);
}


static void draw_outer_model(Application* app, bool isShadow, f32 groundHeight) {

    transf_push_model(&app->transf);
    transf_rotate(&app->transf, app->modelAngle, vec3(1.0f, -1.0f, 0.0f));

    if (!isShadow) {
        
        r3d_draw_mesh(&app->r3d, &app->transf, app->meshThatOneThing, app->textureNoise1, 255);
    }
    else {

        r3d_project_mesh_to_ground(&app->r3d, &app->transf, app->meshThatOneThing, groundHeight, 255);
    }

    transf_pop_model(&app->transf);
}


static void draw_inner_model(Application* app, bool isShadow, f32 groundHeight) {

    const f32 MODEL_SCALE = 0.33f;

    transf_push_model(&app->transf);
    transf_rotate(&app->transf, M_PI/4.0f, vec3(-1.0f, 1.0f, 0.0f));
    transf_rotate(&app->transf, app->modelAngle, vec3(-1.0f, 1.0f, 0.0f));
    transf_scale(&app->transf, vec3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE));

    if (!isShadow) {
        
        r3d_draw_mesh(&app->r3d, &app->transf, app->meshCube, NULL, 211);
    }
    else {

        r3d_project_mesh_to_ground(&app->r3d, &app->transf, app->meshCube, groundHeight, 255);
    }

    transf_pop_model(&app->transf);
}


static void draw_models(Application* app, bool isShadow) {

    const f32 GROUND_HEIGHT = 1.5f;

    Vector4 lightDir = vec4_normalize(vec3(0.1f, -0.25f, 1.0f), false);

    if (!isShadow) {

        r3d_toggle_lighting(&app->r3d, true);
        r3d_set_lighting_properties(&app->r3d, lightDir, 0.75f, LIGHT_DARK);
    }

    draw_outer_model(app, isShadow, GROUND_HEIGHT);
    draw_inner_model(app, isShadow, GROUND_HEIGHT);
    tribuf_draw(app->tribuffer, &app->rasterizer, !isShadow);
}


static void draw_forest(Application* app) {

    const i32 FOREST_Y = 80;
    const f32 AMPLITUDE = 2.0f;
    const f32 PERIOD = M_PI*2.0f;

    Canvas* canvas = app->canvas;
    Bitmap* bmp = app->forestBackground;
    i32 y;

    i32 shiftx = 0; // (i32) roundf(sinf(app->wind) * AMPLITUDE);
    f32 step = PERIOD / ((f32) bmp->height);

    i32 offset = ((i32) bmp->width) - ((i32) canvas->width);

    // canvas_draw_bitmap(canvas, app->forestBackground, 0, 80, FLIP_NONE);

    for (y = 0; y < ((i32) bmp->height); ++ y) {

        shiftx = (i32) roundf(sinf(app->wind + step*y) * AMPLITUDE);

        canvas_draw_bitmap_region(canvas, app->forestBackground,
            0, y, (i32) bmp->width, 1,
            -offset/2 + shiftx, y + FOREST_Y, FLIP_NONE);
    }
}


static void draw_text(Application* app) {

    const i32 BASE_HUE = 6;
    const i32 HUE_VARY = 2;

    Canvas* canvas = app->canvas;
    char buffer[64];

    i32 hue = (i32) roundf(BASE_HUE + sinf(app->textAlpha) * HUE_VARY);

    snprintf(buffer, 64, "FPS: %d", app->framerate);
    canvas_draw_text(canvas, app->debugFont, buffer, 2, 2, -1, 0, ALIGN_LEFT);

    canvas_blend_text(canvas, app->debugFont, "Press ESC to quit", 
        canvas->width - 2, 
        canvas->height - 10, 
        -1, 0, ALIGN_RIGHT, hue);
}


static void redraw_callback(void* pApp, Window* win) {

    Application* app = (Application*) pApp;
    Canvas* canvas = app->canvas;
    f32 ratio = (f32) canvas->width / (f32) canvas->height;

    canvas_clear(canvas, 10);
    canvas_clear_mask(app->maskedCanvas);
    
    canvas_draw_vertically_waving_bitmap(canvas, app->skyBackground, 0, 0,
        M_PI*2.0f - app->skyWave, 4.0f, 1.0f,(M_PI*2.0f) / 64.0f);
    draw_forest(app);

    tribuf_flush(app->tribuffer);

    transf_load_identity(&app->transf);
    transf_set_perspective_projection(&app->transf, 60.0f, ratio, 0.05f, 100.0f);
    transf_set_view(&app->transf, vec3(0, 0, -2.75f), vec3(0, 0, 0), vec3(0, 1.0f, 0));

    draw_floor_3D(app->canvas, app->textureNoise2, &app->transf, 
        -1.5f, 6.0f, 2.0f, -1.5f, -app->groundPos,
        true,  0.75f);

    transf_translate(&app->transf, vec3(0, 0.25f, 0));

    // Shadows
    tri_change_active_canvas(&app->rasterizer, app->maskedCanvas, true);
    draw_models(app, true);
    canvas_blend_bitmap_region(canvas, (Bitmap*) app->maskedCanvas, 
        64, 128, 128, 64, 64, 192-64, -3, FLIP_NONE);
    
    // Actual models
    tri_change_active_canvas(&app->rasterizer, app->canvas, false);
    draw_models(app, false);

    draw_text(app);

    canvas_update_window_content(canvas, win);
}


Application* new_application(Window* win, Error* err) {

    const u16 FOREST_MARGIN = 8;
    const u8 SKY_COLORS[] = {10, 15, 19};
    const i32 SKY_COLOR_TRANSITION_HEIGHT[] = {12, 16, 20, 24};
    const u32 SKY_COLOR_COUNT = 3;

    i32 w, h;

    Application* app = (Application*) calloc(1, sizeof(Application));
    if (app == NULL) {

        *err = memory_error();
        return NULL;
    }

    app->lookup = generate_lookup_tables();

    window_get_canvas_size(win, &w, &h);
    app->canvas = new_canvas(w, h, false, &app->lookup, err);
    if (app->canvas == NULL) {

        dispose_application(app);
        return NULL;
    }

    app->maskedCanvas = new_canvas(w, h, true, &app->lookup, err);
    if (app->maskedCanvas == NULL) {

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

        dispose_application(app);
        return NULL;
    }

    app->textureNoise2 = generate_gaussian_noise_bitmap(
        256, 256, -1.0f, 1.0f, 2, 
        vec3(0.80f, 1.0f, 0.50f), 54321, err);
    if (app->textureNoise2 == NULL) {

        dispose_application(app);
        return NULL;
    }

    app->forestBackground = generate_forest_background(
        ((u16) w) + FOREST_MARGIN*2, 48, 10128, 0, err);
    if (app->forestBackground == NULL) {

        dispose_application(app);
        return NULL;
    }

    app->skyBackground = generate_starry_sky((u16) w, 120, 70119, 
        (u8*) SKY_COLORS, SKY_COLOR_COUNT, 
        (i32*) SKY_COLOR_TRANSITION_HEIGHT, 32, 208, 40, 24, err);
    if (app->skyBackground == NULL) {

        dispose_application(app);
        return NULL;
    }

    app->debugFont = create_debug_font(err);
    if (app->debugFont == NULL) {

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

    app->modelAngle = 0.0f;
    app->groundPos = 0.0f;
    app->wind = 0.0f;
    app->framerate = 60;
    app->textAlpha = 0.0f;
    app->skyWave = 0.0f;

    return app;
}


void dispose_application(Application* app) {

    if (app == NULL)
        return;

    dispose_canvas(app->canvas);
    dispose_canvas(app->maskedCanvas);

    dispose_triangle_buffer(app->tribuffer);
    dispose_model_generator(app->mgen);

    dispose_bitmap(app->textureNoise1);
    dispose_bitmap(app->textureNoise2);
    dispose_bitmap(app->forestBackground);
    dispose_bitmap(app->skyBackground);
    dispose_bitmap(app->debugFont);

    dispose_mesh(app->meshCube);
    dispose_mesh(app->meshThatOneThing);

    m_free(app);
}


void app_register_callbacks(Application* app, Window* win) {

    window_register_callbacks(win, update_callback, redraw_callback, (void*) app);
}
