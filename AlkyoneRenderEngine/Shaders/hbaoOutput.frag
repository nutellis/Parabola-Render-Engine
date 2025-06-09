#version 450

precision highp float;

layout(location = 0) out vec3 fragmentColor;

layout(binding = 13) uniform sampler2D normalTexture; // viewSpace
layout(binding = 14) uniform sampler2D depthTexture;
layout(binding = 15) uniform sampler2D texNoise;

uniform mat4 projectionMatrix;

///////////////////////////////////////////////////////////////////////////////
// Parameters
///////////////////////////////////////////////////////////////////////////////

uniform float radius; //search radius
uniform float bias; // helps with false-occlusion and cleaner results

//#define MAX_SAMPLES 128
//uniform vec2 directions[MAX_SAMPLES];
uniform float Nd; // number of directions
uniform float Ns; // number of steps per direction

uniform float powerExponent; // controls the darkness of the final AO.

// Viewport information
uniform float fieldOfView; // in radians
uniform float aspectRatio; 
float screenHeight = 768.0; 

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////
#define PI 3.14159265359


///////////////////////////////////////////////////////////////////////////////
// Helper Functions
///////////////////////////////////////////////////////////////////////////////
vec3 homogenize(vec4 v) { return v.xyz / v.w; }

float cot(float x) { return 1.0 / tan(x); }

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


///////////////////////////////////////////////////////////////////////////////
// Image Space Horizon Based Ambient Occlusion (HBAO)
///////////////////////////////////////////////////////////////////////////////
void AccumulateAO(inout float AO, inout vec2 rayDirection, vec3 viewSpacePosition, vec3 normal) {
	
	//get sample's position
	float depth = texture(depthTexture, rayDirection).r;
	//fragmentColor = vec3(depth);
	vec3 sampleViewSpacePosition = ReconstructViewPosition(depth, rayDirection);
	//fragmentColor = sampleViewSpacePosition;
	//The actual vector of the sample in view space
	vec3 sampleVector = sampleViewSpacePosition - viewSpacePosition;

	// we need to calculate the angle of the sample to the normal, so we need the dot
	//dot = N * V / ||N|| * ||V|| = cos(theta)
	//N is a unit vector so we can remove it.

	// what is left is to calculate the 1 / ||V||
	float sampleLength = inversesqrt(dot(sampleVector,sampleVector));
	
	float cosTheta = dot(normal, sampleVector) * sampleLength;

	// we need to apply the bias to the cosTheta value
	float NdotV = cosTheta - bias; // bias is subtracted to avoid self occlusion

	// if the sample is behind the normal, we skip it
	if (NdotV < 0.0) {
		// we will see what needs to be done here
	}
	// calculate the attenuation function and clamp it
	float attenuation = clamp(1.0 - (sampleLength / radius * radius), 0.0, 1.0);

	AO += clamp(NdotV * attenuation, 0.0, 1.0); // accumulate the AO value
}



float ComputeHBAO(vec3 viewSpacePosition, vec3 normal) {
	float AO = 0;

	const float anglePerDirection = 2.0 * PI / Nd; 

	vec2 texelSize = 1.0 / textureSize(depthTexture, 0);

	// calculating radius in screen space (this can be moved to the cpu)
	float Pz = viewSpacePosition.z;

	// vertical focal length
	float fy= cot(fieldOfView * 0.5);
	// horizontal focal length
	float fx = fy * aspectRatio;

	vec2 f = vec2(fx,fy);

	vec2 R = f * radius / Pz;

	fragmentColor = vec3(Pz.x,0.0, 0.0);
	vec2 stepSize = (R * 0.5) / (Ns + 1); // might need to check if  /4 is needed

	int directionIndex = 0;
	//for(int directionIndex = 0; directionIndex < Nd; ++directionIndex) {
		
		//calculate this direction's angle
		float angle = anglePerDirection * float(directionIndex);

		//calculate the unit vector of the direction
		vec2 direction = vec2(cos(angle), sin(angle));

		// apply jitter based on the precomputed noise map.

		// sample the texture (cos(a), sin(a), beta)
		//vec3 noise = texture(texNoise, gl_FragCoord.xy * texelSize * textureSize(texNoise,0).x).xyz;

		// jitter the direction by rotating to cos, sin (cos(a), sin(a) values)
	//	direction.x = direction.x * noise.x - direction.y * noise.y;
	//	direction.y = direction.x * noise.y + direction.y * noise.x;
//
//		// we also jitter the first sample in the direction (beta value)
	//	vec2 stepPosition = stepSize * noise.z + 1.0; // the +1 is added to avoid self occlusion in case noise.z is zero.

		vec2 S0 = direction * stepSize;
			
		// we now snap the step position to the nearest texel.
		//This may create banding for high Ns or small radius beacause we are limiting possible directions.
		//S0 = floor(S0 / texelSize + 0.5) * texelSize;
		
		//accumulate the first step.
		AccumulateAO(AO, S0, viewSpacePosition, normal);

		
		// advance step
		S0 += stepSize;
	
		// ray march along the i-th direction for step+1
//		for (int stepIndex = 1; stepIndex < Ns; ++stepIndex) {
//			AccumulateAO(AO, S0, viewSpacePosition, normal);
//		}
	//}
	//AO /= (Nd * Ns); // normalize the AO value

	return AO ;
	}

void main() {

	// we are using full resolution calculations

	vec2 pixelCenter = floor(gl_FragCoord.xy) + 0.5;  
	vec2 uv = pixelCenter / textureSize(depthTexture, 0);      

    // Depth is in viewSpace already
	float depthUv = texture(depthTexture, uv).r;
    vec3 normal = texture(normalTexture, uv).rgb;

	
	// this outputs the depth correctly.
	//fragmentColor = vec3(depthUv);

    // Reconstruct position in view-space from the depth map
	vec3 viewSpacePosition = ReconstructViewPosition(depthUv, uv);

	float hbao = ComputeHBAO(viewSpacePosition, normal);

	//hbao = clamp(1.0 - hbao * 2.0, 0.0, 1.0);

	//hbao = pow(hbao, powerExponent);

   //fragmentColor = vec3(hbao);

   return;
}