$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform vec3 ambientColor;
uniform lowp vec3 Kd;   // diffuse material coefficient


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
    gl_FragColor = (ambientVarying + diffuseVarying) * vec4(Kd, 1.0) + specularVarying;
}
