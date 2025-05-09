#include "Components/CascadeShadowMap.h"
#include <Utilities/Containers/Array.h>
#include <Components/FBORenderTarget.h>
#include <Components/Texture.h>
#include <Components/Components.h>
#include <Utilities/CameraUtiltities.h>
#include <Utilities/PrimitiveShapes.h>
#include <ParabolaMath.h>
#include <Managers/ShaderManager.h>
#include <Components/LightComponents/DirectionalLightComponent.h>
#include <Components/Camera.h>

PCascadeShadowMap::PCascadeShadowMap()
{
}

PCascadeShadowMap::PCascadeShadowMap(
	uint32 InNumCascades, 
	float InFieldOfView,
	float InAspectRatio,
	float ZNear, 
	float ZFar) : NumCascades(InNumCascades)
{

	Cascades = TArray<PCascade *>(InNumCascades);

	for (uint32 i = 0; i < NumCascades; i++)
	{
		PCascade * Cascade = new PCascade();

		float LogStepNear = ZNear * pow((ZFar / ZNear), static_cast<float>(i) / static_cast<float>(NumCascades));
		float UniStepNear = ZNear + (ZFar - ZNear) * static_cast<float>(i) / static_cast<float>(NumCascades);

		Cascade->Frustrum->NearPlane = Lambda * LogStepNear + (1.0 - Lambda) * UniStepNear;


		float LogStepFar = ZNear * pow((ZFar / ZNear), static_cast<float>(i + 1) / static_cast<float>(NumCascades));
		float UniStepFar = ZNear + (ZFar - ZNear) * static_cast<float>(i + 1) / static_cast<float>(NumCascades);

		Cascade->Frustrum->FarPlane = i < NumCascades-1 ? (Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar) * 1.005f : Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar;

		uint64 Resolution =  SMath::HighestPowerOf2(4096 / (static_cast<uint64>(i) + 1)); //
		
		Cascade->Resolution = static_cast<uint32>(Resolution);

		Cascade->Frustrum->Ratio = InAspectRatio;
		Cascade->Frustrum->FieldOfView = InFieldOfView;
		Cascade->Frustrum->Center = Vector3f::ZERO;

		Cascades.PushBack(Cascade);
	}
}

PCascadeShadowMap::~PCascadeShadowMap()
{
}

void PCascadeShadowMap::Init()
{
	glGenBuffers(1, &ShadowMapSSBO);
	glGenBuffers(1, &CascadesMVPBuffer);

	ShadowMapTextureHandles = new uint64[NumCascades];
	for (uint32 i = 0; i < NumCascades; i++)
	{
		Cascades[i]->CascadeFBO->Init("Cascade_" + i, Cascades[i]->Resolution, Cascades[i]->Resolution, false);
		
		ShadowMapTextureHandles[i] = Cascades[i]->CascadeFBO->DepthStencilAttachment->TextureHandle;
		
		glMakeTextureHandleResidentARB(ShadowMapTextureHandles[i]);
	}
	// Init shadow map texture buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ShadowMapSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint64) * NumCascades, &ShadowMapTextureHandles, GL_DYNAMIC_DRAW);
	
	// init cascade MVP buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, CascadesMVPBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrix4f) * NumCascades, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void PCascadeShadowMap::UpdateCascadeExtends(float ZNear, float ZFar, float Ratio, float FieldOfView) {

	for (uint32 i = 0; i < NumCascades; i++)
	{
		float LogStepNear = ZNear * pow((ZFar / ZNear), static_cast<float>(i) / static_cast<float>(NumCascades));
		float UniStepNear = ZNear + (ZFar - ZNear) * static_cast<float>(i) / static_cast<float>(NumCascades);

		Cascades[i]->Frustrum->NearPlane = Lambda * LogStepNear + (1.0 - Lambda) * UniStepNear;


		float LogStepFar = ZNear * pow((ZFar / ZNear), static_cast<float>(i + 1) / static_cast<float>(NumCascades));
		float UniStepFar = ZNear + (ZFar - ZNear) * static_cast<float>(i + 1) / static_cast<float>(NumCascades);

		Cascades[i]->Frustrum->FarPlane = i < NumCascades - 1 ? (Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar) * 1.005f : Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar;

		uint64 Resolution = SMath::HighestPowerOf2(4096 / (static_cast<uint64>(i) + 1)); //

		Cascades[i]->Resolution = static_cast<uint32>(Resolution);

		Cascades[i]->Frustrum->Ratio = Ratio;
		Cascades[i]->Frustrum->FieldOfView = FieldOfView;
	}
}

