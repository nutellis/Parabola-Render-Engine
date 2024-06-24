#version 450 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 NormalMap;
layout (location = 2) in vec2 TextureCoord;

out vec3 FragmentPosition;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{

	FragmentPosition = vec3(model * vec4(VertexPosition, 1.0));
    gl_Position =  projection * view * model * vec4(VertexPosition, 1.0f);


}