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

#define MAX_CASCADES 4


struct PFrustrum {
public:
	Vector3f Corners[8];
	Vector3f Center;
	Vector3f Extents;
	Vector3f Direction;
	float NearPlane;
	float FarPlane;
	float Ratio;
	float FieldOfView;
/*	Matrix4f ProjectionMatrix;
	Matrix4f ViewMatrix*/;

	void CalculateFrustrumCorners(PCameraComponent* Camera);
};

struct PCascade {

public:
	float Min;
	float Max;

	Vector3f FrustrumCenter;

	uint32 Resolution = 4096;
	
	FBORenderTarget* CascadeFBO;

	Matrix4f CascadeCPVMatrix;

	Matrix4f LightViewMatrix;
	Matrix4f LightProjectionMatrix;
	Matrix4f CropMatrix;

	PFrustrum Frustrum;

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
	void CalculateLightProjection(uint32 Index, PCameraComponent* Camera, PDirectionalLightComponent* Light);
	void UpdateCascadeBuffer(Matrix4f CameraViewMatrix);
	void PrepareForDraw(uint32 Index);
	void Draw(uint32 Index);
	void BindSSBO(uint32 Slot);
	void UnbindBuffers();
	void PrepareForDraw(Shader* ActiveShader, Matrix4f CameraViewMatrix, Matrix4f CameraProjectionMatrix);
	void SetViewport(uint32 Width, uint32 Height);

	PCascade GetCascade(uint32 Index);

private:
	uint32 NumCascades;
	TArray<PCascade> Cascades;

	float Lambda = 0.75f;

	uint32 ShadowMapSSBO;
	uint32 CascadesMVPBuffer;
	uint64 * ShadowMapTextureHandles;
};





#endif //CASCADE_SHADOW_MAP_H