
uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


attribute vec4 Position;


void main()
{
    vec4 pos = ModelViewMatrix * Position;  // vertex position in eye coordinates
   
    gl_Position = ProjectionMatrix * pos;
}
