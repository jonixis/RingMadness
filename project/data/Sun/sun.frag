$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform vec3 ambientColor;
uniform vec3 Kd;


uniform sampler2D DiffuseMap;

uniform
#ifdef GL_ES
highp   // offset gets very big, needs to be highp on iOS
#endif
float offset;

varying highp vec4 texCoordVarying;

void main()
{
    
    vec2 texcoord = texCoordVarying.st;
    
    gl_FragColor = vec4(Kd, 1.0);
}

