#include "noisegen.h"
#include "mathext.h"

#include "common/memory.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>


static void box_muller(f32* z1, f32* z2) {

    f32 u1 = ((f32)( rand() % 10000)) / 10000.0f;
    f32 u2 = ((f32)( rand() % 10000)) / 10000.0f;

    *z1 = sqrtf(-2 * logf(u1)) * cosf(2 * M_PI * u2);
    *z2 = sqrtf(-2 * logf(u1)) * sinf(2 * M_PI * u2);
}


static void fill_with_gaussian_noise(Vector4* out, u16 width, u16 height, 
    f32 min, f32 max, Vector4 baseColor) {

    u16 i, j;
    u32 k = 0;

    // Initialize values to avoid warnings
    f32 z1 = 0.0f;
    f32 z2 = 0.0f;
    f32 z = 0.0f;

    for (i = 0; i < width; ++ i) {

        for (j = 0; j < height; ++ j) {

            if (k % 2 == 0) {

                box_muller(&z1, &z2);
                z = z1;
            }
            else {

                z = z2;
            }

            z = clamp_f32(z / (max - min), 0.0f, 1.0f);

            out[k] = vec4_scalar_multiply(baseColor, z);
            ++ k;
        }
    }
}


static void convert_to_rgb332(Vector4* src, u8* dest, u16 width, u16 height) {
    
    u8 r, g, b;
    u32 i;
    u32 count = (u32) (width * height);
    Vector4 c;

    for (i = 0; i < count; ++ i) {

        c = src[i];

        r = (u8) clamp_f32(c.x * 255.0f, 0.0f, 255.0f);
        g = (u8) clamp_f32(c.y * 255.0f, 0.0f, 255.0f);
        b = (u8) clamp_f32(c.z * 255.0f, 0.0f, 255.0f);

        r /= 36;
        g /= 36;
        b /= 85;

        dest[i] = (r << 5) | (g << 3) | b;
    }
}


Bitmap* generate_gaussian_noise_bitmap(u16 width, u16 height,
    f32 min, f32 max, i32 blurAmount, Vector4 baseColor, 
    u32 seed, Error* err) {

    Bitmap* output;
    Vector4* raw;
    
    srand(seed);

    raw = (Vector4*) calloc(width*height, sizeof(Vector4));
    if (raw == NULL) {

        *err = memory_error();
        return NULL;
    }

    output = new_bitmap(width, height, err);
    if (output == NULL) {

        *err = memory_error();
        m_free(raw);
        return NULL;
    }

    fill_with_gaussian_noise(raw, width, height, min, max, baseColor);

    if (blurAmount > 0) {

        // TODO: Apply Gaussian blur
    }

    convert_to_rgb332(raw, output->pixels, width, height);

    m_free(raw);
    return output;
}
