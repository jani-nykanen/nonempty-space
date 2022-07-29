#include "canvas.h"

#include "common/error.h"
#include "common/memory.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef void (*TextFunction) (Canvas* canvas, Bitmap* bmp, 
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy, i32 hue);


static bool clip_rect(Canvas* canvas, 
    i32* x, i32* y, 
    i32* w, i32* h) {

    // Left
    if (*x < canvas->clipArea.x) {

        *w -= (canvas->clipArea.x - (*x));
        *x = canvas->clipArea.x;
    }
    // Right
    if (*x+*w >= canvas->clipArea.x + canvas->clipArea.w) {

        *w -= (*x + *w) - (canvas->clipArea.x + canvas->clipArea.w);
    }

    // Top
    if (*y < canvas->clipArea.y) {

        *h -= (canvas->clipArea.y - (*y));
        *y = canvas->clipArea.y;
    }
    // Bottom
    if (*y+*h >= canvas->clipArea.y + canvas->clipArea.h) {

        *h -= (*y + *h) - (canvas->clipArea.y + canvas->clipArea.h);
    }

    return *w > 0 && *h > 0;
}


static bool clip_rect_region(Canvas* canvas, 
    i32* sx, i32* sy, i32* sw, i32* sh, 
    i32* dx, i32* dy, Flip flip) {

    i32 ow, oh;

    // Left
    ow = *sw;
    if(*dx < canvas->clipArea.x) {

        *sw -= canvas->clipArea.x - (*dx);
        if( (flip & FLIP_HORIZONTAL) == 0)
            *sx += ow-(*sw);

        *dx = canvas->clipArea.x;
    }
    // Right
    if(*dx+*sw >= canvas->clipArea.x + canvas->clipArea.w) {

         *sw -= (*dx + *sw) - (canvas->clipArea.x + canvas->clipArea.w); 
         if( (flip & FLIP_HORIZONTAL) != 0)
            *sx += ow-(*sw);
    }

    // Top
    oh = *sh;
    if(*dy < canvas->clipArea.y) {

        *sh -= canvas->clipArea.y - (*dy);
        if( (flip & FLIP_VERTICAL) == 0)
            *sy += oh-*sh;

        *dy = canvas->clipArea.y;
    }
    // Bottom
    if(*dy+*sh >= canvas->clipArea.y + canvas->clipArea.h) {

        *sh -= (*dy + *sh) - (canvas->clipArea.y + canvas->clipArea.h);
        if( (flip & FLIP_VERTICAL) != 0)
            *sy += oh-(*sh);
    }

    return *sw > 0 && *sh > 0;
}


// For text rendering
static void textfunc_draw_bitmap_region_fast(Canvas* canvas, Bitmap* bmp,
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy, i32 hue) {

    canvas_draw_bitmap_region_fast(canvas, bmp, sx, sy, sw, sh, dx, dy);
}


static void textfunc_draw_bitmap_region_no_flip(Canvas* canvas, Bitmap* bmp,
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy, i32 hue) {

    canvas_draw_bitmap_region(canvas, bmp, sx, sy, sw, sh, dx, dy, FLIP_NONE);
}


static void textfunc_blend_bitmap_region_no_flip(Canvas* canvas, Bitmap* bmp,
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy, i32 hue) {

    canvas_blend_bitmap_region(canvas, bmp, sx, sy, sw, sh, dx, dy, hue, FLIP_NONE);
}


static void draw_text_generic(Canvas* canvas, Bitmap* bmp, str text,
    u32 start, u32 end, i32 x, i32 y, i32 xoff, i32 yoff, Align textAlign, i32 hue,
    TextFunction func) {

    i32 dx, dy; 
    i32 sx, sy; 
 
    i32 charw = ((i32) bmp->width) / 16; 
    i32 charh = charw; 
    i32 chr; 
    
    u32 len;
    u32 i; 

    if (end > (u32) strlen(text))
        end = (u32) strlen(text);

    if (start >= end)
        return;

    len = end - start;

    switch (textAlign) { 
     
    case ALIGN_RIGHT: 
        x -= ((i32) len) * (charw + xoff); 
        break; 
 
    case ALIGN_CENTER: 
        x -= ((i32) len) * (charw + xoff) / 2; 
        break; 
     
    default: 
        break; 
    } 
 
    dx = x; 
    dy = y; 
    
    for (i = start; i < end; ++ i) {

        chr = (i32) text[i];

        if (chr == '\n') { 
 
            dy += charh + yoff; 
            dx = x; 
            continue; 
        } 
 
        sx = chr % 16; 
        sy = chr / 16; 

        func(canvas, bmp, sx*charw, sy*charh, charw, charh, dx, dy, hue);

        dx += charw + xoff; 
    }
}



