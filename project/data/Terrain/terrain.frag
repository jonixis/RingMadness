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

varying vec4 fragPosVarying;
varying vec4 normalVarying;
varying vec4 texCoordVarying;
varying vec4 fragPosLightSpace;


uniform sampler2D depthMap;
vec4 color;



//Fog
float visibility;
float density = 0.0007;
float gradient = 4.0;

uniform vec4 fogColor;

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

void main()
{
    
    // calculate shadow
    float shadow = ShadowCalculation(fragPosLightSpace);
    
    //Fog
    
    visibility = exp(-pow((distanceCameraVertex*density),gradient));
    visibility = clamp(visibility, 0.0, 1.0);
    
    color = (ambientVarying +  (1.0 - shadow) + diffuseVarying) * vec4(Kd,1.0) + specularVarying;
    gl_FragColor = mix(fogColor, color, visibility);
}
