#include "core/window.h"

#include "common/memory.h"

#include "glad/gl.h"

#include "renderer.h"
#include "keyboard.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>


typedef struct {

    GLFWwindow* window;
    Renderer* renderer;
    Keyboard keyboard;

    UpdateCallback updateCb;
    RedrawCallback redrawCb;
    void* param;

    bool running;

    // For fullscreen
    bool fullscreen;
    i32 oldx;
    i32 oldy;
    i32 oldw;
    i32 oldh;

} _Window;

static _Window* winRef = NULL;


static void glfw_window_size_callback(GLFWwindow* window, i32 width, i32 height) {

    renderer_resize_event(winRef->renderer, width, height);
}


void glfw_keyboard_callback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods)
{
    if (action == GLFW_PRESS) {

        keyboard_key_pressed_event(&winRef->keyboard, (KeyCode) key);
    }
    else if (action == GLFW_RELEASE) {

        keyboard_key_released_event(&winRef->keyboard, (KeyCode) key);
    }
}


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

    f64 deltaTime = glfwGetTime();
    f64 timeStep = deltaTime / TICK;

    glfwSetTime(0.0);

    if (glfwWindowShouldClose(win->window)) {

        win->running = false;
        return;
    }

    if (win->updateCb != NULL) {

        win->updateCb(win->param, (Window*) win, timeStep);
    }
    keyboard_update(&win->keyboard);

    if (win->redrawCb != NULL) {

        win->redrawCb(win->param, (Window*) win);
    }
    renderer_refresh(win->renderer);

    glfwPollEvents();
}


Window* new_window(u16 canvasWidth, u16 canvasHeight, 
    const str caption, bool startFullscreen, Error* err) {

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

    win->keyboard = create_keyboard_manager();

    winRef = win;
    glfwSetWindowSizeCallback(win->window, glfw_window_size_callback);
    glfwSetKeyCallback(win->window, glfw_keyboard_callback);

    win->updateCb = NULL;
    win->redrawCb = NULL;
    win->param = NULL;

    win->fullscreen = false;
    win->oldx = 0;
    win->oldy = 0;
    win->oldw = 0;
    win->oldh = 0;

    if (startFullscreen) {

        window_toggle_fullscreen((Window*) win);
    }

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
    win->running = true;

    while (win->running) {

        main_loop(win);
    }
}


void window_toggle_fullscreen(Window* _win) {

    _Window* win = (_Window*) _win;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    win->fullscreen = !win->fullscreen;

    if (win->fullscreen) {

        glfwGetWindowPos(win->window, &win->oldx, &win->oldy);
        glfwGetWindowSize(win->window, &win->oldw, &win->oldh);

        glfwSetWindowMonitor(win->window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else {

        glfwSetWindowMonitor(win->window, NULL, win->oldx, win->oldy, win->oldw, win->oldh, 0);
    }
}


void window_terminate(Window* win) {

    ((_Window*) win)->running = false;
}


void window_get_canvas_size(Window* _win, i32* width, i32* height) {

    _Window* win = (_Window*) _win;

    *width = win->renderer->canvasWidth;
    *height = win->renderer->canvasHeight;
}


InputState window_get_key_state(Window* win, KeyCode key) {

    return keyboard_get_key_state(&((_Window*) win)->keyboard, key);
}
