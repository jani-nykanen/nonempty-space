#include "background.h"
#include "memory.h"

#include <stdlib.h>


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
