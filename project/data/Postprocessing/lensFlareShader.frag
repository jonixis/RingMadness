$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D fbo_texture;

varying vec4 texCoordVarying;

void main() {
    
    vec2 texCoordVarying = -texCoordVarying.st + vec2(1.0);
    
    // ghost vector to image centre:
    vec2 ghostVec = (vec2(0.5) - texCoordVarying.st) * 0.3;
    
    // sample ghosts:
    vec4 result = vec4(0.0);
    for (int i = 0; i < 3; ++i) {
        vec2 offset = fract(texCoordVarying.st + ghostVec * float(i));
        
        float weight = length(vec2(0.5) - offset) / length(vec2(0.5));
        weight = pow(1.0 - weight, 10.0);
        
        result += texture2D(fbo_texture, offset) * weight;
    }
    
    //    for (int i = 0; i < 3; ++i) {
    //        vec2 offset = fract(texCoordVarying.st + ghostVec * float(i)) * 0.5;
    //
    //        float weight = length(vec2(0.5) - offset) / length(vec2(0.5));
    //        weight = pow(1.0 - weight, 10.0);
    //
    //        result += texture2D(fbo_texture, offset) * weight;
    //    }
    
    
    //    // sample halo:
    //    vec2 haloVec = normalize(ghostVec) * 1.0;
    //    float weight = length(vec2(0.5) - fract(texCoordVarying.st + haloVec)) / length(vec2(0.5));
    //    weight = pow(1.0 - weight, 5.0);
    //    result += texture2D(fbo_texture, texCoordVarying.st + haloVec) * weight;
    
    gl_FragColor = result;
}
