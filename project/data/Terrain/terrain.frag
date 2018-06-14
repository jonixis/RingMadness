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

//SSAO//
uniform mat4 ProjectionMatrix;

highp vec3 n;
vec3 pos;

uniform vec3 samples[64];
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

float k = 64.0;

const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0);

uniform sampler2D depth;
uniform sampler2D noise_texture;
//END//

void main()
{
    //Fog
    
    visibility = exp(-pow((distanceCameraVertex*density),gradient));
    visibility = clamp(visibility, 0.0, 1.0);
    
    
    //SSAO//
    vec3 randomVec = normalize(texture2D(noise_texture, noiseScale * noiseScale).xyz);
    vec3 tangent = normalize(randomVec - n * dot(randomVec, n));
    vec3 bitangent = cross(n, tangent);
    mat3 TBN = mat3(tangent, bitangent, n);
    
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 sample = TBN * samples[i]; // from tangent to view-space
        sample = pos + sample * radius;
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample, 1.0);
        offset = ProjectionMatrix * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture2D(DiffuseMap, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / (pos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / k);
    //END//
    
    vec4 ambient = (ambientVarying*occlusion);
    
    color = (ambient + diffuseVarying) * vec4(Kd,1.0) + specularVarying;
    //color = (ambientVarying + diffuseVarying) * vec4(Kd,1.0) + specularVarying;
    //gl_FragColor = mix(fogColor, color, visibility);
    vec3 colorMap = texture(noise_texture, offset.xy).rgb;
    color = (ambientColor + diffuseColor) * colorMap.rgb
    gl_FragColor = vec4(colorMap, 1.0);
    
}