void PCascadeShadowMap::CalculateLightProjection(uint32 Index, PCameraComponent * Camera, PDirectionalLightComponent * Light) {

	// Calculate the Corners 
	Cascades[Index]->Frustrum->CalculateFrustrumCorners(Camera);
	
	Matrix4f LightViewMatrix = LookAt(Cascades[Index]->Frustrum->Center, Cascades[Index]->Frustrum->Center - Light->LightDirection, Vector3f(-1.0f, 0.0f, 0.0f));

	//Matrix4f LightViewMatrix = LookAt(Vector3f::ZERO, -Light->LightDirection, Vector3f(0.0f, -1.0f, 0.0f));

	// Calculate the Crop Matrix
	Vector4f TransformedCorner = LightViewMatrix * Vector4f(Cascades[Index]->Frustrum->Corners[0], 1.0f);
	Vector3f Min = Vector3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), TransformedCorner.Z);
	Vector3f Max = Vector3f(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), TransformedCorner.Z);

	for (int i = 1; i < 8; i++)
	{
		TransformedCorner = LightViewMatrix * Vector4f(Cascades[Index]->Frustrum->Corners[i], 1.0f);
		Min.Z = SMath::Min(Min.Z, TransformedCorner.Z);
		Max.Z = SMath::Max(Max.Z, TransformedCorner.Z);
	}

	Matrix4f LightProjectionMatrix = Ortho(-1.0, 1.0, -1.0, 1.0, -Max.Z, -Min.Z);

	Matrix4f LightViewProjectionMatrix = LightProjectionMatrix * LightViewMatrix;


	for (int i = 1; i < 8; i++)
	{
		TransformedCorner = LightViewProjectionMatrix * Vector4f(Cascades[Index]->Frustrum->Corners[i], 1.0f);
		
		TransformedCorner.X /= TransformedCorner.W;
		TransformedCorner.Y /= TransformedCorner.W;
		
		Min.X = SMath::Min(Min.X, TransformedCorner.X);
		Max.X = SMath::Max(Max.X, TransformedCorner.X);
		Min.Y = SMath::Min(Min.Y, TransformedCorner.Y);
		Max.Y = SMath::Max(Max.Y, TransformedCorner.Y);
	}

	float ScaleX = 2.0f / (Max.X - Min.X);
	float ScaleY = 2.0f / (Max.Y - Min.Y);

	float OffsetX = -0.5f * (Max.X + Min.X) * ScaleX;
	float OffsetY = -0.5f * (Max.Y + Min.Y) * ScaleY;

	// Texel Snapping (https://www.realtimeshadows.com/sites/default/files/sig2013-course-hardshadows_0.pdf)
	float TexelsPerUnit = (Cascades[Index]->Frustrum->DiagonalLength * 2.0f); // Cascades[Index]->Resolution / 2.0f; // 

	OffsetX = std::ceil(OffsetX * TexelsPerUnit) / TexelsPerUnit; //  + 0.5f
	OffsetY = std::ceil(OffsetY * TexelsPerUnit) / TexelsPerUnit; // + 0.5f


	Matrix4f CropMatrix = Translate(Vector3f(OffsetX, OffsetY, 0.0f), Matrix4f::IDENTITY);
	
	CropMatrix = Scale(Vector3f(ScaleX, ScaleY, 1.0f), CropMatrix);

	Cascades[Index]->LightViewMatrix = LightViewMatrix;

	Cascades[Index]->CropMatrix = CropMatrix;

	Cascades[Index]->LightProjectionMatrix = CropMatrix * LightProjectionMatrix;

	Cascades[Index]->CascadeCPVMatrix = CropMatrix * LightProjectionMatrix * LightViewMatrix;
	

	// shad_modelview = lightview
	// shad_mvp = lightprojection * lightview
	// shad_proj = crop * projection
	// shad_cpm = crop * projection * view
}

