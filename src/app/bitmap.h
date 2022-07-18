#ifndef NONEMPTY_SPACE_APP_BITMAP_H
#define NONEMPTY_SPACE_APP_BITMAP_H


#include "common/types.h"
#include "common/error.h"


#define IMPLEMENTS_BITMAP \
    u16 width; \
    u16 height; \
    u8* pixels; \
    u8* mask; \


typedef struct {

    IMPLEMENTS_BITMAP

} Bitmap;


Bitmap* new_bitmap(u16 width, u16 height, Error* err);
void dispose_bitmap(Bitmap* bmp);


#endif // NONEMPTY_SPACE_APP_BITMAP_H
