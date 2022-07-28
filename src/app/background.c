#include "background.h"
#include "memory.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


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

    const u8 INNER_COLOR = 255;
    const u8 OUTER_COLOR = 147;

    // TODO: Check if in the proper range

    i32 dx, dy;
    i32 r;
    i32 w = (i32) dest->width;
    u8 color;

    for (dy = y - radius; dy <= y + radius; ++ dy) {

        r = radius - abs(y - dy);
        for (dx = x - r; dx <= x + r; ++ dx) {

            color = INNER_COLOR;
            if (radius > 0 && abs(dx - x) + abs(dy - y) >= radius)
                color = OUTER_COLOR;

            dest->pixels[dy * w + dx] = color;
        }
    }
}


static void generate_stars(Bitmap* dest, i32 starCount) {

    const i32 OFFSET = 8;
    const i32 RADIUS_PROB[] = {50, 90, 100};

    i32 w = (i32) dest->width;
    i32 h = (i32) dest->height;

    i32 xoff = OFFSET;
    i32 yoff = OFFSET;
    i32 rangeW = w - xoff;
    i32 rangeH = h - yoff;

    i32 i, j;
    i32 dx, dy;
    i32 p;
    i32 radius = 0;

    for (i = 0; i < starCount; ++ i) {

        dx = xoff + (rand() % rangeW);
        dy = yoff + (rand() % rangeH);

        p = (rand() % 100) + 1;
        for (j = 0; j < 3; ++ j) {

            if (p < RADIUS_PROB[j]) {

                radius = j;
                break;
            }
        }
        draw_star(dest, dx, dy, radius);
    }
}


static void draw_moon(Bitmap* dest, i32 cx, i32 cy, i32 radius) {

    const u8 INNER_COLOR = 254;
    const u8 OUTER_COLOR = 213;
    const i32 OUTER_BORDER = 2;

    i32 dx, dy;

    i32 scx = cx - radius/2;
    i32 scy = cy - radius/2;
    i32 sdist;

    u8 color;

    for (dy = cy - radius; dy <= cy + radius; ++ dy) {

        for (dx = cx - radius; dx <= cx + radius; ++ dx) {

            // Check if outside the first circle
            if (hypotf((f32) (dx - cx), (f32) (dy - cy)) >= radius)
                continue;

            // ...and if inside the other circle
            if (hypotf((f32) (dx - scx), (f32) (dy - scy)) < radius)
                continue;

            color = INNER_COLOR;

            sdist = (i32) hypotf((f32) (dx - (cx - OUTER_BORDER)), (f32) (dy - (cy - OUTER_BORDER)));
            if (sdist >= radius)
                color = OUTER_COLOR;

            dest->pixels[dy * ((i32) dest->width) + dx] = color;
        }
    }
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

    out = new_bitmap(width, height, err);
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
    i32 starCount, i32 moonX, i32 moonY, i32 moonRadius,
    Error* err) {

    Bitmap* out;
    i32 w = (i32) width;
    i32 h = (i32) height;

    out = new_bitmap(width, height, err);
    if (out == NULL) {

        return NULL;
    }

    fill_sky(out, w, h, colors, colorListLength, colorTransitionHeights);

    srand(seed);
    generate_stars(out, starCount);
    draw_moon(out, moonX, moonY, moonRadius);

    return out;
}
