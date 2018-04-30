$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying highp vec4 texCoordVarying;

void main()
{
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ModelViewMatrix * Position;
}
