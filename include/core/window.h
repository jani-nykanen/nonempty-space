#ifndef NONEMPTY_SPACE_CORE_WINDOW_H
#define NONEMPTY_SPACE_CORE_WINDOW_H


#include "common/types.h"
#include "common/error.h"


EMPTY_STRUCT(Window);


typedef void (*UpdateCallback) (void*, Window*, f32);
typedef void (*RedrawCallback) (void*, Window*);


Window* new_window(u16 canvasWidth, u16 canvasHeight, const str caption, Error* err);
void dispose_window(Window* win);

void window_register_callbacks(Window* win,
    UpdateCallback updateCb, RedrawCallback redrawCb, 
    void* param);
void window_update_canvas(Window* win, u8* data);
void window_activate(Window* win);


#endif // NONEMPTY_SPACE_CORE_WINDOW_H
