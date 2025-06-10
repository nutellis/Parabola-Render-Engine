#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

out vec3 viewSpaceNormal;
out vec2 texCoord;

void main()
{
	gl_Position =  modelViewProjectionMatrix * vec4(position, 1.0);
	texCoord = uv;

	viewSpaceNormal = (normalMatrix * vec4(normal, 0.0)).xyz;
}