Canvas* new_canvas(i32 width, i32 height, bool createMask, LookUpTables* lookup, Error* err) {
    
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

    if (createMask) {

        canvas->mask = (u8*) calloc(width*height, sizeof(u8));
        if (canvas->mask == NULL) {

            *err = memory_error();
            dispose_canvas(canvas);
            return NULL;
        }
    }

    canvas->clipArea = rect(0, 0, width, height);

    canvas->lookup = lookup;

    return canvas;
}


void dispose_canvas(Canvas* canvas) {

    if (canvas == NULL)
        return;

    m_free(canvas->pixels);
    m_free(canvas->mask);
    m_free(canvas);
}


void canvas_update_window_content(Canvas* canvas, Window* win) {

    window_update_canvas(win, canvas->pixels);
}


void canvas_clear(Canvas* canvas, u8 color) {

    memset(canvas->pixels, color, canvas->width*canvas->height);
}


void canvas_clear_mask(Canvas* canvas) {

    if (canvas->mask == NULL)
        return;

    memset(canvas->mask, 0, canvas->width*canvas->height);
}


void canvas_fill_rect(Canvas* canvas, i32 dx, i32 dy, i32 dw, i32 dh, u8 color) {

    size_t dest;
    i32 y;

    if (!clip_rect(canvas, &dx, &dy, &dw, &dh))
        return;

    dest = dy * canvas->width + dx;

    for (y = 0; y < dh; ++ y) {
        
        memset(canvas->pixels + dest, color, dw);
        dest += (size_t) canvas->width;
    }
}


void canvas_draw_bitmap_region_fast(Canvas* canvas, Bitmap* bmp,
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy)  {

    size_t dest;
    u32 src;
    i32 y;

    if (!clip_rect_region(canvas, &sx, &sy, &sw, &sh, &dx, &dy, false))
        return;

    dest = dy * ((i32) canvas->width) + dx;
    src = sy * ((i32) bmp->width) + sx;

    for (y = 0; y < sh; ++ y) {
        
        memcpy(canvas->pixels + dest, bmp->pixels + src, sw);
        src += (u32) bmp->width;
        dest += (size_t) canvas->width;
    }
}


void canvas_draw_bitmap_fast(Canvas* canvas, Bitmap* bmp, i32 dx, i32 dy) {
    
    canvas_draw_bitmap_region_fast(canvas, bmp,
        0, 0, (i32) bmp->width, (i32) bmp->height, dx, dy);
}


void canvas_draw_text_fast(Canvas* canvas, Bitmap* bmp,
    str text, i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align) {

    draw_text_generic(canvas, bmp, text, 
        0, (u32) strlen(text), x, y, xoff, yoff, align, 0,
        textfunc_draw_bitmap_region_fast);
}


void canvas_draw_text_substring_fast(Canvas* canvas, Bitmap* bmp,
    str text, u32 start, u32 end, 
    i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align) {

    draw_text_generic(canvas, bmp, text, 
        start, end, x, y, xoff, yoff, align, 0,
        textfunc_draw_bitmap_region_fast);
}


void canvas_draw_bitmap_region(Canvas* canvas, Bitmap* bmp,
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy, Flip flip) {

    u32 dest;
    u32 src;
    i32 x, y;
    i32 dirx = (flip & FLIP_HORIZONTAL) == 0 ? 1 : -1;
    i32 diry = (flip & FLIP_VERTICAL) == 0 ? 1 : -1;

    if (bmp->mask == NULL && flip == FLIP_NONE) {

        canvas_draw_bitmap_region_fast(canvas, bmp, sx, sy, sw, sh, dx, dy);
        return;
    }

    if (!clip_rect_region(canvas, &sx, &sy, &sw, &sh, &dx, &dy, flip))
        return;

    src = sy * ((i32) bmp->width) + sx;
    if ((flip & FLIP_HORIZONTAL) != 0)
        src += sw - 1;

    dest = dy * ((i32) canvas->width) + dx;
    if ((flip & FLIP_VERTICAL) != 0)
        dest += ( (i32) canvas->width) * (sh - 1) + sw - 1;
    
    // Having `if` statement here, not inside the for loop, is
    // significantly faster than having it in the for loop, even
    // though that would be less code.
    if (bmp->mask != NULL) {

        for (y = 0; y < sh; ++ y) {

            for (x = 0; x < sw; ++ x) {

                canvas->pixels[dest] = (bmp->pixels[src] & bmp->mask[src]) | 
                            (canvas->pixels[dest] & (~bmp->mask[src]));

                dest += diry;
                src += dirx;
            }

             dest += (canvas->width - sw) * diry;
            src += bmp->width - sw * dirx;
        }
    }
    else {

        for (y = 0; y < sh; ++ y) {

            for (x = 0; x < sw; ++ x) {

                canvas->pixels[dest] = bmp->pixels[src];

                dest += diry;
                src += dirx;
            }

            dest += (canvas->width - sw) * diry;
            src += bmp->width - sw * dirx;
        }
    }
}


