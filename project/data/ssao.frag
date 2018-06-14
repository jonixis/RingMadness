$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mat4 ProjectionMatrix;

uniform sampler2D depthMap;
uniform sampler2D normalMap;
uniform sampler2D noiseTex;

uniform vec3 samples[16];

varying vec4 texCoordVarying;

const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0);

float kernelSize = 16.0;
float radius = 0.5;
float bias = 0.025;


void main()
{
    // depth of fragment
    float depth = texture2D(depthMap, texCoordVarying.xy).r;
    
    // construct position from depth
    vec2 fragPosXY = texCoordVarying.xy * 2.0 -1.0;
    float fragPosZ = texture2D(depthMap, texCoordVarying.xy).r;
    vec3 fragPos = vec3(fragPosXY, fragPosZ);
    fragPos = vec3( ProjectionMatrix * vec4(fragPos, 1.0) );
    
    // normal from texture (rescale to [-1.0 , 1.0])
    vec3 normal = normalize(texture2D(normalMap, texCoordVarying.xy).rgb);
    normal = normalize(normal);
    
    // random vector for the fragments -> points in xy-direction
    vec3 randomVec = normalize(texture2D(noiseTex, texCoordVarying.xy).xyz);
    randomVec = normalize(randomVec);
    
    // construct TBN matrix to transform from tangent to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    
    float occlusion = 0.0;
    // for each kernel-sample, test depth values
    for (int i = 0; i < 16; ++i) {
        
        vec3 samp = TBN * samples[i];
        samp = samp * radius + fragPos;
        
        vec4 offset = vec4(samp, 1.0);
        offset = ProjectionMatrix * offset; // from view to clip-space
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5; // tranform to range [0.0, 1.0]
        
        // get depth of the sample
        float sampleDepth = texture2D(depthMap, offset.xy).z;
        
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samp.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    
    // compute occlusion-weight for current fragment
    occlusion = 1.0 - (occlusion / kernelSize);
    
    // output gray-scale color to texture
    gl_FragColor = vec4(vec3(occlusion), 1.0);
}
