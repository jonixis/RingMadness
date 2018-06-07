$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec4 sunPosition;
uniform vec4 modelMatrix;
uniform vec4 camPosition;
uniform vec4 planePosition; 

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying vec4 ambientVarying;
varying vec4 diffuseVarying;
varying vec4 specularVarying;
varying float distanceCameraVertex;

varying vec4 normalVarying;

//SSAO//
highp vec3 n;
vec3 pos;
//END//

void main()
{
    //light
    ambientVarying = vec4(0.4,0.4,0.4,1.0);
    
    diffuseVarying = vec4(0.3);
 
    highp vec3 normal = normalize(Normal);
    highp vec3 lightDirection = vec3(normalize(sunPosition-(modelMatrix * Position)));
    
    highp float intensity = dot(normal,lightDirection);
    highp vec3 diffuse = vec3(0.5,0.5,0.5) * clamp(intensity, 0.0, 1.0);
    diffuseVarying = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
 
    
    specularVarying = vec4(0.0);
/*    if (intensity > 0.0)
    {
        highp vec3 eyeVec = vec3(normalize(camPosition-(modelMatrix * Position)));
        highp vec3 h = vec3(normalize(lightDirection+eyeVec));
        
        
        highp vec3 specular = vec3(0.2,0.2,0.2) * pow(dot(h,normal),1.0);
        specularVarying = vec4(clamp(specular, 0.0, 1.0), 1.0);
        
    }
 */
    
    //SSAO//
    n = normal;
    pos = vec3(Position);
    //END//
    
    //normal coloring
    normalVarying = vec4(normal, 1.0);
    
    distanceCameraVertex = length(ModelViewMatrix * Position);
    
    gl_Position = ProjectionMatrix * ModelViewMatrix * Position;
}
