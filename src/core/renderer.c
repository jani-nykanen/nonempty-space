#include "renderer.h"

#include "common/error.h"
#include "common/memory.h"

#include "glad/gl.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "shadersrc.h"


static void init_opengl() {

    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_DEPTH_TEST);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}


static u32 create_opengl_texture(u16 width, u16 height) {

    u32 tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    return tex;
}


static Buffers create_rectangle_mesh_buffers() {

    const f32 VERTICES[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    const f32 UVS[] = {0, 1, 1, 1, 1, 0, 0, 0};
    const u16 INDICES[] = {0, 1, 2, 2, 3, 0};

    Buffers b;

    glGenBuffers(1, &b.vertexBuffer);
    glGenBuffers(1, &b.uvBuffer);
    glGenBuffers(1, &b.indexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, b.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 8, VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, b.uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 8, UVS, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * 6, INDICES, GL_STATIC_DRAW);

    b.elementCount = 6;

    return b;
}


static void draw_buffers(Buffers b) {

    glDrawElements(GL_TRIANGLES, b.elementCount, GL_UNSIGNED_SHORT, NULL);
}


static void compute_color_table(u32 table[256]) {

    i32 i;
    u8 c;
    u8 r, g, b;

    for (i = 0; i < 256; ++ i) {

        c = (u8) i;

        r = c >> 5;

        g = c << 3;
        g = g >> 5;

        b = c << 6;
        b = b >> 6;

        r *= 36;
        if (r >= 252)
            r = 255;

        g *= 36;
        if (g >= 252)
            g = 255;

        b *= 85;

        table[i] = (255 << 24) | ( ((i32) b) << 16) | ( ((i32) g) << 8) | ((i32) r);
    }
}


Renderer* new_renderer(u16 canvasWidth, u16 canvasHeight, void* window, Error* err) {

    Renderer* r = (Renderer*) calloc(1, sizeof(Renderer));
    if (r == NULL) {

        *err = memory_error();
        return NULL;
    }

    init_opengl();

    r->canvasWidth = (i32) canvasWidth;
    r->canvasHeight = (i32) canvasHeight;
    r->window = window;

    r->shader = new_shader((str) VERTEX_SRC, (str) FRAGMENT_SRC, err);
    if (r->shader == NULL) {

        dispose_renderer(r);
        return NULL;
    }

    r->canvasTexture = create_opengl_texture(canvasWidth, canvasHeight);
    if (r->canvasTexture == 0) {

        *err = create_error_no_param("Failed to create an OpenGL texture for canvas!");
        dispose_renderer(r);

        return NULL;
    }

    r->updateData = (u32*) calloc(canvasWidth*canvasHeight, sizeof(u32));
    if (r->updateData == NULL) {

        *err = memory_error();
        dispose_renderer(r);

        return NULL;
    }
    compute_color_table(r->colorTable);

    r->canvasMeshBuffers = create_rectangle_mesh_buffers();

    return r;
}


void dispose_renderer(Renderer* r) {

    if (r == NULL)
        return;

    if (r->canvasTexture != 0) {

        glDeleteTextures(1, &r->canvasTexture);
    }
    
    dispose_shader(r->shader);
    
    m_free(r->updateData);
    m_free(r);
}


void renderer_resize_event(Renderer* r, i32 width, i32 height) {

    // Find the best multiplier to preserve square pixels.
    // Variable naming is especially bad here I know

    f32 output[4];

    f32 fw = (f32) width;
    f32 fh = (f32) height;

    f32 fcw = (f32) r->canvasWidth;
    f32 fch = (f32) r->canvasHeight;

    f32 m = 1.0f;

    glViewport(0, 0, width, height);

    if (fw / fh >= fcw / fch) {

        m = fh / fch;
    }
    else {

        m = fw / fcw;
    }

    if (m >= 1.0f) {

        m = floorf(m);
    }

    output[0] = fw/2.0f - m * fcw / 2.0f;
    output[1] = fh/2.0f - m * fch / 2.0f;
    output[2] = m * fcw;
    output[3] = m * fch;

    // Scale to [-1.0, 1.0]^2 range
    output[0] = ((output[0] / fw) * 2.0) - 1.0f;
    output[1] = ((output[1] / fh )* 2.0) - 1.0f;
    output[2] = ((output[2] / fw) * 2.0);
    output[3] = ((output[3] / fh )* 2.0);

    shader_pass_vertex_shader_properties(r->shader, 
        output[0], output[1], output[2], output[3]);
}


void renderer_update_canvas_texture(Renderer* r, u8* pixels) {
    
    i32 i;

    for (i = 0; i < (i32)r->canvasWidth*r->canvasHeight; ++ i) {

        r->updateData[i] = r->colorTable[ pixels[i] ];
    }

    // glBindTexture(GL_TEXTURE_2D, r->canvasTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
        (i32) r->canvasWidth, (i32) r->canvasHeight, 
        GL_RGBA, GL_UNSIGNED_BYTE, (void*) r->updateData);
}


void renderer_refresh(Renderer* r) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_buffers(r->canvasMeshBuffers);
    glfwSwapBuffers((GLFWwindow*) r->window);
}

