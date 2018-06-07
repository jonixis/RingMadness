$B_SHADER_VERSION
#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D fbo_texture_scene;
uniform sampler2D fbo_texture_bloomBlur;

varying vec4 texCoordVarying;


void main() {
    const float gamma = 1.5; //was 1.8
    const float exposure = 2.0; //was 1.0
    vec3 hdrColor = texture2D(fbo_texture_scene, texCoordVarying.st).rgb;
    vec3 bloomColor = texture2D(fbo_texture_bloomBlur, texCoordVarying.st).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction
    result = pow(result, vec3(1.0 / gamma));
    gl_FragColor = vec4(result, 1.0);
}
