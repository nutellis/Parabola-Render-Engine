#version 450

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

///////////////////////////////////////////////////////////////////////////////
// Environment
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec3 normalColor;
layout(location = 1) out vec3 depthColor;


///////////////////////////////////////////////////////////////////////////////
// Input varyings from vertex shader
///////////////////////////////////////////////////////////////////////////////
in vec3 viewSpaceNormal;

uniform float near; 
uniform float far; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	normalColor = vec3(gl_FragCoord.z); // normalize(viewSpaceNormal);

    depthColor = vec3(LinearizeDepth(gl_FragCoord.z)); // / far;
}