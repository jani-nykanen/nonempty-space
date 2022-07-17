#ifndef NONEMPTY_SPACE_APP_APP_H
#define NONEMPTY_SPACE_APP_APP_H


#include "core/window.h"


typedef struct {

    // ...

} Application;


Application* new_application(Error* err);
void dispose_application(Application* app);

void app_register_callbacks(Application* app, Window* win);


#endif // NONEMPTY_SPACE_APP_APP_H
