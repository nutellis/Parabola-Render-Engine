#version 450

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

///////////////////////////////////////////////////////////////////////////////
// Environment
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec3 normalColor;

layout(binding = 21) uniform sampler2D depthTexture; 
///////////////////////////////////////////////////////////////////////////////
// Input varyings from vertex shader
///////////////////////////////////////////////////////////////////////////////

in vec2 texCoord;

uniform float fieldOfView; // in radians
uniform float aspectRatio; 
uniform float width;
uniform float height;

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

vec3 MinDiff(vec3 P, vec3 Pr, vec3 Pl)
{
    vec3 V1 = Pr - P;
    vec3 V2 = P - Pl;

	float dotV1 = dot(V1,V1);

	float dotV2 = dot(V2,V2);

	// return the vector with the smallest squared length
	if (dotV1 < dotV2) return V1;
	else return V2;
}

vec3 ReconstructNormal(float depth, vec2 uv, vec3 viewSpacePosition) {
	
	float depthR = texture(depthTexture, uv + vec2(1.0 / width, 0)).r;
	float depthL = texture(depthTexture, uv + vec2(-1.0 / width, 0)).r;
	float depthT = texture(depthTexture, uv + vec2(0, 1.0 / height)).r;
	float depthB = texture(depthTexture,  uv + vec2(0, -1.0 / height)).r;

	vec3 Pr = ReconstructViewPosition(depthR, uv + vec2(1.0 / width, 0));
    vec3 Pl = ReconstructViewPosition(depthL, uv + vec2(-1.0 / width, 0));
    vec3 Pt = ReconstructViewPosition(depthT, uv + vec2(0, 1.0 / height));
    vec3 Pb = ReconstructViewPosition(depthB, uv + vec2(0, -1.0 / height));

	vec3 y = MinDiff(viewSpacePosition, Pt, Pb);
	vec3 x = MinDiff(viewSpacePosition, Pr, Pl);
	vec3 normal = normalize(cross(y, x));

	return normal * 0.5 + 0.5;

}

void main()
{
	vec2 pixelCenter = floor(gl_FragCoord.xy) + 0.5;  
	vec2 uv = pixelCenter / textureSize(depthTexture, 0);      

	float linearDepth = texture(depthTexture, uv).r;

	//calculate the view position
	vec3 viewPosition = ReconstructViewPosition(linearDepth, uv);

	// for this position calculatethe view space normal
	vec3 viewNormal = ReconstructNormal(linearDepth, uv, viewPosition);

	normalColor = viewNormal;

}