#ifndef NONEMPTY_SPACE_APP_3D_RENDERER_H
#define NONEMPTY_SPACE_APP_3D_RENDERER_H


#include "trianglebuffer.h"


typedef struct {

    TriangleBuffer* buffer;
    TriangleRasterizer* rasterizer;

} Renderer3D;


Renderer3D create_renderer_3D(TriangleBuffer* buffer, TriangleRasterizer* rasterizer);


#endif // NONEMPTY_SPACE_APP_3D_RENDERER_H
