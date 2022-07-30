#include "app.h"

#include "core/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {

    Window* win;
    Application* app;

} MainProgram;


static void dispose_program(MainProgram* prog) {

    if (prog->app != NULL)
        dispose_application(prog->app);

    if (prog->win != NULL)
        dispose_window(prog->win);
}


static bool check_window_flag(i32 argc, str* argv) {

    i32 i;

    for (i = 1; i < argc; ++ i) {

        if (strcmp(argv[i], "-w") == 0) {

            return true;
        }
    }
    return false;
}


i32 main(i32 argc, str* argv) {

    MainProgram prog;
    Error err = create_empty_error();

    bool fullscreen = !check_window_flag(argc, argv);

    prog.app = NULL;
    prog.win = NULL;

    if (init_system(&err) != 0) {

        fprintf(stderr, "An error occurred: %s\n", err.message);
        return 1;
    }

    prog.win = new_window(256, 192, "Nonempty Space", fullscreen, &err);
    if (prog.win == NULL) {

        fprintf(stderr, "An error occurred: %s\n", err.message);
        dispose_program(&prog);
        return 1;
    }

    prog.app = new_application(prog.win, &err);
    if (prog.app == NULL) {

        fprintf(stderr, "An error occurred: %s\n", err.message);
        dispose_program(&prog);
        return 1;
    }
    app_register_callbacks(prog.app, prog.win);

    window_activate(prog.win);
    dispose_program(&prog);
    
    terminate_system();

    return 0;
}
