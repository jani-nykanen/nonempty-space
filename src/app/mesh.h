#ifndef NONEMPTY_SPACE_APP_MESH_H
#define NONEMPTY_SPACE_APP_MESH_H


#include "common/types.h"
#include "common/error.h"


typedef struct {

    f32*  vertices;
    f32*  uvs;
    f32*  normals;
    u16*  indices;
    bool* outlines;

    u32 vertexCount;
    u32 uvCount;
    u32 normalCount;
    u32 indexCount;
    u32 outlineCount;

} Mesh;


Mesh* new_mesh(
    const f32* vertices, const f32* uvs, const f32* normals, const bool* outlines, const u16* indices,
    u32 vertexCount,  u32 uvCount, u32 normalCount, u32 outlineCount, u32 indexCount, 
    Error* err);
void dispose_mesh(Mesh* mesh);


#endif // NONEMPTY_SPACE_APP_MESH_H
