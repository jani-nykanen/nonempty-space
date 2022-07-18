#include "bitmap.h"

#include "common/error.h"
#include "common/memory.h"

#include <stdlib.h>


Bitmap* new_bitmap(u16 width, u16 height, Error* err) {

    Bitmap* bmp = (Bitmap*) calloc(1, sizeof(Bitmap));
    if (bmp == NULL) {

        *err = memory_error();
        return NULL;
    }

    bmp->width = width;
    bmp->height = height;

    bmp->pixels = (u8*) calloc(width*height, sizeof(u8));
    if (bmp->pixels == NULL) {

        *err = memory_error();
        dispose_bitmap(bmp);
        return NULL;
    }

    bmp->mask = (u8*) calloc(width*height, sizeof(u8));
    if (bmp->mask == NULL) {

        *err = memory_error();
        dispose_bitmap(bmp);
        return NULL;
    }

    return bmp;
}


void dispose_bitmap(Bitmap* bmp) {

    if (bmp == NULL)
        return;

    m_free(bmp->pixels);
    m_free(bmp->mask);
    m_free(bmp);
}
