#version 450

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

layout(location = 0) out vec4 fragmentColor;

layout(binding = 11) uniform sampler2D frameBufferTexture;

uniform int filterSize = 1;

vec4 textureRect(in sampler2D tex, vec2 rectangleCoord)
{
	return texture(tex, rectangleCoord / textureSize(tex, 0));
}
 
void main() {
	vec3 result = vec3(0.0);
	float weight = 1.0 / (filterSize * filterSize);

	for(float i = -filterSize / 2; i <= filterSize / 2; i += 1.0)
	{
		for(float j = -filterSize / 2; j <= filterSize / 2; j += 1.0)
		{
			result += weight * textureRect(frameBufferTexture, gl_FragCoord.xy + vec2(i, j)).rgb;
		}
	}
    fragmentColor = vec4(result, 1.0);
}
