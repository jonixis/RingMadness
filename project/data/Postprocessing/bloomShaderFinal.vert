$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

attribute vec4 Position;
attribute vec4 TexCoord;

varying vec4 texCoordVarying;

void main() {
    texCoordVarying = TexCoord;
    gl_Position = Position;
}

