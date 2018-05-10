$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform lowp vec3 Kd;   // diffuse material coefficient

uniform
#ifdef GL_ES
highp   // offset gets very big, needs to be highp on iOS
#endif
float offset;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;

void main()
{
    
    gl_FragColor = (ambientVarying + diffuseVarying) * vec4(Kd, 1.0);
}

