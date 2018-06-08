$B_SHADER_VERSION
#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D fbo_texture;

varying vec4 texCoordVarying;

void main() {
    vec4 color = texture2D(fbo_texture, texCoordVarying.st);
    
    // Extract sun color and ring color
    if((color.x > 0.9 && color.y > 0.5 && color.z < 0.1) || (color.x == 1.0 && color.y == 0.0 && color.z == 0.0)){
        gl_FragColor = color;
    }
    else{
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
