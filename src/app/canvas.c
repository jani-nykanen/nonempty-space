#include "canvas.h"

#include "common/error.h"
#include "common/memory.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


Canvas* new_canvas(i32 width, i32 height, Error* err) {
    
    Canvas* canvas = (Canvas*) calloc(1, sizeof(Canvas));
    if (canvas == NULL) {

        *err = memory_error();
        return NULL;
    }

    canvas->width = (u16) width;
    canvas->height = (u16) height;

    canvas->pixels = (u8*) calloc(width*height, sizeof(u8));
    if (canvas->pixels == NULL) {

        *err = memory_error();
        dispose_canvas(canvas);
        return NULL;
    }

    return canvas;
}


void dispose_canvas(Canvas* canvas) {

    if (canvas == NULL)
        return;

    m_free(canvas->pixels);
    m_free(canvas);
}


void canvas_update_window_content(Canvas* canvas, Window* win) {

    window_update_canvas(win, canvas->pixels);
}


void canvas_clear(Canvas* canvas, u8 color) {

    memset(canvas->pixels, color, canvas->width*canvas->height);
}
