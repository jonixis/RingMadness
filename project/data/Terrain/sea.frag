$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform vec3 Kd;

uniform sampler2D DiffuseMap;

uniform
#ifdef GL_ES
highp   // offset gets very big, needs to be highp on iOS
#endif
float offset;

varying vec4 ambientVarying;
varying vec4 diffuseVarying;
varying vec4 specularVarying;

vec4 color;

//fog
varying float visibility;
uniform vec4 fogColor;

void main()
{
    color = (ambientVarying + diffuseVarying) * vec4(Kd,1.0) + specularVarying;
    gl_FragColor = mix(fogColor, color, visibility);
}
