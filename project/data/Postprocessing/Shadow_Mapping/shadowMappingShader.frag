$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D depthMap;
uniform sampler2D diffuseTexture;

varying vec4 fragPosVarying;
varying vec4 normalVarying;
varying vec4 texCoordVarying;
varying vec4 fragPosLightSpace;

uniform vec4 sunPosition;
uniform vec4 camPosition;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(depthMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    
    return shadow;
}

void main() {

    

    vec3 color = texture2D(diffuseTexture, texCoordVarying.st).rgb;
    vec3 normal = normalize(normalVarying);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(sunPosition - fragPosVarying);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    
    
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    gl_FragColor = vec4(lighting, 1.0);

}


