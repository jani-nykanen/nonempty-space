#include "font.h"

#include <stdlib.h>


#include "fontsrc.h"



Bitmap* create_debug_font(Error* err) {

    i32 i, j;
    u16 shift;
    u8 mask;

    Bitmap* bmp = new_bitmap(IMAGE_WIDTH, IMAGE_HEIGHT, false);
    if (bmp == NULL) {

        return NULL;
    }

    j = 0;
    for (i = 0; i < IMAGE_WIDTH*IMAGE_HEIGHT; ++ i) {

        shift = (7 - (i % 8));
        mask = 1 << shift;

        bmp->pixels[i] = 255 * ((IMAGE_DATA[j] & mask) >> shift);
        bmp->mask[i] = bmp->pixels[i];

        if (i % 8 == 7) 
            ++ j;
    }

    return bmp;
}
