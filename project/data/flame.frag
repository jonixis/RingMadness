$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform vec3 ambientColor;

uniform sampler2D DiffuseMap;

uniform
#ifdef GL_ES
highp   // offset gets very big, needs to be highp on iOS
#endif
float offset;

varying vec4 texCoordVarying;

void main()
{
    //wave effect for flame
    vec2 texcoord = texCoordVarying.st;
    
    gl_FragColor = texture2D(DiffuseMap, texcoord);
}
