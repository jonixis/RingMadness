$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec4 sunPosition;
uniform vec4 modelMatrix;
uniform vec4 camPosition;
uniform float time; 

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying vec4 ambientVarying;
varying vec4 diffuseVarying;
varying vec4 specularVarying;
varying vec4 normalVarying;

//Fog
varying float visibility;
float density = 0.001;
float gradient = 4.0;

const float PI = 3.1415926535897932384626433832795;
const float waveLength = 4.0;
const float waveAmplitude = 0.2;
float xDisortion = 0.0;
float yDisortion = 0.0;
float zDisortion = 0.0;

vec4 tempPosition;
vec4 tempNormal;

float generateOffset(float x, float z, float val1, float val2){
    
    float radiansX = ((mod(x+z*x*val1, waveLength)/waveLength) + time*0.01 * mod(x * 0.8 + z, 1.5)) * 2.0 * PI;
    float radiansZ = ((mod(val2 * (z*x +x*z), waveLength)/waveLength) + time*0.01 * 2.0 * mod(x , 2.0) ) * 2.0 * PI;
    
    //return (sin(rZ) - cos(rX)) * 0.01;
    //return ((sin(x * 4.0 + time *0.05)) + cos(z * 8.0 + time*0.05))* 0.1;
    return (sin(radiansX) + cos(radiansZ)) *waveAmplitude * 0.5 ;
}

void main()
{
    //normalVarying = vec4(Normal, 1.0);
    //normalVarying = Position;
    
    //Disortion
    xDisortion = generateOffset(Position.x, Position.z,  0.2, 0.1);
    yDisortion = generateOffset(Position.x, Position.z, 0.1, 0.3);
    zDisortion = generateOffset(Position.x, Position.z,  0.15, 0.2);
    
    
    tempPosition = vec4(Position.x + xDisortion, Position.y + yDisortion, Position.z + zDisortion, 1.0);
    tempPosition = vec4(Position.x, Position.y + yDisortion, Position.z, 1.0);
    
    
    tempNormal = vec4(Normal.x + xDisortion, Normal.y + yDisortion, Normal.z + zDisortion, 1.0);
    normalVarying = tempNormal;
    
    
    //lightning
    ambientVarying = vec4(0.4,0.4,0.4,1.0);
    
    diffuseVarying = vec4(0.0);
    
    highp vec3 normal = vec3(tempNormal);
    highp vec3 lightDirection = vec3(normalize(sunPosition-(modelMatrix * Position)));
    
    highp float intensity = dot(normal,lightDirection);
    highp vec3 diffuse = vec3(0.5,0.5,0.5) * clamp(intensity, 0.0, 1.0);
    diffuseVarying = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    
    
    specularVarying = vec4(0.0);
        if (intensity > 0.0)
     {
     highp vec3 eyeVec = vec3(normalize(camPosition-(modelMatrix * Position)));
     highp vec3 h = vec3(normalize(lightDirection+eyeVec));
     
     
     highp vec3 specular = vec3(0.2,0.2,0.2) * pow(dot(h,normal),1.0);
     specularVarying = vec4(clamp(specular, 0.0, 1.0), 1.0);
     
     }
 
    
    //Fog
    float distanceCameraVertex = length(ModelViewMatrix * Position);
    visibility = exp(-pow((distanceCameraVertex*density),gradient));
    visibility = clamp(visibility, 0.0, 1.0);
    

    gl_Position = ProjectionMatrix * ModelViewMatrix * tempPosition;
}
