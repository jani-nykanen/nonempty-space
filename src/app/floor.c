#include "floor.h"
#include "mathext.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>


#define PRECISION 12


void draw_floor_3D(Canvas* canvas, Bitmap* texture,
    Transformations* transf, f32 startDepth, f32 endDepth,
    f32 xdif, f32 height) {

    const f32 NEAR = 0.025f;

    Vector4 A = vec3(-xdif, height, startDepth);
    Vector4 B = vec3(xdif, height, endDepth);

    i32 ystart, yend;
    i32 xstart, xend;
    i32 ixstart;

    i32 dx, dy;
    i32 stepx, stepy;

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
    xend = (i32) (B.x * canvas->width);

    ystart = (i32) (A.y * canvas->height);
    yend = (i32) (B.y * canvas->height);

    if (ystart == yend) return;

    stepx = abs(xend - xstart) << PRECISION;
    stepx /= (ystart - yend);
    stepy = yend > ystart ? 1 : -1;

    xstart <<= PRECISION;
    xend <<= PRECISION;

    for (dy = ystart; dy != yend; dy += stepy,
        xstart += stepx) {

        if ((stepy > 0 && dy < 0) ||
            (stepy < 0 && dy >= canvas->height))
            continue;

        if ((stepy < 0 && dy < 0) ||
            (stepy > 0 && dy >= canvas->height))
            break;

        ixstart = xstart >> PRECISION;

        for (dx = max_i32(0, ixstart); dx < min_i32(canvas->width, canvas->width/2 + (canvas->width/2 - ixstart)); ++ dx) {

            canvas->pixels[dy * canvas->width + dx] = 255;
        }
    }

}
