$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;

void main()
{
    ambientVarying = vec4(Ka * vec3(0.9), 1.0);
    diffuseVarying = vec4(Kd, 1.0);
    
    gl_Position = ProjectionMatrix * ModelViewMatrix * Position;
}
