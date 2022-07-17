#include "core/window.h"

#include "common/memory.h"

#include "glad/gl.h"

#include "renderer.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>


typedef struct {

    GLFWwindow* window;
    Renderer* renderer;

    UpdateCallback updateCb;
    RedrawCallback redrawCb;
    void* param;

    f64 oldTime;
    bool running;

} _Window;


static i32 create_glfw_window(_Window* win, i32 width, i32 height, const str caption, Error* err) {

    glfwWindowHint(GLFW_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_VERSION_MINOR, 1);

    win->window = glfwCreateWindow(width, height, caption, NULL, NULL);
    if (win->window == NULL) {

        *err = create_error_no_param("Failed to create a GLFW window!");
        return 1;
    }
    glfwMakeContextCurrent(win->window);

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        
        *err = create_error_no_param("Failed to initialize Glad OpenGL loader!");
        return 1;
    }
    return 0;
}


static void main_loop(_Window* win) {

    const f32 TICK = 1.0f / 60.0f;

    f32 deltaTime, timeStep;

    f64 newTime = glfwGetTime();

    deltaTime = (f32) (newTime - win->oldTime);
    timeStep = deltaTime / TICK;

    if (glfwWindowShouldClose(win->window)) {

        win->running = false;
        return;
    }

    if (win->updateCb != NULL) {

        win->updateCb(win->param, (Window*) win, timeStep);
    }

    if (win->redrawCb != NULL) {

        win->redrawCb(win->param, (Window*) win);
    }
    renderer_refresh(win->renderer);

    glfwPollEvents();
}


Window* new_window(u16 canvasWidth, u16 canvasHeight, const str caption, Error* err) {

    i32 winWidth, winHeight;

    _Window* win = (_Window*) calloc(1, sizeof(_Window));
    if (win == NULL) {

        *err = memory_error();
        return NULL;
    }

    winWidth = ((i32) canvasWidth)*2;
    winHeight = ((i32) canvasHeight)*2;

    if (create_glfw_window(win, winWidth, winHeight, caption, err) != 0) {

        dispose_window((Window*) win);
        return NULL;
    }

    win->renderer = new_renderer(canvasWidth, canvasHeight, (void*) win->window, err);
    if (win->renderer == NULL) {

        dispose_window((Window*) win);
        return NULL;
    }
    renderer_resize_event(win->renderer, winWidth, winHeight);

    win->updateCb = NULL;
    win->redrawCb = NULL;
    win->param = NULL;

    return (Window*) win;
}


void dispose_window(Window* _win) {

    _Window* win = (_Window*) _win;
    if (win == NULL)
        return;

    if (win->window != NULL) {

        glfwDestroyWindow(win->window);
    }
    m_free(win);
}


void window_register_callbacks(Window* _win,
    UpdateCallback updateCb, RedrawCallback redrawCb, 
    void* param) {

    _Window* win = (_Window*) _win;

    win->updateCb = updateCb;
    win->redrawCb = redrawCb;
    win->param = param;
}


void window_update_canvas(Window* _win, u8* data) {

    _Window* win = (_Window*) _win;

    renderer_update_canvas_texture(win->renderer, data);
}


void window_activate(Window* _win) {

    _Window* win = (_Window*) _win;

    glfwSetTime(0.0);
    win->oldTime = 0.0;
    win->running = true;

    while (win->running) {

        main_loop(win);
    }
}
