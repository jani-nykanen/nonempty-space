#include "app.h"

#include "common/memory.h"

#include <stdio.h>
#include <stdlib.h>


static void update_callback(void* pApp, Window* win, f32 timeStep) {

    //...
}


static void redraw_callback(void* pApp, Window* win) {

    //...
}


Application* new_application(Error* err) {

    Application* app = (Application*) calloc(1, sizeof(Application));
    if (app == NULL) {

        *err = memory_error();
        return NULL;
    }

    return app;
}


void dispose_application(Application* app) {

    if (app == NULL)
        return;

    m_free(app);
}


void app_register_callbacks(Application* app, Window* win) {

    window_register_callbacks(win, update_callback, redraw_callback, (void*) app);
}
