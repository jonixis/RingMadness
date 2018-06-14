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
varying float distanceCameraVertex;

vec4 color;

varying vec4 normalVarying;

//Fog
float visibility;
float density = 0.0007;
float gradient = 4.0;

uniform vec4 fogColor;

void main()
{
    //Fog
    
    visibility = exp(-pow((distanceCameraVertex*density),gradient));
    visibility = clamp(visibility, 0.0, 1.0);
    
    color = (ambientVarying + diffuseVarying) * vec4(Kd,1.0) + specularVarying;
    gl_FragColor = mix(fogColor, color, visibility);
}
