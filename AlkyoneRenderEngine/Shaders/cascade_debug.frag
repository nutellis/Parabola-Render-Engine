#version 450

#define USE_HARDWARE_SHADOWS 0

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

#define PI 3.14159265359

///////////////////////////////////////////////////////////////////////////////
// Input varyings from vertex shader
///////////////////////////////////////////////////////////////////////////////
in vec2 texCoord;
in vec3 viewSpaceNormal;
in vec3 viewSpacePosition;

///////////////////////////////////////////////////////////////////////////////
// Input uniform variables
///////////////////////////////////////////////////////////////////////////////
uniform mat4 viewInverse;


uniform int enableSSAO = 0;


///////////////////////////////////////////////////////////////////////////////
// Light source
///////////////////////////////////////////////////////////////////////////////

uniform vec3 point_light_color = vec3(1.0, 1.0, 1.0);
uniform float point_light_intensity_multiplier = 1.0;

uniform vec3 LightPosition;
uniform vec3 LightDirection;

uniform vec3 viewSpaceLightPosition;
uniform vec3 viewSpaceLightDir;

///////////////////////////////////////////////////////////////////////////////
// Shadow Information
///////////////////////////////////////////////////////////////////////////////
// textures 10 to 13
uniform sampler2D shadowMap[4];


in vec4 shadowMapCoord[4];
uniform vec4 farPlanes;
uniform int numOfCascades;



layout(binding = 16) uniform sampler2D ssaoTexture;

///////////////////////////////////////////////////////////////////////////////
// Output color
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec4 fragmentColor;

vec3 color[4] = vec3[4](vec3(0.85, 0.85, 1.0),
						vec3(0.85, 1.0, 0.85),
						vec3(1.0, 0.85, 0.85),
						vec3(1.0, 1.0, 1.0));

vec3 shadowCoef(int index)
{
	

	vec3 shadowUV = shadowMapCoord[index].xyz;

	float shadowDepth = 0;

	shadowDepth = texture(shadowMap[index], shadowMapCoord[index].xy).r;

	float visibility = (shadowMapCoord[index].z <= shadowDepth) ? 1.0 : 0.5;

	return color[index] * visibility;
}

int getIndex() {

	float viewDepth = abs(viewSpacePosition.z);
	int cascadeIndex =
		int(viewDepth > farPlanes.x) +
		int(viewDepth > farPlanes.y) +
		int(viewDepth > farPlanes.z);
	cascadeIndex = clamp(cascadeIndex, 0, numOfCascades - 1);

	return cascadeIndex;
}

vec3 baseColor(int index) {
	
	return color[index];
}

vec3 calculateDiffuse(vec3 wo, vec3 n, vec3 base_color)
{
	vec3 Li = point_light_intensity_multiplier * point_light_color; 
	vec3  wi = LightDirection;

	vec3 lightColor = vec3(1.0,1.0,1.0);
	float specularStrength = 0.8;

	vec3 lightDir = LightDirection; 
	vec3 viewDir = normalize(wo - gl_FragCoord.xyz);
	vec3 reflectDir = reflect(-LightDirection, n);  

	float ambientStrength = 0.10;
    vec3 ambient = ambientStrength * Li;
	
	float diff = max(dot(n, LightDirection), 0.0);
	vec3 diffuse = diff * Li;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * Li;  

    vec3 result = (diffuse + ambient) * base_color; //specular

	return result;
}

void main()
{
	vec3 wo = -normalize(viewSpacePosition);
	vec3 n = normalize(viewSpaceNormal);

	int index = getIndex();

	vec3 finalColour = calculateDiffuse(wo,n,baseColor(index)) * shadowCoef(index);

	fragmentColor = vec4(finalColour, 1.0);
}