void canvas_draw_bitmap(Canvas* canvas, Bitmap* bmp, 
    i32 dx, i32 dy, Flip flip) {

    canvas_draw_bitmap_region(canvas, bmp,
        0, 0, (i32) bmp->width, (i32) bmp->height, dx, dy, flip);
}


void canvas_draw_text(Canvas* canvas, Bitmap* bmp,
    str text, i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align) {

    draw_text_generic(canvas, bmp, text, 
        0, (u32) strlen(text), x, y, xoff, yoff, align, 0,
        textfunc_draw_bitmap_region_no_flip);
}


void canvas_draw_text_substring(Canvas* canvas, Bitmap* bmp,
    str text, u32 start, u32 end, 
    i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align) {

    draw_text_generic(canvas, bmp, text, 
        start, end, x, y, xoff, yoff, align, 0,
        textfunc_draw_bitmap_region_no_flip);
}


void canvas_blend_bitmap_region(Canvas* canvas, Bitmap* bmp,
    i32 sx, i32 sy, i32 sw, i32 sh, i32 dx, i32 dy, i32 hue, Flip flip) {

    // TODO: Repeating code, write a general method to use in both
    // ordinary draw and this blend function, and then pass a callback
    // function for pixel operation

    u32 dest;
    u32 src;
    i32 x, y;
    i32 dirx = (flip & FLIP_HORIZONTAL) == 0 ? 1 : -1;
    i32 diry = (flip & FLIP_VERTICAL) == 0 ? 1 : -1;

    u8 background, target, mask;
    u8 (*tint) [TINT_MAX][256];

    if (bmp->mask == NULL) 
        return;

    if (!clip_rect_region(canvas, &sx, &sy, &sw, &sh, &dx, &dy, flip))
        return;

    tint = &canvas->lookup->tintWhite;
    if (hue < 0) {

        hue *= -1;
        tint = &canvas->lookup->tintBlack;
    }

    src = sy * ((i32) bmp->width) + sx;
    if ((flip & FLIP_HORIZONTAL) != 0)
        src += sw - 1;

    dest = dy * ((i32) canvas->width) + dx;
    if ((flip & FLIP_VERTICAL) != 0)
        dest += ( (i32) canvas->width) * (sh - 1) + sw - 1;

    for (y = 0; y < sh; ++ y) {

        for (x = 0; x < sw; ++ x) {

            mask = bmp->mask[src];
            background = canvas->pixels[dest];
            target = (*tint)[canvas->lookup->dither[hue][x % 2 == y % 2]][background];

            canvas->pixels[dest] = (target & mask) | (background & (~mask));

            dest += diry;
            src += dirx;
        }

        dest += (canvas->width - sw) * diry;
        src += bmp->width - sw * dirx;
    }
}


void canvas_blend_text(Canvas* canvas, Bitmap* bmp,
    str text, i32 x, i32 y, 
    i32 xoff, i32 yoff, Align align, i32 hue) {

    draw_text_generic(canvas, bmp, text, 
        0, (u32) strlen(text), x, y, xoff, yoff, align, hue,
        textfunc_blend_bitmap_region_no_flip);
}


void canvas_draw_line(Canvas* canvas, 
    i32 x1, i32 y1, i32 x2, i32 y2, u8 color) {
    
    // Bresenham's line algorithm
    i32 dx = abs(x2-x1);
    i32 sx = x1 < x2 ? 1 : -1;
    i32 dy = (i32) abs(y2 - y1);
    i32 sy = y1 < y2 ? 1 : -1; 
    i32 err = (dx > dy ? dx : -dy) / 2;
    i32 e2;
    
    while(true) {

        if (!(y1 >= canvas->clipArea.h || y1 < canvas->clipArea.x ||
            x1 >= canvas->clipArea.w || x1 < canvas->clipArea.y )) {

            canvas->pixels[y1*canvas->width + x1] = color;
        }
        
        if (x1 == x2 && y1 == y2) 
            break;

        e2 = err;
        if (e2 > -dx) { 

            err -= dy; 
            x1 += sx; 
        }
        if (e2 < dy) { 

            err += dx; 
            y1 += sy; 
        }
    }    
}
