#include "3d.h"


Renderer3D create_renderer_3D(TriangleBuffer* buffer, TriangleRasterizer* rasterizer) {

    Renderer3D r3d;

    r3d.buffer = buffer;
    r3d.rasterizer = rasterizer;

    return r3d;
}
