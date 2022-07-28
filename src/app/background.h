#ifndef NONEMPTY_SPACE_APP_BACKGROUND_H
#define NONEMPTY_SPACE_APP_BACKGROUND_H


#include "bitmap.h"


Bitmap* generate_forest_background(
    u16 width, u16 height, u32 seed, 
    u8 color, Error* err);

Bitmap* generate_starry_sky(
    u16 width, u16 height, u32 seed,
    u8* colors, u32 colorListLength, i32* colorTransitionHeights,
    i32 starCount,
    Error* err);


#endif // NONEMPTY_SPACE_APP_BACKGROUND_H
