#version 450

precision highp float;

layout(location = 0) out vec3 fragmentColor;

layout(binding = 13) uniform sampler2D viewSpaceNormalTexture;
layout(binding = 14) uniform sampler2D depthTexture;

///////////////////////////////////////////////////////////////////////////////
// Input varyings from vertex shader
///////////////////////////////////////////////////////////////////////////////
uniform mat4 projectionMatrix;

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////
#define PI 3.14159265359

#define MAX_SAMPLES 128
uniform vec3 samples[MAX_SAMPLES];
uniform int	sampleSize;

uniform int filterSize = 1;

layout(binding = 15) uniform sampler2D texNoise;


///////////////////////////////////////////////////////////////////////////////
// Helper Functions
///////////////////////////////////////////////////////////////////////////////
vec3 homogenize(vec4 v) { return v.xyz / v.w; }

vec4 textureRect(in sampler2D tex, vec2 rectangleCoord)
{
	return texture(tex, rectangleCoord / textureSize(tex, 0));
}

float ComputeSSAO(float depth, vec3 viewSpacePosition, vec3 viewSpaceNormal) {
	float hemisphere_radius = 0.5;
	int num_valid_samples = 0;
    int num_visible_samples = 0;

	float occlusion = 0.0;

	float bias = 0.025;

	vec3 randomNoise = texture(texNoise, gl_FragCoord.xy / vec2(64.0)).xyz;

	vec3 viewSpaceTangent =  normalize(randomNoise - viewSpaceNormal * dot(randomNoise, viewSpaceNormal));
	vec3 viewSpaceBitangent = cross(viewSpaceNormal, viewSpaceTangent);

	mat3 tbn = mat3(viewSpaceTangent, viewSpaceBitangent, viewSpaceNormal); // local base

    for (int i = 0; i < sampleSize; i++) {

        // Project an hemishere sample onto the local base
        vec3 samplePosition = tbn * samples[i];

        // compute view-space position of sample
        vec3 samplePositionViewSpace = viewSpacePosition + samplePosition * hemisphere_radius;

        // compute the ndc-coords of the sample
		vec3 sampleCoordsClipSpace = homogenize(projectionMatrix * vec4(samplePositionViewSpace, 1.0));

        // Sample the depth-buffer at a texture coord based on the ndc-coord of the sample
		vec2 sampleCoordsUVSpace = sampleCoordsClipSpace.xy * 0.5 + 0.5;
		float blockerDepth = texture(depthTexture, sampleCoordsUVSpace.xy).r;

		vec3 blockerPositionViewSpace = homogenize(inverse(projectionMatrix) * 
            vec4(sampleCoordsClipSpace.xy, blockerDepth * 2.0 - 1.0, 1.0));


		if(blockerPositionViewSpace.z < samplePositionViewSpace.z) {
			num_valid_samples++;
			num_visible_samples++;
		} else if(distance(blockerPositionViewSpace, viewSpacePosition) < hemisphere_radius) {
			num_valid_samples++;
		}

		if (num_valid_samples > 0)
		{
			occlusion = float(num_visible_samples) / float(num_valid_samples);
		}
	}
	return occlusion;
}


vec3 ReconstructViewPosition(float fragmentDepth) {
	
	vec2 uv = gl_FragCoord.xy / textureSize(viewSpaceNormalTexture, 0);
	// Normalized Device Coordinates (clip space)
	vec4 ndc = vec4(uv.x * 2.0 - 1.0, uv.y * 2.0 - 1.0, fragmentDepth * 2.0 - 1.0, 1.0);

	// Transform to view space
	vec3 viewSpacePosition = homogenize(inverse(projectionMatrix) * ndc);
	
	return viewSpacePosition;
}


void main() {
    // Fetch depth and normal data
	float depth = textureRect(depthTexture, gl_FragCoord.xy).r;
    vec3 viewSpaceNormal = textureRect(viewSpaceNormalTexture, gl_FragCoord.xy).rgb;

    // Reconstruct position in view-space
	vec3 fragPos = ReconstructViewPosition(depth);

    float occlusion = ComputeSSAO(depth, fragPos, viewSpaceNormal);

    fragmentColor = vec3(occlusion);
}