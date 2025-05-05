#version 450
#extension GL_ARB_bindless_texture : enable
#extension GL_NV_gpu_shader5 : enable
#extension GL_ARB_gpu_shader_int64: enable

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
// Shadow Information
///////////////////////////////////////////////////////////////////////////////
layout(std430, binding = 0) buffer TextureHandles {
    uint64_t handles[]; // Array of texture handles
};
// textures 10 to 13
uniform sampler2DShadow shadowMap[4];


in vec4 shadowMapCoord[4];
uniform vec4 FragmentDistance;


layout(binding = 16) uniform sampler2D ssaoTexture;

///////////////////////////////////////////////////////////////////////////////
// Output color
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec4 fragmentColor;

float shadowCoef()
{
	int index = 3;
	
	// find the appropriate depth map to look up in based on the depth of this fragment
	if(gl_FragCoord.z < FragmentDistance.x)
		index = 0;
	else if(gl_FragCoord.z < FragmentDistance.y)
		index = 1;
	else if(gl_FragCoord.z < FragmentDistance.z)
		index = 2;

	
	// get the texture handle of the shadow map
	 uint64_t shadowHandle = handles[index];

	vec3 shadowUV = shadowMapCoord[index].xyz / shadowMapCoord[index].w;

	float shadowDepth = 0;
//	#if defined(USE_HARDWARE_SHADOWS)
//		shadowDepth = textureProj(shadowMap[index], shadowMapCoord[index]);
//	#else
		shadowDepth = textureProj(shadowMap[index], shadowMapCoord[index]);
	//#endif

	//float bias = 0.002;
	float diff = shadowDepth - shadowUV.z; // - bias;
	 
	return clamp(diff * 250.0 + 1.0, 0.0, 1.0);
}

vec4 debugShadows() {
	int index = 3;
	
	// find the appropriate depth map to look up in based on the depth of this fragment
	if(gl_FragCoord.z < FragmentDistance.x)
		index = 0;
	else if(gl_FragCoord.z < FragmentDistance.y)
		index = 1;
	else if(gl_FragCoord.z < FragmentDistance.z)
		index = 2;

	vec3 uvw = shadowMapCoord[index].xyz / shadowMapCoord[index].w;
	return vec4(uvw.xy, 0.0, 1.0); // just visualize light-space UV

}


vec3 calculateDirectIllumiunation(vec3 wo, vec3 n, vec3 base_color)
{
	vec3 direct_illum = base_color;
	//const float distance_to_light = length(viewSpaceLightPosition - viewSpacePosition);
	//const float falloff_factor = 1.0 / (distance_to_light * distance_to_light);
	   // constant across the scene

	vec3 Li = point_light_intensity_multiplier * point_light_color; // * falloff_factor;
	vec3  wi = LightDirection;

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

	vec3 base_color = material_color;
	if(has_color_texture == 1)
	{
		base_color = base_color * texture(colorMap, texCoord).rgb;
	}
	const float shadow_ambient = 0.9;
	visibility = shadow_ambient * shadowCoef();
	// Direct illumination
	vec3 direct_illumination_term = visibility * calculateDirectIllumiunation(wo, n, base_color);

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
	
	fragmentColor =  vec4(shading, 1.0); //debugShadows(); //

	return;
}