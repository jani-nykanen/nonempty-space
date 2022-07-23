#ifndef NONEMPTY_SPACE_APP_3D_RENDERER_H
#define NONEMPTY_SPACE_APP_3D_RENDERER_H


#include "trianglebuffer.h"
#include "transformations.h"
#include "mesh.h"


typedef struct {

    TriangleBuffer* buffer;

    Vector4 lightDir;
    f32 lightMag;
    bool lightingEnabled;

} Renderer3D;


Renderer3D create_renderer_3D(TriangleBuffer* buffer);

void r3d_draw_triangle(Renderer3D* r3d,
    Transformations* transf,
    Bitmap* texture, u8 color, i32 tint,
    Vector4 A, Vector4 B, Vector4 C, 
    Vector4 tA, Vector4 tB, Vector4 tC,
    Vector4 normal);
void r3d_draw_mesh(Renderer3D* r3d, Transformations* transf, 
    Mesh* mesh, Bitmap* texture, u8 color);


#endif // NONEMPTY_SPACE_APP_3D_RENDERER_H
