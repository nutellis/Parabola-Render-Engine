#version 450

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
uniform int has_metalness_texture = 0;
layout(binding = 2) uniform sampler2D metalnessMap;
uniform int has_fresnel_texture = 0;
layout(binding = 3) uniform sampler2D  fresnelMap;
uniform int has_shininess_texture = 0;
layout(binding = 4) uniform sampler2D  roughnessMap;
uniform int has_emission_texture = 0;
//layout(binding = 5) uniform sampler2D emissiveMap;

struct Material {
	vec3 colour;
	float metalness;
	float fresnel;
	float roughness;
	vec3 emission;
};

///////////////////////////////////////////////////////////////////////////////
// Environment
///////////////////////////////////////////////////////////////////////////////
//layout(binding = 6) uniform sampler2D environmentMap;
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
layout(binding = 20) uniform sampler2D normalMap;


///////////////////////////////////////////////////////////////////////////////
// Input uniform variables
///////////////////////////////////////////////////////////////////////////////
uniform mat4 viewInverse;


///////////////////////////////////////////////////////////////////////////////
// Light source
///////////////////////////////////////////////////////////////////////////////

uniform vec3 point_light_color = vec3(1.0, 1.0, 1.0);
uniform float point_light_intensity_multiplier = 1.5;

uniform vec3 LightPosition;
uniform vec3 LightDirection;

uniform vec3 viewSpaceLightPosition;
uniform vec3 viewSpaceLightDir;

///////////////////////////////////////////////////////////////////////////////
//SSAO
///////////////////////////////////////////////////////////////////////////////
uniform int enableSSAO = 0;
layout(binding = 16) uniform sampler2D ssaoTexture;

///////////////////////////////////////////////////////////////////////////////
//Cascade Shadow Information
///////////////////////////////////////////////////////////////////////////////
// textures 10 to 13
uniform int enableCSM = 0;
uniform sampler2D shadowMap[4];


uniform int numOfCascades; 

in vec4 lightSpacePosition[4];
uniform vec4 farPlanes;

///////////////////////////////////////////////////////////////////////////////
// PCSS
///////////////////////////////////////////////////////////////////////////////
uniform int usePCSS = 0;
float lightSize = 1; 
uniform vec4 lightFrustrumWidth; 
uniform vec4 near;
uniform vec4 far;
uniform vec4 resolution;

const vec2 poisson[64] = vec2[](
    vec2(-0.613392, 0.617481),
    vec2(0.170019, -0.040254),
    vec2(-0.299417, 0.791925),
    vec2(0.645680, 0.493210),
    vec2(-0.651784, 0.717887),
    vec2(0.421003, 0.027070),
    vec2(-0.817194, -0.271096),
    vec2(-0.705374, -0.668203),
    vec2(0.977050, -0.108615),
    vec2(0.063326, 0.142369),
    vec2(0.203528, 0.214331),
    vec2(-0.667531, 0.326090),
    vec2(-0.098422, -0.295755),
    vec2(-0.885922, 0.215369),
    vec2(0.566637, 0.605213),
    vec2(0.039766, -0.396100),
    vec2(0.751946, 0.453352),
    vec2(0.078707, -0.715323),
    vec2(-0.075838, -0.529344),
    vec2(0.724479, -0.580798),
    vec2(0.222999, -0.215125),
    vec2(-0.467574, -0.405438),
    vec2(-0.248268, -0.814753),
    vec2(0.354411, -0.887570),
    vec2(0.175817, 0.382366),
    vec2(0.487472, -0.063082),
    vec2(-0.084078, 0.898312),
    vec2(0.488876, -0.783441),
    vec2(0.470016, 0.217933),
    vec2(-0.696890, -0.549791),
    vec2(-0.149693, 0.605762),
    vec2(0.034211, 0.979980),
    vec2(0.503098, -0.308878),
    vec2(-0.016205, -0.872921),
    vec2(0.385784, -0.393902),
    vec2(-0.146886, -0.859249),
    vec2(0.643361, 0.164098),
    vec2(0.634388, -0.049471),
    vec2(-0.688894, 0.007843),
    vec2(0.464034, -0.188818),
    vec2(-0.440840, 0.137486),
    vec2(0.364483, 0.511704),
    vec2(0.034028, 0.325968),
    vec2(0.099094, -0.308023),
    vec2(0.693960, -0.366253),
    vec2(0.678884, -0.204688),
    vec2(0.001801, 0.780328),
    vec2(0.145177, -0.898984),
    vec2(0.062655, -0.611866),
    vec2(0.315226, -0.604297),
    vec2(-0.780145, 0.486251),
    vec2(-0.371868, 0.882138),
    vec2(0.200476, 0.494430),
    vec2(-0.494552, -0.711051),
    vec2(0.612476, 0.705252),
    vec2(-0.578845, -0.768792),
    vec2(-0.772454, -0.090976),
    vec2(0.504440, 0.372295),
    vec2(0.155736, 0.065157),
    vec2(0.391522, 0.849605),
    vec2(-0.620106, -0.328104),
    vec2(0.789239, -0.419965),
    vec2(-0.545396, 0.538133),
    vec2(-0.178564, -0.596057)
);

