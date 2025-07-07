#version 450

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

///////////////////////////////////////////////////////////////////////////////
// Environment
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec3 depthColor;


///////////////////////////////////////////////////////////////////////////////
// Input varyings from vertex shader
///////////////////////////////////////////////////////////////////////////////

uniform float near; 
uniform float far;
uniform int isOrtho;

float OrthoLinearizeDepth(float depth) 
{
    return near + depth * (far - near);
}

float PerspLinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}


void main()
{
    float lineardepth;
    if(isOrtho == 1) {
        lineardepth = OrthoLinearizeDepth(gl_FragCoord.z);
    } else {
        lineardepth = PerspLinearizeDepth(gl_FragCoord.z);
    }

    depthColor = vec3(lineardepth);
}