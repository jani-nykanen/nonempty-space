#include "floor.h"
#include "mathext.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>


#define PRECISION 12


void draw_floor_3D(Canvas* canvas, Bitmap* texture,
    Transformations* transf, f32 startDepth, f32 endDepth,
    f32 xdif, f32 height, f32 zshift,
    bool enableFog, f32 fogDensity) {

    // TODO: Split to multiple functions for clarity

    // Notes: we use floating points to compute depth, but fixed point
    // for drawing the "scanlines". Reason: floating point operations are
    // only done once par "scanline", but fixed point operations once
    // per pixel.

    const f32 NEAR = 0.025f;

    Vector4 A = transf_apply_to_vector(transf, vec3(-xdif, height, startDepth));
    Vector4 B = transf_apply_to_vector(transf, vec3(xdif, height, endDepth));

    i32 ystart, yend;
    i32 xstart, xend;
    i32 ixstart;
    i32 ixdif;

    i32 dx, dy;
    i32 stepx, stepy;

    i32 tx, tstepx;
    i32 itx, ity;
    i32 texh;

    f32 depth;
    f32 d, depthStep;

    f32 fog;
    i32 hue;

    if (endDepth < startDepth || A.z < NEAR) 
        return;

    A.x /= A.z;
    A.y /= A.z;

    B.x /= B.z;
    B.y /= B.z;

    A.x = (A.x + 1.0f) / 2.0f;
    A.y = (A.y + 1.0f) / 2.0f;

    B.x = (B.x + 1.0f) / 2.0f;
    B.y = (B.y + 1.0f) / 2.0f;

    xstart = (i32) (A.x * canvas->width);
    xend = canvas->width/2 - ((i32) (B.x * canvas->width) - canvas->width/2);

    ystart = (i32) (A.y * canvas->height);
    yend = (i32) (B.y * canvas->height);

    if (ystart == yend) return;

    stepx = abs(xend - xstart) << PRECISION;
    stepx /= (ystart - yend);
    stepy = yend > ystart ? 1 : -1;

    xstart <<= PRECISION;

    tstepx = texture->width << PRECISION;
    texh = (i32) (fabsf(B.z - A.z) / (xdif*2) * texture->height);
 
    depthStep = (B.z - A.z) / ((f32) (yend - ystart));
    d = B.z;

    for (dy = ystart; dy != yend; dy += stepy,
        xstart += stepx,
        d += depthStep) {

        if ((stepy > 0 && dy < 0) ||
            (stepy < 0 && dy >= canvas->height))
            continue;

        if ((stepy < 0 && dy < 0) ||
            (stepy > 0 && dy >= canvas->height))
            break;

        ixstart = xstart >> PRECISION;

        ixdif = (canvas->width/2 - ixstart);
        if (ixdif == 0)
            break;;

        tstepx = texture->width << PRECISION;
        tstepx /= 2 * ixdif;
        if (ixstart < 0) {

            tx += (-ixstart) * tstepx;
        }

        // Compute texture y coordinate
        depth = (d / B.z) / ((1.0f - d) / A.z + d / B.z);
        ity = (i32) neg_mod_i32(roundf(depth * texh + zshift * texture->height), texture->height);  

        // Compute fog
        fog = depth * fogDensity;
        fog = 1.0f - clamp_f32(1.0f / expf(fog*fog), 0.0f, 1.0f);
        hue = (i32) (fog * (TINT_MAX-1)*2);

        tx = -ixstart * tstepx;
        for (dx = 0; dx < canvas->width; ++ dx, tx += tstepx) {

            itx = neg_mod_i32(tx >> PRECISION, texture->width);

            canvas->pixels[dy * canvas->width + dx] = 
                canvas->lookup->tintBlack[canvas->lookup->dither[hue][dx % 2 == dy % 2]] 
                    [texture->pixels[ity * texture->width + itx]];
        }
    }

}
