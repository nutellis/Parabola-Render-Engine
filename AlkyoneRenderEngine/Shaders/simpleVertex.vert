#version 450 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 NormalMap;
layout (location = 2) in vec2 TextureCoord;
layout (location = 3) in vec3 InstanceOffset;

out vec3 FragmentPosition;
out vec3 Normal;
out vec2 Texture;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    // Incorporate InstanceOffset into the vertex position
    vec3 modifiedVertexPosition = VertexPosition + InstanceOffset;

    FragmentPosition = vec3(model * vec4(modifiedVertexPosition, 1.0));

    Normal = mat3(transpose(inverse(model))) * NormalMap;
    Texture = TextureCoord;

    gl_Position = projection * view * vec4(FragmentPosition, 1.0);
}