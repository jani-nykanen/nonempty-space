#include "mesh.h"

#include "common/memory.h"

#include <stdlib.h>
#include <string.h>


Mesh* new_mesh(
    const f32* vertices, const f32* uvs, const f32* normals, const bool* outlines, const u16* indices,
    u32 vertexCount,  u32 uvCount, u32 normalCount,  u32 outlineCount,  u32 indexCount,
    Error* err) {

    Mesh* mesh = (Mesh*) calloc(1, sizeof(Mesh));
    if (mesh == NULL) {

        *err = memory_error();
        return NULL;
    }

    if ((mesh->vertices = (f32*) calloc(vertexCount, sizeof(f32))) == NULL ||
        (mesh->uvs = (f32*) calloc(uvCount, sizeof(f32))) == NULL ||
        (mesh->normals = (f32*) calloc(normalCount, sizeof(f32))) == NULL ||
        (mesh->indices = (u16*) calloc(indexCount, sizeof(u16))) == NULL ||
        (mesh->outlines = (bool*) calloc(outlineCount, sizeof(bool))) == NULL ) {

        *err = memory_error();
        dispose_mesh(mesh);

        return NULL;
    }

    mesh->vertexCount = vertexCount;
    mesh->uvCount = uvCount;
    mesh->normalCount = normalCount;
    mesh->outlineCount = outlineCount;
    mesh->indexCount = indexCount;

    memcpy(mesh->vertices, vertices, sizeof(f32) * vertexCount);
    memcpy(mesh->uvs, uvs, sizeof(f32) * uvCount);
    memcpy(mesh->normals, normals, sizeof(f32) * normalCount);
    memcpy(mesh->indices, indices, sizeof(u16) * indexCount);
    memcpy(mesh->outlines, outlines, sizeof(bool) * outlineCount);

    return mesh;
}


void dispose_mesh(Mesh* mesh) {

    if (mesh == NULL) return;

    m_free(mesh->vertices);
    m_free(mesh->uvs);
    m_free(mesh->normals);
    m_free(mesh->indices);
    m_free(mesh->outlines);

    m_free(mesh);
}
