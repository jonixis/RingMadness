$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif


uniform lowp vec3 Kd;


uniform sampler2D DiffuseMap;
uniform mediump vec4 LightPos;
uniform
#ifdef GL_ES
highp   // offset gets very big, needs to be highp on iOS
#endif
float offset;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying highp vec4 texCoordVarying;

varying mediump vec4 posVarying;   


void main()
{
    
    gl_FragColor = (ambientVarying + diffuseVarying) * vec4(Kd, 1.0) + specularVarying;
    //gl_FragColor = (diffuseVarying);

}
