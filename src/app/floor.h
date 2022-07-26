#ifndef NONEMPTY_SPACE_APP_FLOOR_H
#define NONEMPTY_SPACE_APP_FLOOR_H


#include "canvas.h"
#include "transformations.h"


// Why this is in a separate file?
// Canvas should not depend on transformations, but this does
// not really with triangle rasterizer, so it's here.


void draw_floor_3D(Canvas* canvas, Bitmap* texture,
    Transformations* transf, 
    f32 startDepth, f32 endDepth,
    f32 xdif, f32 height);


#endif // NONEMPTY_SPACE_APP_FLOOR_H
