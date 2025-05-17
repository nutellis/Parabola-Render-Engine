#version 450


#define USE_HARDWARE_SHADOWS 0

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

///////////////////////////////////////////////////////////////////////////////
// Material
///////////////////////////////////////////////////////////////////////////////
uniform vec3 material_color = vec3(1, 1, 1);
uniform float material_metalness = 0;
uniform float material_fresnel = 0;
uniform float material_shininess = 0;
uniform vec3 material_emission = vec3(0);

uniform int has_color_texture = 0;
layout(binding = 1) uniform sampler2D colorMap;
uniform int has_emission_texture = 0;
//layout(binding = 5) uniform sampler2D emissiveMap;

///////////////////////////////////////////////////////////////////////////////
// Environment
///////////////////////////////////////////////////////////////////////////////
layout(binding = 6) uniform sampler2D environmentMap;
layout(binding = 7) uniform sampler2D irradianceMap;
layout(binding = 8) uniform sampler2D reflectionMap;
uniform float environment_multiplier;

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////
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
//Cascade Shadow Information
///////////////////////////////////////////////////////////////////////////////
// textures 10 to 13
uniform sampler2D shadowMap[4];



in vec4 shadowMapCoord[4];
uniform vec4 farPlanes;

///////////////////////////////////////////////////////////////////////////////
// PCSS
///////////////////////////////////////////////////////////////////////////////
uniform int usePCSS = 0;
uniform float lightSize = 0.5; 
uniform vec4 lightFrustrumWidth; 
uniform vec4 nearPlanes; 
uniform vec4 texelSize;


