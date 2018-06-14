$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mediump mat4 ModelViewMatrix;
uniform mediump mat4 ProjectionMatrix;

attribute vec4 Position;
attribute vec4 TexCoord;

varying vec4 texCoordVarying;

void main() {
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ModelViewMatrix * Position;
}
