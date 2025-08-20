#pragma once
#ifndef CASCADE_SHADOW_MAP_H
#define CASCADE_SHADOW_MAP_H

#include <Core/RenderCore.h>
#include <Components/RenderTarget.h>
#include <Components/Texture.h>
#include <Components/Shader.h>
#include <Utilities/PrimitiveShapes.h>

class FBORenderTarget;
class RTexture;
class PCameraComponent;
class PDirectionalLightComponent;
class Shader;
class PFrustrum;

#define MAX_CASCADES 4


struct RCascade {

public:
	uint32 Resolution = 4096;
	
	FBORenderTarget* CascadeFBO;

	Matrix4f CascadePVMatrix;

	Matrix4f LightViewMatrix;
	Matrix4f LightProjectionMatrix;
	Matrix4f CropMatrix;

	PFrustrum * Frustrum;

	Vector4f CascadeDebugColour;

	//TODO: this needs a cleanup
	float Near;
	float Far;

	RCascade();
	RCascade(std::string Name, uint32 Width, uint32 Height);
	~RCascade();
	RCascade* CalculateStep();

};

class RCascadeShadowMap
{
public:
	RCascadeShadowMap();
	RCascadeShadowMap(uint32 InNumCascades, float InFieldOfView, float AspectRatio, float ZNear, float ZFar);
	//RCascadeShadowMap(uint32 InNumCascades, float ZNear, float ZFar);
	//RCascadeShadowMap(uint32 NumCascades = 4);
	~RCascadeShadowMap();
	void Init();
	void UpdateCascadeExtends(float ZNear, float ZFar, float Ratio, float FieldOfView);
	void CalculateLightProjection(uint32 Index, PCameraComponent* Camera, PDirectionalLightComponent* Light, bool SquareBox);
	void UpdateCascadeBuffer(Matrix4f CameraViewMatrix);
	void Draw(uint32 Index);
	void BindSSBO(uint32 Slot);
	void CleanUp();
	void PrepareForDraw(Shader* ActiveShader, Matrix4f CameraViewMatrix, Matrix4f CameraProjectionMatrix, bool UsePCSS = false);
	void SetViewport(uint32 Width, uint32 Height);

	RCascade * GetCascade(uint32 Index);

public:
	uint32 NumCascades;
	TArray<RCascade *> Cascades;

	float Lambda = 0.25f;
	float TransitionOverlap = 0.95;
	float LightDistance = 50.0f;

	/* Options */
	bool ShowCascadeFrustrumDebug = false;
	bool ShowCascadeBoundingDebug = false;
	bool SoloCascadeDebug = false;
	bool ShowLightFrustrumDebug = false;
	int32 CascadeIndex = 1;
	bool ShowShadowMapDebug = false;
	bool SquareShadowBox = false;
	bool ShowCascade[4] = { true, true, true, true };
	bool ToggleShadowView = false;
	bool ShouldUpdate = false;

private:
	uint32 ShadowMapSSBO;
	uint32 CascadesMVPBuffer;
};





#endif //CASCADE_SHADOW_MAP_H