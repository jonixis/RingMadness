$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D depthMap;

varying vec4 texCoordVarying;

//uniform float near_plane;
//uniform float far_plane;

// required when using a perspective projection matrix
//float LinearizeDepth(float depth)
//{
//    float near_plane = 1.0;
//    float far_plane = 200.0;
//    float z = depth * 2.0 - 1.0; // Back to NDC
//    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
//}
//
//
//void main() {
//
//    float depthValue = texture2D(depthMap, texCoordVarying.st).r;
//
//    gl_FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
//
////    gl_FragColor = vec4(vec3(depthValue), 1.0); // orthographic
//
//}

float LinearizeDepth(in vec2 uv)
{
    float zNear = 1000.0;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = 1200.0; // TODO: Replace by the zFar  of your perspective projection
    float depth = texture2D(depthMap, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

void main()
{
    float c = LinearizeDepth(texCoordVarying.st);
    gl_FragColor = vec4(c, c, c, 1.0);
}
