#ifndef NONEMPTY_SPACE_APP_CANVAS_H
#define NONEMPTY_SPACE_APP_CANVAS_H


#include "common/error.h"

#include "core/window.h"


typedef struct {

    u16 width;
    u16 height;
    u8* pixels;

} Canvas;


Canvas* new_canvas(i32 width, i32 height, Error* err);
void dispose_canvas(Canvas* canvas);

void canvas_update_window_content(Canvas* canvas, Window* win);

void canvas_clear(Canvas* canvas, u8 color);


#endif // NONEMPTY_SPACE_APP_CANVAS_H
