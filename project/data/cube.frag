$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif


uniform
#ifdef GL_ES
highp   // offset gets very big, needs to be highp on iOS
#endif
float offset;

uniform samplerCube texCube;

varying vec4 fragPos;

void main()
{
    //wave effect for flame
    
    vec4 lightColor = vec4(1.0, 0.0, 0.0, 1.0);
    
    gl_FragColor = textureCube(texCube, vec3(fragPos.x, fragPos.z + 0.00, fragPos.y));
}
