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
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
//TODO: pass less uniforms!

uniform mat4 lightMatrix;
///////////////////////////////////////////////////////////////////////////////
// Output to fragment shader
///////////////////////////////////////////////////////////////////////////////
out vec2 texCoord;
out vec3 viewSpaceNormal;
out vec3 viewSpacePosition;

out vec4 shadowMapCoord;

void main()
{
	mat4 modelViewMatrix = view * model;
	mat4 modelViewProjectionMatrix = projection * modelViewMatrix;
	mat4 normalMatrix = transpose(inverse(mat4(modelViewMatrix)));

	// Output to fragment shader
	gl_Position = modelViewProjectionMatrix * vec4(VertexPosition, 1.0);
	texCoord = TextureCoord;
	viewSpaceNormal = (normalMatrix * vec4(NormalPosition, 0.0)).xyz;
	viewSpacePosition = (modelViewMatrix * vec4(VertexPosition, 1.0)).xyz;

	shadowMapCoord = lightMatrix * vec4(viewSpacePosition, 1.0);

}
