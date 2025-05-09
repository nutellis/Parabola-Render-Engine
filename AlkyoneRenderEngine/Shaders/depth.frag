#version 450

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

layout(location = 0) out vec4 fragmentColor;
uniform float near_plane;
uniform float far_plane;

in float depth;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far_plane;
    fragmentColor = vec4(gl_FragCoord.z); //vec3(depth), 1.0);
}