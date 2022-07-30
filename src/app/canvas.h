#ifndef NONEMPTY_SPACE_APP_CANVAS_H
#define NONEMPTY_SPACE_APP_CANVAS_H


#include "common/error.h"
#include "common/rectangle.h"

#include "core/window.h"

#include "bitmap.h"
#include "lookup.h"


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

    IMPLEMENTS_BITMAP

    Rectangle clipArea;
    LookUpTables* lookup;

} Canvas;


Canvas* new_canvas(i32 width, i32 height, bool createMask, LookUpTables* lookup, Error* err);
void dispose_canvas(Canvas* canvas);

void canvas_update_window_content(Canvas* canvas, Window* win);

void canvas_clear(Canvas* canvas, u8 color);
void canvas_clear_mask(Canvas* canvas);

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

void canvas_blend_bitmap_region(Canvas* canvas, Bitmap* bmp,
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy, i32 hue, Flip flip);
void canvas_blend_text(Canvas* canvas, Bitmap* bmp,
    str text, i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align, i32 hue);  

void canvas_draw_vertically_waving_bitmap(Canvas* canvas, Bitmap* bmp,
    i32 dx, i32 dy, f32 wave, f32 amplitude, f32 latitude, f32 period);

void canvas_draw_line(Canvas* canvas, 
    i32 x1, i32 y1, i32 x2, i32 y2, u8 color);
    

#endif // NONEMPTY_SPACE_APP_CANVAS_H
