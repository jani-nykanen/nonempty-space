#ifndef NONEMPTY_SPACE_CORE_WINDOW_H
#define NONEMPTY_SPACE_CORE_WINDOW_H


#include "common/types.h"
#include "common/error.h"

#include "keycode.h"
#include "inputstate.h"


EMPTY_STRUCT(Window);


typedef void (*UpdateCallback) (void*, Window*, f32);
typedef void (*RedrawCallback) (void*, Window*);


Window* new_window(u16 canvasWidth, u16 canvasHeight, 
    const str caption, bool startFullscreen, Error* err);
void dispose_window(Window* win);

void window_register_callbacks(Window* win,
    UpdateCallback updateCb, RedrawCallback redrawCb, 
    void* param);
void window_update_canvas(Window* win, u8* data);
void window_activate(Window* win);

void window_toggle_fullscreen(Window* win);
void window_terminate(Window* win);

void window_get_canvas_size(Window* win, i32* width, i32* height);

InputState window_get_key_state(Window* win, KeyCode key);


#endif // NONEMPTY_SPACE_CORE_WINDOW_H
