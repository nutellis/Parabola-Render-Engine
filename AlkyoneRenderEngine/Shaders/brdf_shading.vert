#version 450
///////////////////////////////////////////////////////////////////////////////
// Input vertex attributes
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 NormalPosition;
layout(location = 2) in vec2 TextureCoord;

///////////////////////////////////////////////////////////////////////////////
// Input uniform variables
///////////////////////////////////////////////////////////////////////////////
uniform mat4 normalMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;


///////////////////////////////////////////////////////////////////////////////
// Cascade Shadow Map Data
///////////////////////////////////////////////////////////////////////////////
layout(std430, binding = 0) buffer Cascades {
    mat4 lightMatrices[];
};
uniform int numOfCascades; 


///////////////////////////////////////////////////////////////////////////////
// Output to fragment shader
///////////////////////////////////////////////////////////////////////////////
out vec2 texCoord;
out vec3 viewSpaceNormal;
out vec3 viewSpacePosition;

out vec4 shadowMapCoord[4];

void main()
{
	// Output to fragment shader
	gl_Position = modelViewProjectionMatrix * vec4(VertexPosition, 1.0);
	texCoord = TextureCoord;
	viewSpaceNormal = (normalMatrix * vec4(NormalPosition, 0.0)).xyz;
	viewSpacePosition = (modelViewMatrix * vec4(VertexPosition, 1.0)).xyz;

    for (int i = 0; i < numOfCascades; ++i) {
		shadowMapCoord[i] = lightMatrices[i] * vec4(viewSpacePosition, 1.0);
    }
}
