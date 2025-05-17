#version 450 core
out vec4 FragColor;

uniform vec4 uColour;

void main()
{
    FragColor = vec4(uColour) ;

}