const vec2 offsets[16] = vec2[](
    vec2( -0.94201624, -0.39906216 ),
    vec2(  0.94558609, -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2(  0.34495938,  0.29387760 ),
    vec2( -0.91588581,  0.45771432 ),
    vec2( -0.81544232, -0.87912464 ),
    vec2( -0.38277543,  0.27676845 ),
    vec2(  0.97484398,  0.75648379 ),
    vec2(  0.44323325, -0.97511554 ),
    vec2(  0.53742981, -0.47373420 ),
    vec2( -0.26496911, -0.41893023 ),
    vec2(  0.79197514,  0.19090188 ),
    vec2( -0.24188840,  0.99706507 ),
    vec2( -0.81409955,  0.91437590 ),
    vec2(  0.19984126,  0.78641367 ),
    vec2(  0.14383161, -0.14100790 )
);

const vec2 poisson[64] = vec2[](
    vec2( 0.5093, -0.3431 ),
    vec2( -0.3122,  0.5691 ),
    vec2( 0.1223,  0.7255 ),
    vec2( 0.3993,  0.0586 ),
    vec2( 0.5487, -0.0242 ),
    vec2( -0.6123, -0.3390 ),
    vec2( -0.4282,  0.3894 ),
    vec2( 0.3122,  0.5122 ),
    vec2( -0.3298, -0.3770 ),
    vec2( -0.6641,  0.1059 ),
    vec2( 0.2340, -0.5379 ),
    vec2( -0.2299, -0.6703 ),
    vec2( 0.5331, -0.1179 ),
    vec2( 0.0802,  0.2286 ),
    vec2( -0.4136, -0.2437 ),
    vec2( 0.3238,  0.0059 ),
    vec2( 0.6110, -0.1572 ),
    vec2( 0.1560,  0.5991 ),
    vec2( 0.2942, -0.4132 ),
    vec2( -0.4442,  0.1487 ),
    vec2( 0.4702,  0.4508 ),
    vec2( 0.3810, -0.4555 ),
    vec2( -0.1534,  0.4434 ),
    vec2( 0.5975,  0.1565 ),
    vec2( -0.2684,  0.3744 ),
    vec2( -0.0836, -0.2247 ),
    vec2( 0.4737, -0.4480 ),
    vec2( -0.4133,  0.5294 ),
    vec2( 0.2184, -0.2701 ),
    vec2( -0.6148,  0.1499 ),
    vec2( 0.0899, -0.5099 ),
    vec2( -0.5087, -0.4595 ),
    vec2( 0.2917, -0.1797 ),
    vec2( -0.2191,  0.0197 ),
    vec2( 0.4818,  0.5563 ),
    vec2( -0.3246,  0.4948 ),
    vec2( 0.4967, -0.1702 ),
    vec2( 0.3664, -0.3523 ),
    vec2( -0.3447, -0.2654 ),
    vec2( 0.2310, -0.3074 ),
    vec2( -0.5076, -0.0520 ),
    vec2( 0.1366, -0.1357 ),
    vec2( -0.0695,  0.3195 ),
    vec2( -0.4261, -0.0503 ),
    vec2( 0.4932, -0.4029 ),
    vec2( -0.2959,  0.1985 ),
    vec2( 0.2817, -0.5972 ),
    vec2( -0.4310,  0.2877 ),
    vec2( 0.0609,  0.3994 ),
    vec2( 0.5374, -0.3870 ),
    vec2( 0.1187,  0.4473 ),
    vec2( -0.3878,  0.2691 ),
    vec2( -0.2214,  0.1298 ),
    vec2( -0.2325, -0.4597 ),
    vec2( 0.0984, -0.6151 ),
    vec2( -0.0651,  0.5206 ),
    vec2( -0.0585,  0.2218 ),
    vec2( 0.3700,  0.2168 ),
    vec2( 0.1325,  0.5723 ),
    vec2( -0.2899, -0.2298 ),
    vec2( 0.5062,  0.2843 ),
    vec2( 0.3769,  0.3685 ),
    vec2( -0.5055,  0.0651 ),
    vec2( 0.2419,  0.4159 )
);

layout(binding = 16) uniform sampler2D ssaoTexture;

///////////////////////////////////////////////////////////////////////////////
// Output color
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec4 fragmentColor;

float PCF(float bias, float filterRadius, int index, float stepSize) {
    float sum = 0.0;

    for (int x = 0; x < 64; x++) {
        vec2 offset = shadowMapCoord[index].xy + stepSize * poisson[x] * filterRadius;

		offset = clamp(offset, 0.0, 1.0);

        float depth = texture(shadowMap[index], offset).r;
        float visibility = (shadowMapCoord[index].z - bias <= depth) ? 1.0 : 0.0;
        sum += visibility;
    }

    return sum / 64.0;
}

// https://developer.download.nvidia.com/whitepapers/2008/PCSS_Integration.pdf
float PCSS(float bias, int index, float stepSize) {

	// Step 1: Blocker step
	float zBlocker = 0;

	float blockerSum = 0.0;
    int numBlockers = 0;

	float zReceiver = shadowMapCoord[index].z;

	float searchWidth = (lightSize/lightFrustrumWidth[index]) * (zReceiver - nearPlanes[index]) / zReceiver;

	for(int i = 0; i < 16; ++i) {
		vec2 offset = shadowMapCoord[index].xy + (stepSize * searchWidth * poisson[i]);
		float shadowMapDepth = texture(shadowMap[index], offset).r;
		if ( shadowMapDepth < zReceiver ) {
			blockerSum += shadowMapDepth;
			numBlockers++;
		}
	}

    if (numBlockers < 1)
        return 1.0;

	zBlocker = blockerSum / numBlockers;

	// Step 2: Penumbra size estimation 
	float penumbraRatio = (zReceiver - zBlocker) / zBlocker;
    float filterRadius = stepSize * penumbraRatio * (lightSize/lightFrustrumWidth[index]) * nearPlanes[index] / zReceiver;

	return filterRadius;
	// Step 3: PCF filtering
	return PCF(bias, filterRadius, index, stepSize);
}


float calculateShadowsCoef(vec3 n, vec3 wi)
{
	int index = 3;
	// find the appropriate depth map based on the depth of this fragment
	if(abs(viewSpacePosition.z) < farPlanes.x)
		index = 0;
	else if(abs(viewSpacePosition.z) < farPlanes.y)
		index = 1;
	else if(abs(viewSpacePosition.z) < farPlanes.z)
		index = 2;

	float bias = 0.005 * (1.0 - dot(n, wi));
	bias = clamp(bias, 0.0, 0.01);

    float stepSize = 1.0 / texelSize[index];

	if(usePCSS == 1) {
		return PCSS(bias, index, stepSize);
	} 

	float filterRadius = texelSize[index] / 1024.0;
	return PCF(bias, filterRadius, index, stepSize);
}



vec3 calculateDirectIllumiunation(vec3 wo, vec3 n, vec3 base_color)
{
	vec3 direct_illum = base_color;
	//const float distance_to_light = length(viewSpaceLightPosition - viewSpacePosition);
	//const float falloff_factor = 1.0 / (distance_to_light * distance_to_light);
	   // constant across the scene
	// Directional Light
	vec3 Li = point_light_intensity_multiplier * point_light_color; // * falloff_factor;
	vec3  wi = normalize(LightDirection);

	//vec3 wi = normalize(viewSpaceLightPosition - viewSpacePosition);

	if(dot(wi, n) <= 0.0)
		return vec3(0.0);
	float ndotwi = dot(n, wi);
	vec3 diffuse_term = (1.0 / PI) * base_color * ndotwi * Li;
	direct_illum = diffuse_term;

	vec3 wh = normalize(wi + wo);
	float ndotwh = max(0.0001, dot(n, wh));
	float ndotwo = max(0.0001, dot(n, wo));
	float wodotwh = max(0.0001, dot(wo, wh));
	float D = ((material_shininess + 2) / (2.0 * PI)) * pow(ndotwh, material_shininess);
	float G = min(1.0, min(2.0 * ndotwh * ndotwo / wodotwh, 2.0 * ndotwh * ndotwi / wodotwh));
	float F = material_fresnel + (1.0 - material_fresnel) * pow(1.0 - wodotwh, 5.0);
	F = clamp(F, 0.0, 0.8);
	float denominator = 4.0 * clamp(ndotwo * ndotwi, 0.0001, 1.0);
	float brdf = D * F * G / denominator;

	vec3 dielectric_term = brdf * ndotwi * Li + (1.0 - F) * diffuse_term;
	vec3 metal_term = brdf * base_color * ndotwi * Li;
	
	direct_illum = material_metalness * metal_term + (1.0 - material_metalness) * dielectric_term;
	
	return direct_illum;
}

vec3 calculateIndirectIllumination(vec3 wo, vec3 n, vec3 base_color)
{
	vec3 indirect_illum = vec3(0.f);

	vec3 world_normal = vec3(viewInverse * vec4(n, 0.0));
	float theta = acos(max(-1.0f, min(1.0f, world_normal.y)));
	float phi = atan(world_normal.z, world_normal.x);
	if(phi < 0.0f)
		phi = phi + 2.0f * PI;
	vec2 lookup = vec2(phi / (2.0 * PI), 1 - theta / PI);
	vec3 Li =  environment_multiplier * texture(irradianceMap, lookup).rgb;
	vec3 diffuse_term = base_color * (1.0 / PI) * Li;

	vec3 wi = normalize(reflect(-wo, n));
	vec3 wr = normalize(vec3(viewInverse * vec4(wi, 0.0)));
	float ndotwr = max(dot(world_normal, wr), 0.0);

	theta = acos(max(-1.0f, min(1.0f, wr.y)));
	phi = atan(wr.z, wr.x);
	if(phi < 0.0f)
		phi = phi + 2.0f * PI;
	lookup = vec2(phi / (2.0 * PI), 1 - theta / PI);
	float roughness = sqrt(sqrt(2.0 / (material_shininess + 2.0)));
	roughness = clamp(roughness, 0.15, 1.0);

	Li = environment_multiplier * textureLod(reflectionMap, lookup, roughness * 7.0).rgb;
	
	vec3 wh = normalize(wi + wo);
	float wodotwh = max(0.0, dot(wo, wh));
	float F = material_fresnel + (1.0 - material_fresnel) * pow(1.0 - wodotwh, 5.0);

	vec3 dielectric_term = F * Li * ndotwr + (1.0 - F) * diffuse_term;
	vec3 metal_term = F * base_color * Li;

	indirect_illum = material_metalness * metal_term + (1.0 - material_metalness) * dielectric_term;

	return indirect_illum;
}

void main()
{

	float visibility = 1.0;
	float ssao = 1.0;
//    if (enableSSAO == 1) {
//        ssao = texture(ssaoTexture, gl_FragCoord.xy / textureSize(ssaoTexture, 0).xy).r; // Fetch SSAO value
//    }
	vec3 wo = -normalize(viewSpacePosition);
	vec3 n = normalize(viewSpaceNormal);

	vec3 wi = normalize(LightDirection);

	vec3 base_color = vec3(0.1, 0.85, 0.8); // material_color;
//	if(has_color_texture == 1)
//	{
//		base_color = base_color * texture(colorMap, texCoord).rgb;
//	}
	const float shadow_ambient = 0.9;
	visibility = shadow_ambient * calculateShadowsCoef(n, wi);
	// Direct illumination
	vec3 direct_illumination_term =calculateDirectIllumiunation(wo, n, base_color) * visibility; 

	// Indirect illumination
	vec3 indirect_illumination_term = calculateIndirectIllumination(wo, n, base_color);

	///////////////////////////////////////////////////////////////////////////
	// Add emissive term. If emissive texture exists, sample this term.
	///////////////////////////////////////////////////////////////////////////
	vec3 emission_term = material_emission * material_color;
//	if(has_emission_texture == 1)
//	{
//		emission_term = texture(emissiveMap, texCoord).rgb * 0.5 ;
//	}

	vec3 shading = direct_illumination_term + indirect_illumination_term + emission_term; // (ssao * )
	
	// vec3(calculateShadowsCoef(n, wi))
	//
	fragmentColor =  vec4(vec3(viewSpacePosition.z), 1.0);

	return;
}