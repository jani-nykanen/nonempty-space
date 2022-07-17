#include "core/system.h"


#include <GLFW/glfw3.h>


i32 init_system(Error* err) {

    if (!glfwInit()) {

        *err = create_error_no_param("Failed to initialize GLFW!");
        return 1;
    }
    return 0;
}


void terminate_system() {

    glfwTerminate();
}
