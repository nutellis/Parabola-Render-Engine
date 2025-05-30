#pragma once
#ifndef CASCADE_SHADOW_MAP_H
#define CASCADE_SHADOW_MAP_H

#include <Core/RenderCore.h>
#include <Components/RenderTarget.h>
#include <Components/Texture.h>
#include <Components/Shader.h>
#include <Utilities/PrimitiveShapes.h>

class FBORenderTarget;
class Texture;
class PCameraComponent;
class PDirectionalLightComponent;
class Shader;
class PFrustrum;

#define MAX_CASCADES 4


struct PCascade {

public:
	uint32 Resolution = 4096;
	
	FBORenderTarget* CascadeFBO;

	Matrix4f CascadeCPVMatrix;

	Matrix4f LightViewMatrix;
	Matrix4f LightProjectionMatrix;
	Matrix4f CropMatrix;

	PFrustrum * Frustrum;

	Vector4f CascadeDebugColour;

	PCascade();
	~PCascade();
	PCascade* CalculateStep();

};

class PCascadeShadowMap
{
public:
	PCascadeShadowMap();
	PCascadeShadowMap(uint32 InNumCascades, float InFieldOfView, float AspectRatio, float ZNear, float ZFar);
	//PCascadeShadowMap(uint32 InNumCascades, float ZNear, float ZFar);
	//PCascadeShadowMap(uint32 NumCascades = 4);
	~PCascadeShadowMap();
	void Init();
	void UpdateCascadeExtends(float ZNear, float ZFar, float Ratio, float FieldOfView);
	void CalculateLightProjection(uint32 Index, PCameraComponent* Camera, PDirectionalLightComponent* Light, bool SquareBox);
	void UpdateCascadeBuffer(Matrix4f CameraViewMatrix);
	void Draw(uint32 Index);
	void BindSSBO(uint32 Slot);
	void UnbindBuffers();
	void PrepareForDraw(Shader* ActiveShader, Matrix4f CameraViewMatrix, Matrix4f CameraProjectionMatrix, bool UsePCSS = false);
	void SetViewport(uint32 Width, uint32 Height);

	PCascade * GetCascade(uint32 Index);

public:
	uint32 NumCascades;
	TArray<PCascade *> Cascades;

	float Lambda = 0.5f;

private:
	uint32 ShadowMapSSBO;
	uint32 CascadesMVPBuffer;
};





#endif //CASCADE_SHADOW_MAP_H