///////////////////////////////////////////////////////////////////////////////
// Output color
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec4 fragmentColor;

float PCF(vec2 projectedPos, float currentDepth, float filterRadius, int index) {
    float sum = 0.0;

	float stepSize = 1.0 / resolution[index];

	float depth = texture(shadowMap[index], projectedPos.xy).r;
    float visibility = (currentDepth <= depth) ? 1.0 : 0.0;
	if (visibility == 1.0) {
		return 1.0;
	}

    for (int x = 0; x < 64; x++) {

        vec2 offset = projectedPos.xy + stepSize * poisson[x] * filterRadius;

        float depth = texture(shadowMap[index], offset).r;
        float visibility = (currentDepth <= depth) ? 1.0 : 0.0;
        sum += visibility;
    }
    return sum / 64.0;
}

// https://developer.download.nvidia.com/whitepapers/2008/PCSS_Integration.pdf
float PCSS(vec3 projectedPos, float bias, int index) {

	// Step 1: Blocker step
	float zBlocker = 0;

	float blockerSum = 0.0;
    float numBlockers = 0.0;
	
	float zReceiver = projectedPos.z;

	float searchWidth = lightSize/lightFrustrumWidth[index] * (zReceiver - near[index]) / zReceiver;

	for(int i = 0; i < 16; ++i) {
		vec2 offset = projectedPos.xy + searchWidth * poisson[i];
		float shadowMapDepth = texture(shadowMap[index],offset).r;
		if (shadowMapDepth < zReceiver) {
			blockerSum += shadowMapDepth;
			numBlockers++;
		}
	}
	
	 zBlocker = blockerSum / numBlockers;

	 if(numBlockers < 1.0) {
		return 1.0;
	}

	// Step 2: Penumbra size estimation 
	float penumbraRatio = (zReceiver - zBlocker) / zBlocker;

	float filterRadiusUV = penumbraRatio * lightSize/lightFrustrumWidth[index] * near[index] / zReceiver; 
	//fragmentColor = vec4(vec3(penumbraRatio), 1.0);
	return PCF(projectedPos.xy, zReceiver - bias, filterRadiusUV, index);
}


float calculateShadowsCoef(vec3 n, vec3 wi)
{
	float viewDepth = abs(viewSpacePosition.z);
	
	int cascadeIndex = 3;
	// find the appropriate depth map based on the depth of this fragment
	if(abs(viewSpacePosition.z) < farPlanes.x)
		cascadeIndex = 0;
	else if(abs(viewSpacePosition.z) < farPlanes.y)
		cascadeIndex = 1;
	else if(abs(viewSpacePosition.z) < farPlanes.z)
		cascadeIndex = 2;

	float bias = 0.000001 * (1.0 - dot(n, wi));
	//bias = max(bias, 0.01);

	float filterRadius = 0.0;

	vec3 projectedPos = lightSpacePosition[cascadeIndex].xyz / lightSpacePosition[cascadeIndex].w ;
	projectedPos = projectedPos * 0.5 + 0.5;

	if(projectedPos.z < 0.0 || projectedPos.z > 1.0) {
		return 1.0;
	}

	if(usePCSS == 1) {
		return PCSS(projectedPos, bias, cascadeIndex);
	} else {

		filterRadius = resolution[cascadeIndex] / 1024.0;
		return PCF(projectedPos.xy, projectedPos.z - bias, filterRadius, cascadeIndex);
	}
}



