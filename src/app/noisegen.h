#ifndef NONEMPTY_SPACE_APP_NOISEGEN_H
#define NONEMPTY_SPACE_APP_NOISEGEN_H


#include "common/vector.h"

#include "bitmap.h"


Bitmap* generate_gaussian_noise_bitmap(u16 width, u16 height,
    f32 min, f32 max, i32 blurAmount, Vector4 baseColor, 
    u32 seed, Error* err);


Bitmap* generate_gaussian_noise_bitmap_no_params(u16 width, u16 height,
    f32 min, f32 max, i32 blurAmount, Vector4 baseColor);


#endif // NONEMPTY_SPACE_APP_NOISEGEN_H
