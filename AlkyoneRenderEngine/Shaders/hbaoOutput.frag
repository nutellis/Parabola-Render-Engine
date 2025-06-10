#version 450

precision highp float;

layout(location = 0) out vec3 fragmentColor;

layout(binding = 13) uniform sampler2D depthTexture;
layout(binding = 14) uniform sampler2D normalTexture;
layout(binding = 15) uniform sampler2D texNoise;

uniform mat4 projectionMatrix;

///////////////////////////////////////////////////////////////////////////////
// Parameters
///////////////////////////////////////////////////////////////////////////////

uniform float radius; //search radius
uniform float bias; // helps with false-occlusion and cleaner results

uniform int Nd; // number of directions
uniform int Ns; // number of steps per direction

uniform float powerExponent; // controls the darkness of the final AO.

// Viewport information
uniform float fieldOfView; // in radians
uniform float aspectRatio; 

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////
#define PI 3.14159265359


///////////////////////////////////////////////////////////////////////////////
// Helper Functions
///////////////////////////////////////////////////////////////////////////////
vec3 ReconstructViewPosition(float depth, vec2 uvPosition) {

	vec2 ndc = uvPosition * 2.0 - 1.0;

	// We go from NDC to view space position directly. Most of these calculations can be moved to the CPU
	float tanHalfFovY = tan(fieldOfView * 0.5);
	float tanHalfFovX = tanHalfFovY * aspectRatio;

	float x = ndc.x * tanHalfFovX;
	float y = ndc.y * tanHalfFovY;
	float z = 1.0;

	vec3 viewSpacePosition = vec3(x, y, z) * depth;
	
	return viewSpacePosition;
}

vec2 rotateDirection(vec2 direction, vec2 angle) {
	return vec2(direction.x * angle.x - direction.y * angle.y, direction.x * angle.y + direction.y * angle.x);
}


///////////////////////////////////////////////////////////////////////////////
// Image Space Horizon Based Ambient Occlusion (HBAO)
///////////////////////////////////////////////////////////////////////////////
float AccumulateAO(vec2 rayDirection, vec3 viewSpacePosition, vec3 normal) {
		
	// Sample the depth at the AO ray direction
	float sampleDepth = texture(depthTexture, rayDirection).r;

	vec3 sampleViewSpacePosition = ReconstructViewPosition(sampleDepth, rayDirection);
	
	//The actual vector of the sample in view space
	vec3 sampleVector = sampleViewSpacePosition - viewSpacePosition;
	
	// we need to calculate the angle of the sample to the normal, so we need the dot
	//dot = N * V / ||N|| * ||V|| = cos(theta)
	//N is a unit vector so we can remove it.

	// what is left is to calculate the 1 / ||V||
	float VdotV = dot(sampleVector,sampleVector);
	
	float NdotV = dot(normal, sampleVector) * inversesqrt(VdotV);
	
	// we need to apply the bias to the cosTheta value
	NdotV = NdotV - bias; // bias is subtracted to avoid self occlusion
	
	// calculate the attenuation function and clamp it
	float attenuation = clamp((VdotV / (radius * radius) + 1.0), 0.0, 1.0);

	return clamp(NdotV * attenuation, 0.0, 1.0); // accumulate the AO value
}


float ComputeHBAO(vec3 viewSpacePosition, vec3 normal) {
	float AO = 0;
	float screenHeight = textureSize(depthTexture, 0).y;

	const float anglePerDirection = 2.0 * PI / Nd; 

	vec2 texelSize = 1.0 / textureSize(depthTexture, 0);

	// calculating radius in screen space (this can be moved to the cpu)
	float Pz = viewSpacePosition.z;

	// screen space radius
	float R = radius * 0.5 / tan(fieldOfView * 0.5) * screenHeight;
		
	float radiusPixels = R / Pz;

	float minTexel = min(texelSize.x, texelSize.y);
	if(radiusPixels < minTexel) {
		return 1.0;
	}
	
	float stepSizePixels = (radiusPixels) / (Ns + 1);
			
	for(int directionIndex = 0; directionIndex < Nd; ++directionIndex) {
		
		//calculate this direction's angle
		float angle = anglePerDirection * float(directionIndex);

		//calculate the unit vector of the direction
		vec2 direction = vec2(cos(angle), sin(angle));
		
		// apply jitter based on the precomputed noise map.

		// sample the texture (cos(a), sin(a), beta)
		vec3 noise = texture(texNoise, gl_FragCoord.xy * 1.0 / textureSize(texNoise, 0)).xyz;
		
		// jitter the direction by rotating to cos, sin (cos(a), sin(a) values)
		direction = rotateDirection(direction, noise.xy);

		// we also jitter the first sample in the direction (beta value)
		float rayStepPixels = stepSizePixels * noise.z + 1.0;; //   // the +1 is added to avoid self occlusion in case noise.z is zero.

		vec2 S0 = direction * rayStepPixels;
		
		// we now snap the step position to the nearest texel.
		//This may create banding for high Ns or small radius beacause we are limiting possible directions.
		S0 = floor(S0 / texelSize + 0.5) * texelSize;
		
		//accumulate the first step.
		AO += AccumulateAO(S0, viewSpacePosition, normal);

		// ray march along the i-th direction for step+1
		for (int stepIndex = 1; stepIndex < Ns; ++stepIndex) {
			// advance step
			rayStepPixels += stepSizePixels;

			S0 = direction * rayStepPixels;

			// calculate
			AO += AccumulateAO(S0, viewSpacePosition, normal);
		}
	}
	float NdNs = float(Nd * Ns);
	AO /= NdNs; // normalize the AO value

	return AO ;
}

void main() {

	// we are using full resolution calculations

	vec2 pixelCenter = floor(gl_FragCoord.xy) + 0.5;  
	vec2 uv = pixelCenter / textureSize(depthTexture, 0);      

    // Depth is in viewSpace already
	float depthUv = texture(depthTexture, uv).r;
    vec3 normal = texture(normalTexture, uv).rgb;

	normal = normalize(normal);

    // Reconstruct position in view-space from the depth map
	vec3 viewSpacePosition = ReconstructViewPosition(depthUv, uv);

	float hbao = ComputeHBAO(viewSpacePosition, normal);

	hbao = clamp(1.0 - hbao * 2.0, 0.0, 1.0);

	hbao = pow(hbao, powerExponent);

	fragmentColor = vec3(hbao);

   return;
}