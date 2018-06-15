$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mat4 ProjectionMatrix;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[16];
uniform mat4 projection;

varying vec4 texCoords;

const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0);

float kernelSize = 16.0;
float radius = 0.5;
float bias = 0.025;


void main()
{
    // Implemented with Tutorial from https://learnopengl.com/Advanced-Lighting/SSAO
    // get input for SSAO algorithm
    vec3 fragPos = texture2D(gPosition, texCoords.xy).xyz;
    vec3 normal = normalize(texture2D(gNormal, texCoords.xy).rgb);
    vec3 randomVec = normalize(texture2D(texNoise, texCoords.xy * noiseScale).xyz);
    
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < 16; ++i)
    {
        // get sample position
        vec3 sample = TBN * samples[i]; // from tangent to view-space
        sample = fragPos + sample * radius;
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample, 1.0);
        offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture2D(gPosition, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    
    occlusion = 1.0 - (occlusion / kernelSize);
    
    gl_FragColor = vec4(vec3(occlusion), 1.0);
}
