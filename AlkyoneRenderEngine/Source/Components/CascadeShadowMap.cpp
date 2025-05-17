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
#include <Components/CameraComponents/Camera.h>

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

		Cascade->Frustrum->FarPlane = Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar;

		uint64 Resolution =  SMath::HighestPowerOf2(4096 / (static_cast<uint64>(i) + 1)); //
		
		Cascade->Resolution = static_cast<uint32>(Resolution);

		Cascade->Frustrum->Ratio = InAspectRatio;
		Cascade->Frustrum->FieldOfView = InFieldOfView;
		Cascade->Frustrum->Center = Vector3f::ZERO;

		Cascade->CascadeDebugColour = Vector4f(
			0.85f + 0.15f * float(i >= 2),
			0.85f + 0.15f * float(i == 1 || i == 3),
			0.85f + 0.15f * float(i == 0 || i == 3),
			0.3f
		);


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

	for (uint32 i = 0; i < NumCascades; i++)
	{
		Cascades[i]->CascadeFBO->Init("Cascade_" + i, Cascades[i]->Resolution, Cascades[i]->Resolution, false);
		
	}	
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

		Cascades[i]->Resolution = static_cast<uint32>(SMath::HighestPowerOf2(4096 / (static_cast<uint64>(i) + 1)));
		Cascades[i]->Frustrum->Ratio = Ratio;
		Cascades[i]->Frustrum->FieldOfView = FieldOfView;
	}
}

void PCascadeShadowMap::CalculateLightProjection(uint32 Index, PCameraComponent * Camera, PDirectionalLightComponent * Light) {

	Vector3f Min = Vector3f(std::numeric_limits<float>::infinity());
	Vector3f Max = Vector3f(-std::numeric_limits<float>::infinity());


	// Calculate the Corners in world space!
	Cascades[Index]->Frustrum->CalculateFrustrumCorners(Camera);

	Vector3f LightPosition = Cascades[Index]->Frustrum->Center + Vector3f(Light->LightDirection) * (Cascades[Index]->Frustrum->FarPlane - Cascades[Index]->Frustrum->NearPlane);

	Matrix4f LightViewMatrix = LookAt(LightPosition, Cascades[Index]->Frustrum->Center, Vector3f(0.0f, 1.0f, 0.0f));
	//Matrix4f LightViewMatrix = LookAt(Cascades[Index]->Frustrum->Center, Cascades[Index]->Frustrum->Center - Vector3f(Light->LightDirection), Vector3f(0.0f, 1.0f, 0.0f));


	// World Space -> Light Space
	Vector4f TransformedCorner = LightViewMatrix * Vector4f(Cascades[Index]->Frustrum->Corners[0], 1.0f);
	Min = TransformedCorner;
	Max = TransformedCorner;

	for (int i = 1; i < 8; i++)
	{
		TransformedCorner = LightViewMatrix * Vector4f(Cascades[Index]->Frustrum->Corners[i], 1.0f);

		Min.Z = SMath::Min(Min.Z, TransformedCorner.Z);
		Max.Z = SMath::Max(Max.Z, TransformedCorner.Z);
	}

	float N = Min.Z;
	float F = Max.Z;
	Matrix4f LightProjectionMatrix = Ortho(-1, 1, -1, 1, N, F);

	//TODO: Do a tighter bounding by being scene aware 



	for (int i = 1; i < 8; i++)
	{
		TransformedCorner = LightProjectionMatrix * LightViewMatrix * Vector4f(Cascades[Index]->Frustrum->Corners[i], 1.0f);
		Min.X = SMath::Min(Min.X, TransformedCorner.X);
		Max.X = SMath::Max(Max.X, TransformedCorner.X);
		Min.Y = SMath::Min(Min.Y, TransformedCorner.Y);
		Max.Y = SMath::Max(Max.Y, TransformedCorner.Y);
	}

	float TexelSize = SMath::Max(Max.X - Min.X, Max.Y - Min.Y) / Cascades[Index]->Resolution;

	float L = std::round(Min.X / TexelSize) * TexelSize;
	float R = std::round(Max.X / TexelSize) * TexelSize;
	float B = std::round(Min.Y / TexelSize) * TexelSize;
	float T = std::round(Max.Y / TexelSize) * TexelSize;


	// Texel Snapping (https://www.realtimeshadows.com/sites/default/files/sig2013-course-hardshadows_0.pdf)

	float ScaleX = 2.0f / (R - L);
	float ScaleY = 2.0f / (T - B);

	//Calculate Crop Matrix  (https://www.realtimeshadows.com/sites/default/files/sig2013-course-hardshadows_0.pdf)

	float OffsetX = - 0.5f * (R + L) * ScaleX;
	float OffsetY = -0.5f * (T + B) * ScaleY;


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

	Matrix4f TextureSpaceMatrix = Scale(Vector3f(0.5f, 0.5f, 0.5f), Translate(Vector3f(0.5f, 0.5f, 0.5), Matrix4f::IDENTITY));
	Vector4f FarPlanes = Vector4f::ONE;
	Vector4f NearPlanes = Vector4f::ONE;
	Vector4f LightFrustrumWidth = Vector4f::ONE;
	Vector4f TexelSizeVector = Vector4f::ZERO;
	
	//In texture space!
	for (uint32 i = 0; i < NumCascades; i++)
	{
		// from light projection space (Crop * Projection * View) to Texture Space
		CascadeMatrices[i] = TextureSpaceMatrix * Cascades[i]->CascadeCPVMatrix;
		
		//Bind Textures
		glActiveTexture(GL_TEXTURE10 + i);

		glBindTexture(GL_TEXTURE_2D, Cascades[i]->CascadeFBO->DepthStencilAttachment->TextureID);
	
		std::string index = "shadowMap[" + std::to_string(i) + "]";
		ActiveShader->SetInt(index.c_str(), 10 + i);
		
		LightFrustrumWidth[i] = 2.0 / Cascades[i]->LightProjectionMatrix[0][0];

		TexelSizeVector[i] = Cascades[i]->Resolution;

		// in world space
		FarPlanes[i] = Cascades[i]->Frustrum->FarPlane;
		
		//0.5f * (-Cascades[i]->Frustrum->FarPlane * CameraProjectionMatrix[2][2] +
			// CameraProjectionMatrix[3][2]) / Cascades[i]->Frustrum->FarPlane + 0.5f;

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