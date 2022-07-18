#ifndef NONEMPTY_SPACE_APP_CANVAS_H
#define NONEMPTY_SPACE_APP_CANVAS_H


#include "common/error.h"
#include "common/rectangle.h"

#include "core/window.h"

#include "bitmap.h"


typedef enum {

    FLIP_NONE = 0,
    FLIP_HORIZONTAL = 1,
    FLIP_VERTICAL = 2,
    FLIP_BOTH = 3
} Flip;


typedef enum {

    ALIGN_LEFT = 0,
    ALIGN_CENTER = 1,
    ALIGN_RIGHT = 2

} Align;


typedef struct {

    u16 width;
    u16 height;
    u8* pixels;

    Rectangle clipArea;

} Canvas;


Canvas* new_canvas(i32 width, i32 height, Error* err);
void dispose_canvas(Canvas* canvas);

void canvas_update_window_content(Canvas* canvas, Window* win);

void canvas_clear(Canvas* canvas, u8 color);

void canvas_draw_bitmap_region_fast(Canvas* canvas, Bitmap* bmp,
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy);
void canvas_draw_bitmap_fast(Canvas* canvas, Bitmap* bmp, i32 dx, i32 dy);
void canvas_draw_text_fast(Canvas* canvas, Bitmap* bmp,
    str text, i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align);
void canvas_draw_text_substring_fast(Canvas* canvas, Bitmap* bmp,
    str text, u32 start, u32 end, 
    i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align);

void canvas_draw_bitmap_region(Canvas* canvas, Bitmap* bmp,
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy, Flip flip);
void canvas_draw_bitmap(Canvas* canvas, Bitmap* bmp, 
    i32 dx, i32 dy, Flip flip);
void canvas_draw_text(Canvas* canvas, Bitmap* bmp,
    str text, i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align);  
void canvas_draw_text_substring(Canvas* canvas, Bitmap* bmp,
    str text, u32 start, u32 end, 
    i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align); 


#endif // NONEMPTY_SPACE_APP_CANVAS_H
