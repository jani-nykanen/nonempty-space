#include "renderer.h"

#include "common/error.h"
#include "common/memory.h"

#include "glad/gl.h"

#include <stdio.h>
#include <stdlib.h>



static void init_opengl() {

    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_DEPTH_TEST);
}


static u32 create_opengl_texture(u16 width, u16 height) {

    u32 tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, NULL);

    return tex;
}


static Buffers create_rectangle_mesh_buffers() {

    const f32 VERTICES[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    const f32 UVS[] = {0, 1, 1, 1, 1, 0, 0, 0};
    const u16 INDICES[] = {0, 1, 2, 2, 3, 0};

    // TODO: The rest
}


Renderer* new_renderer(u16 canvasWidth, u16 canvasHeight, Error* err) {

    Renderer* r = (Renderer*) calloc(1, sizeof(Renderer));
    if (r == NULL) {

        *err = memory_error();
        return NULL;
    }

    init_opengl();

    r->canvasWidth = canvasWidth;
    r->canvasHeight = canvasHeight;

    r->canvasTexture = create_opengl_texture(canvasWidth, canvasHeight);
    if (r->canvasTexture == 0) {

        *err = create_error_no_param("Failed to create an OpenGL texture for canvas!");
        dispose_renderer(r);

        return NULL;
    }

    return r;
}


void dispose_renderer(Renderer* r) {

    if (r == NULL)
        return;

    if (r->canvasTexture != 0) {

        glDeleteTextures(1, &r->canvasTexture);
    }
    m_free(r);
}


void renderer_resize_event(Renderer* r, i32 width, i32 height) {

}


void renderer_update_canvas_texture(Renderer* r, u16* pixels) {

}


void renderer_refresh(Renderer* r) {

}

