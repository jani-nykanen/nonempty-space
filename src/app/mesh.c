#include "mesh.h"

#include "common/memory.h"

#include <stdlib.h>
#include <string.h>


Mesh* new_mesh(const f32* vertices, const f32* uvs, const f32* normals, const u16* indices,
    u32 vertexCount, u32 uvCount, u32 normalCount, u32 indexCount, Error* err) {

    // TODO: Implement

    return NULL;
}


void dispose_mesh(Mesh* mesh) {

    if (mesh == NULL) return;

    m_free(mesh->vertices);
    m_free(mesh->uvs);
    m_free(mesh->normals);
    m_free(mesh->indices);

    m_free(mesh);
}
