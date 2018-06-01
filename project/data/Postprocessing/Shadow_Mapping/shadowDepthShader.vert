$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mat4 ModelViewMatrix;
uniform mat4 lightSpaceMatrix;
uniform vec4 modelMatrix;
attribute vec4 Position;

void main()
{
    gl_Position = lightSpaceMatrix * ModelViewMatrix * Position;
    //gl_Position = lightSpaceMatrix * modelMatrix * Position;

}
