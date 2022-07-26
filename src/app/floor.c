#include "floor.h"
#include "mathext.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>


#define PRECISION 12


static const i32 UNIT = 1 << PRECISION;


static i32 correct_perspective(i32 d0, i32 d1, i32 d, i32 h, i32 shift) {

    i32 p1, p2, out;

    p1 = (UNIT - d) << PRECISION;
    p1 /= d0;

    p2 = d << PRECISION;
    p2 /= d1;

    out = d << PRECISION;
    out /= d1;

    out <<= PRECISION;
    out /= p1 + p2;

    out += shift;
    out *= h;
    out >>= PRECISION;

    return out;
}


void draw_floor_3D(Canvas* canvas, Bitmap* texture,
    Transformations* transf, f32 startDepth, f32 endDepth,
    f32 xdif, f32 height,
    f32 zshift) {

    // TODO: Split to multiple functions for clarity

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
    i32 ishift;
    
    i32 d0 = (i32) (A.z * UNIT);
    i32 d1 = (i32) (B.z * UNIT);
    i32 d, dstep;

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
    texh = (i32) (fabsf(endDepth - startDepth) / (xdif*2) * texture->height);

    ishift = (i32) (zshift * UNIT);

    dstep = abs(d0 - d1);
    dstep /= yend - ystart;
    d = d1;

    for (dy = ystart; dy != yend; dy += stepy,
        xstart += stepx,
        d += dstep) {

        if ((stepy > 0 && dy < 0) ||
            (stepy < 0 && dy >= canvas->height))
            continue;

        if ((stepy < 0 && dy < 0) ||
            (stepy > 0 && dy >= canvas->height))
            break;

        ixstart = xstart >> PRECISION;
        tx = 0;

        ixdif = (canvas->width/2 - ixstart);
        if (ixdif == 0)
            break;;

        tstepx = texture->width << PRECISION;
        tstepx /= 2 * ixdif;

        if (ixstart < 0) {

            tx += (-ixstart) * tstepx;
        }


        for (dx = max_i32(0, ixstart); 
            dx < min_i32(canvas->width, canvas->width/2 + ixdif); 
            ++ dx,
            tx += tstepx) {

            itx = neg_mod_i32(tx >> PRECISION, texture->width);
            ity = neg_mod_i32(correct_perspective(d0, d1, d, texh, ishift), texture->height);

            canvas->pixels[dy * canvas->width + dx] = texture->pixels[ity * texture->width + itx];
        }
    }

}