void PCascadeShadowMap::UpdateCascadeBuffer(Matrix4f CameraViewMatrix) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, CascadesMVPBuffer);

	Matrix4f* CascadeMatrices = (Matrix4f*)glMapBufferRange(
		GL_SHADER_STORAGE_BUFFER, 0,
		sizeof(Matrix4f) * NumCascades,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	Matrix4f TextureSpaceMatrix = Scale(0.5, Translate(Vector3f(0.5), Matrix4f::IDENTITY));

	//In texture space!
	for (uint32 i = 0; i < NumCascades; i++)
	{
		CascadeMatrices[i] = TextureSpaceMatrix * Cascades[i]->CascadeCPVMatrix * Inverse(CameraViewMatrix);
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void PCascadeShadowMap::BindSSBO(uint32 Slot)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, Slot, ShadowMapSSBO);

}

void PCascadeShadowMap::UnbindBuffers()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void PCascadeShadowMap::PrepareForDraw(
	Shader* ActiveShader, 
	Matrix4f CameraViewMatrix, 
	Matrix4f CameraProjectionMatrix, 
	bool UsePCSS
)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, CascadesMVPBuffer);

	Matrix4f* CascadeMatrices = (Matrix4f*)glMapBufferRange(
		GL_SHADER_STORAGE_BUFFER, 0,
		sizeof(Matrix4f) * NumCascades,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	Matrix4f TextureSpaceMatrix = Scale(0.5, Translate(Vector3f(0.5), Matrix4f::IDENTITY));
	Vector4f FarPlanes = Vector4f::ONE;
	Vector4f NearPlanes = Vector4f::ONE;
	Vector4f LightFrustrumWidth = Vector4f::ONE;
	Vector4f TexelSizeVector = Vector4f::ZERO;
	
	//In texture space!
	for (uint32 i = 0; i < NumCascades; i++)
	{
		CascadeMatrices[i] = TextureSpaceMatrix * Cascades[i]->CascadeCPVMatrix * Inverse(CameraViewMatrix);
		
		//Bind Textures
		glActiveTexture(GL_TEXTURE10 + i);

		glBindTexture(GL_TEXTURE_2D, Cascades[i]->CascadeFBO->DepthStencilAttachment->TextureID);
	
		std::string index = "shadowMap[" + std::to_string(i) + "]";
		ActiveShader->SetInt(index.c_str(), 10 + i);

		std::string indexRegular = "shadowMapRegular[" + std::to_string(i) + "]";
		ActiveShader->SetInt(indexRegular.c_str(), 10 + i);
		
		LightFrustrumWidth[i] = 2.0 / Cascades[i]->LightProjectionMatrix[0][0];

		TexelSizeVector[i] = Cascades[i]->Resolution;

		// in texture space
		FarPlanes[i] = 0.5f * (-Cascades[i]->Frustrum->FarPlane * CameraProjectionMatrix[2][2] +
			CameraProjectionMatrix[3][2]) / Cascades[i]->Frustrum->FarPlane + 0.5f;

		//in world space
		NearPlanes[i] = Cascades[i]->Frustrum->NearPlane;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	ActiveShader->SetInt("numOfCascades", NumCascades);
	ActiveShader->SetVec4("farPlanes", FarPlanes);
	ActiveShader->SetVec4("nearPlanes", NearPlanes);
	ActiveShader->SetVec4("lightFrustrumWidth", LightFrustrumWidth);
	ActiveShader->SetVec4("texelSize", TexelSizeVector);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, CascadesMVPBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ShadowMapSSBO);
}

PCascade * PCascadeShadowMap::GetCascade(uint32 Index)
{
	return Cascades[Index];
}

PCascade::PCascade()
{
	Frustrum = new PFrustrum();
	CascadeFBO = new FBORenderTarget();
}
PCascade::~PCascade() {
	delete Frustrum;
	delete CascadeFBO;
}