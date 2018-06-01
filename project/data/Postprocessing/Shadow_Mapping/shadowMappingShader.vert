$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

attribute vec4 Position;
attribute vec4 TexCoord;
attribute vec3 Normal;

varying vec4 fragPosVarying;
varying vec4 normalVarying;
varying vec4 texCoordVarying;
varying vec4 fragPosLightSpace;

uniform mat4 lightSpaceMatrix;

/*
 // For normal Shadow mapping //
void main() {
    fragPosVarying = vec3(ModelMatrix * Position);
    normalVarying = transpose(inverse(mat3(ModelMatrix))) * Normal;
    texCoordVarying = TexCoord;
    fragPosLightSpace = lightSpaceMatrix * vec4(fragPosVarying, 1.0);
    gl_Position = ProjectionMatrix * ModelViewMatrix * Position;
}
*/


// For Debugging the shadow map//
void main() {
    texCoordVarying = TexCoord;
    gl_Position = Position;
}

