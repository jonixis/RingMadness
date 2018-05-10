$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform vec3 ambientColor;

uniform sampler2D DiffuseMap;

uniform
#ifdef GL_ES
highp   // offset gets very big, needs to be highp on iOS
#endif
float offset;

varying highp vec4 texCoordVarying;
varying vec4 ambientVarying;
varying vec4 diffuseVarying;
varying vec4 specularVarying;

void main()
{
    //wave effect for flame
    vec2 texcoord = texCoordVarying.st;
    
    //gl_FragColor = texture2D(DiffuseMap, texcoord);
    gl_FragColor = (ambientVarying + diffuseVarying) * texture2D(DiffuseMap, texcoord) + specularVarying;
}
