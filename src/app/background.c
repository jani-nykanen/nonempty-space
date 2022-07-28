#include "background.h"
#include "memory.h"

#include <stdlib.h>


static void fill_sky(Bitmap* target, i32 w, i32 h,
    u8* colors, u32 colorListLength, i32* colorTransitionHeights) {

    i32 x, y;
    u32 index, step;
    u8 color1, color2;

    i32 dest;

    index = 0;
    step = 0;

    color1 = colors[0];
    color2 = colors[0];

    for (y = 0; y < h; ++ y) {

        dest = y * ((i32) target->width);
        for (x = 0; x < w; ++ x, ++ dest) {

            if (x % 2 == y % 2) {

                target->pixels[dest] = color1;
            }
            else {

                target->pixels[dest] = color2;
            }
        }

        if ((++ step) == colorTransitionHeights[index]) {

            step = 0;
            ++ index;

            if (index > (colorListLength-1)*2) {

                index = (colorListLength-1)*2;
            }

            color1 = colors[index / 2];
            color2 = colors[(index + 1) / 2];
        }
    }
}


static void draw_star(Bitmap* dest, i32 x, i32 y, i32 radius) {

    // TODO: This
}


Bitmap* generate_forest_background(
    u16 width, u16 height, u32 seed, 
    u8 color, Error* err) {  

    const i32 ERROR_PROB = 2;

    Bitmap* out;
    i32 maxSteps = ((i32) height) / 8;
    i32 dir = 0;
    i32 stepCount = 0;
    i32 i, j;
    i32 y;

    out = new_bitmap((u16) width, (u16) height, err);
    if (out == NULL) {

        return NULL;
    }

    srand(seed);
    dir = -1 + 2 * (rand() % 2);

    y = ((i32) height) / 2 + (rand() % maxSteps);
    
    memset(out->mask, 0, width*height);
    memset(out->pixels, color, width*height);

    stepCount = (rand() % maxSteps) + 1;

    for (i = 0; i < (i32) width; ++ i) {

        for (j = y; j < (i32) height; ++ j) {

            out->mask[j * ((i32) width) + i] = 255; 
        }

        y += dir;
        if (rand() % ERROR_PROB == 0)
            y += dir;

        if ((-- stepCount) == 0 || y >= ((i32) height)-1 || y <= 0) {

            if (y < 0)
                y = 0;
            else if (y >= ((i32) height))
                y = ((i32) height) -1;

            dir = -dir;
            stepCount = (rand() % maxSteps) + 1;
        }
    }
    return out;
}


Bitmap* generate_starry_sky(
    u16 width, u16 height, u32 seed,
    u8* colors, u32 colorListLength, i32* colorTransitionHeights,
    i32 starCount,
    Error* err) {

    Bitmap* out;
    i32 w = (i32) width;
    i32 h = (i32) height;

    out = new_bitmap((u16) width, (u16) height, err);
    if (out == NULL) {

        return NULL;
    }

    fill_sky(out, w, h, colors, colorListLength, colorTransitionHeights);

    return out;
}
