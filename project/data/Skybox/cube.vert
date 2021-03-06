$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

attribute vec4 Position;


varying vec4 fragPos;

//varying vec4 texCoordVarying;

void main()
{
    
    fragPos = Position;
    
    gl_Position = ProjectionMatrix * ModelViewMatrix * Position;
}