vec3 calculateDirectIllumiunation(vec3 wo, vec3 n, Material mat)
{
	vec3 direct_illum = mat.colour;
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
	vec3 diffuse_term = (1.0 / PI) * mat.colour * ndotwi * Li;
	direct_illum = diffuse_term;

	vec3 wh = normalize(wi + wo);
	float ndotwh = max(0.0001, dot(n, wh));
	float ndotwo = max(0.0001, dot(n, wo));
	float wodotwh = max(0.0001, dot(wo, wh));
	float D = ((mat.roughness + 2) / (2.0 * PI)) * pow(ndotwh, mat.roughness);
	float G = min(1.0, min(2.0 * ndotwh * ndotwo / wodotwh, 2.0 * ndotwh * ndotwi / wodotwh));
	float F = mat.fresnel + (1.0 - mat.fresnel) * pow(1.0 - wodotwh, 5.0);
	F = clamp(F, 0.0, 0.8);
	float denominator = 4.0 * clamp(ndotwo * ndotwi, 0.0001, 1.0);
	float brdf = D * F * G / denominator;

	vec3 dielectric_term = brdf * ndotwi * Li + (1.0 - F) * diffuse_term;
	vec3 metal_term = brdf * mat.colour * ndotwi * Li;
	
	direct_illum = mat.metalness * metal_term + (1.0 - mat.metalness) * dielectric_term;
	
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


Material initMaterial() {
	Material mat;
	vec3 base_color = vec3(1, 1, 1);// material_color; // //
	if(has_color_texture == 1)
	{
		mat.colour = base_color * texture(colorMap, texCoord).rgb;
	} else {
		mat.colour = material_color;
	}
	if(has_metalness_texture == 1)
	{
		mat.metalness = texture(metalnessMap, texCoord).b;
	} else {
		mat.metalness = material_metalness;
	}
	if(has_shininess_texture == 1)
	{
		mat.roughness = texture(roughnessMap, texCoord).g;
	} else {
		mat.roughness = material_shininess;
	}
	if(has_fresnel_texture == 1)
	{
		mat.fresnel = texture(fresnelMap, texCoord).g;
	} else {
		mat.fresnel = material_fresnel;
	}

	

	return mat;
}

void main()
{

	float visibility = 1.0;
	float ssao = 1.0;
    if (enableSSAO == 1) {
		ssao = texture(ssaoTexture, gl_FragCoord.xy / textureSize(ssaoTexture, 0).xy).r; // Fetch SSAO value
    }
	vec3 wo = -normalize(viewSpacePosition);

	vec3 n = normalize(viewSpaceNormal);
	
	//texture(normalMap, gl_FragCoord.xy / textureSize(normalMap, 0).xy).xyz;


	vec3 wi = normalize(LightDirection);

	vec3 base_color = vec3(1, 1, 1);// material_color; // //
	if(has_color_texture == 1)
	{
		base_color = base_color * texture(colorMap, texCoord).rgb;
	}
	const float shadow_ambient = 0.9;

	if (enableCSM == 1) {
		visibility = shadow_ambient * calculateShadowsCoef(n, wi);
	}

	Material mat = initMaterial();

	// Direct illumination
	vec3 direct_illumination_term = calculateDirectIllumiunation(wo, n, mat) * visibility; 

	// Indirect illumination
	vec3 indirect_illumination_term = calculateIndirectIllumination(wo, n, base_color) * ssao;

	///////////////////////////////////////////////////////////////////////////
	// Add emissive term. If emissive texture exists, sample this term.
	///////////////////////////////////////////////////////////////////////////
	vec3 emission_term = material_emission * material_color;
//	if(has_emission_texture == 1)
//	{
//		emission_term = texture(emissiveMap, texCoord).rgb * 0.5 ;
//	}

	vec3 shading = direct_illumination_term +  indirect_illumination_term + emission_term; 


	fragmentColor = vec4(vec3(shading), 1.0);
	
	return;
}