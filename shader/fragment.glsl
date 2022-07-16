uniform sampler2D texSampler;
    
varying vec2 uv;
     

void main() {
    
    gl_FragColor = texture2D(texSampler, uv);
}
