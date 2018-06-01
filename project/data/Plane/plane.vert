$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mediump mat4 ModelViewMatrix;
uniform mediump mat4 ProjectionMatrix;
uniform mediump mat4 ModelMatrix;

uniform highp mat3 NormalMatrix;

uniform highp vec4 LightPos;
highp vec4 tempLightPos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;       // pos in world space
varying mediump vec3 normalVarying;    // normal in world space

void main()
{
    posVarying = ModelMatrix * Position;
    tempLightPos = ModelMatrix * LightPos;
    
    // Ambient
    ambientVarying = vec4(Ka * Ia, 1.0);
    
    // Diffuse
    diffuseVarying = vec4(0.0);
    mediump vec3 n = normalize(Normal);
    mediump vec3 l = vec3(normalize(tempLightPos - posVarying));
    lowp float intensity = dot(n, l);
    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
    diffuseVarying = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    //diffuseVarying = vec4(Normal, 1.0);
    
    // Specular
    specularVarying = vec4(0.0);
    
    gl_Position = ProjectionMatrix * ModelViewMatrix * Position;
}
