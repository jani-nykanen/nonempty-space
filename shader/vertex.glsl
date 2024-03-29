attribute vec2 vertexPos;
attribute vec2 vertexUV;
    
// uniform mat4 transform;
    
uniform vec2 pos;
uniform vec2 size;
    
varying vec2 uv;


void main() {
    
    // gl_Position = transform * vec4(vertexPos * scale + pos, 1, 1);
    gl_Position = vec4(vertexPos * size + pos, 1, 1);
    uv = vertexUV;
}
