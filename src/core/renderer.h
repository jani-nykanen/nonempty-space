#ifndef NONEMPTY_SPACE_CORE_INTERNAL_RENDERER_H
#define NONEMPTY_SPACE_CORE_INTERNAL_RENDERER_H


#include "shader.h"


typedef struct {

    u32 vertexBuffer;
    u32 uvBuffer;
    u32 indexBuffer;
    u32 elementCount;

} Buffers;


typedef struct {

    Shader* shader;
    u32 canvasTexture;
    Buffers buffers;

    u16 canvasWidth;
    u16 canvasHeight;

    u32 colorTable[65536];

} Renderer;


Renderer* new_renderer(u16 canvasWidth, u16 canvasHeight, Error* err);
void dispose_renderer(Renderer* r);

void renderer_resize_event(Renderer* r, i32 width, i32 height);
void renderer_update_canvas_texture(Renderer* r, u16* pixels);
void renderer_refresh(Renderer* r);


#endif // NONEMPTY_SPACE_CORE_INTERNAL_RENDERER_H
