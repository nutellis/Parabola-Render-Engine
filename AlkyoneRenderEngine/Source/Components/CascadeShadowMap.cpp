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
#include <Managers/SceneManager.h>
#include <Components/Scene.h>
#include <Components/RenderActor.h>
#include <Components/RenderComponents/StaticMeshGroup.h>
#include <Components/StaticMesh.h>
#include <Components/Colliders/IntersectionTests.h>
#include <Components/Colliders/BoundingHelper.h>


RCascadeShadowMap::RCascadeShadowMap()
{
}

RCascadeShadowMap::RCascadeShadowMap(
	uint32 InNumCascades, 
	float InFieldOfView,
	float InAspectRatio,
	float ZNear, 
	float ZFar) : NumCascades(InNumCascades)
{

	Cascades = TArray<RCascade *>(InNumCascades);

	for (uint32 i = 0; i < NumCascades; i++)
	{
		uint64 Resolution = SMath::HighestPowerOf2(4096 / (static_cast<uint64>(i) + 1)); //

		RCascade * Cascade = new RCascade("Cascade_" + i, Resolution, Resolution);

		float LogStepNear = ZNear * pow((ZFar / ZNear), static_cast<float>(i) / static_cast<float>(NumCascades));
		float UniStepNear = ZNear + (ZFar - ZNear) * static_cast<float>(i) / static_cast<float>(NumCascades);

		Cascade->Frustrum->NearPlane = Lambda * LogStepNear + (1.0 - Lambda) * UniStepNear * TransitionOverlap;


		float LogStepFar = ZNear * pow((ZFar / ZNear), static_cast<float>(i + 1) / static_cast<float>(NumCascades));
		float UniStepFar = ZNear + (ZFar - ZNear) * static_cast<float>(i + 1) / static_cast<float>(NumCascades);

		Cascade->Frustrum->FarPlane = Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar;

		Cascade->Resolution = static_cast<uint32>(Resolution);

		Cascade->Frustrum->Ratio = InAspectRatio;
		Cascade->Frustrum->FieldOfView = InFieldOfView;
		Cascade->Frustrum->FrustrumBox = new PBoundingBox();
		Cascade->Frustrum->BoundingBox = new AABB();

		Cascade->CascadeDebugColour = Vector4f(
			0.85f + 0.15f * float(i >= 2),
			0.85f + 0.15f * float(i == 1 || i == 3),
			0.85f + 0.15f * float(i == 0 || i == 3),
			0.3f
		);


		Cascades.PushBack(Cascade);
	}
}

RCascadeShadowMap::~RCascadeShadowMap()
{
	for (RCascade * Cascade : Cascades) {
		delete Cascade->CascadeFBO;
	}
	Cascades.Clear();
	glDeleteBuffers(1, &CascadesMVPBuffer);
}

void RCascadeShadowMap::Init()
{
	glGenBuffers(1, &ShadowMapSSBO);
	glGenBuffers(1, &CascadesMVPBuffer);

	RTextureOptions ColourTextureOptions = RTextureOptions(
		GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT,
		GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER,
		GL_NONE, GL_LESS, false);

	TArray<RTextureOptions> ColourAttachmentOptions = TArray<RTextureOptions>(1, ColourTextureOptions);


	for (uint32 i = 0; i < NumCascades; i++)
	{
		//Cascades[i]->CascadeFBO->Init(TArray<RTextureOptions>(), RTextureOptions::InitForShadows());

		Cascades[i]->CascadeFBO->Init(ColourAttachmentOptions, RTextureOptions::InitForShadows());
	}	
	// init cascade MVP buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, CascadesMVPBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrix4f) * NumCascades, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void RCascadeShadowMap::UpdateCascadeExtends(float ZNear, float ZFar, float Ratio, float FieldOfView) {

	for (uint32 i = 0; i < NumCascades; i++)
	{
		float LogStepNear = ZNear * pow((ZFar / ZNear), static_cast<float>(i) / static_cast<float>(NumCascades));
		float UniStepNear = ZNear + (ZFar - ZNear) * static_cast<float>(i) / static_cast<float>(NumCascades);

		Cascades[i]->Frustrum->NearPlane = Lambda * LogStepNear + (1.0 - Lambda) * UniStepNear * TransitionOverlap;


		float LogStepFar = ZNear * pow((ZFar / ZNear), static_cast<float>(i + 1) / static_cast<float>(NumCascades));
		float UniStepFar = ZNear + (ZFar - ZNear) * static_cast<float>(i + 1) / static_cast<float>(NumCascades);

		Cascades[i]->Frustrum->FarPlane = Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar;

		Cascades[i]->Resolution = static_cast<uint32>(SMath::HighestPowerOf2(4096 / (static_cast<uint64>(i) + 1)));
		Cascades[i]->Frustrum->Ratio = Ratio;
		Cascades[i]->Frustrum->FieldOfView = FieldOfView;
	}
}
void RCascadeShadowMap::CalculateLightProjection(
	uint32 Index,
	PCameraComponent* Camera,
	PDirectionalLightComponent* Light,
	bool SquareBox
) {
	float L, R, B, T, N, F;

	// Calculate the Corners in world space!
	Cascades[Index]->Frustrum->CalculateFrustrumCorners(Camera, 5.0);
	Cascades[Index]->Frustrum->CalculateFrustrumPlanes(Camera);

	// World Space Frustrum -> Light Space
	PBoundingBox LightSpaceTransform = BoundingHelper::TransformCreate(*Cascades[Index]->Frustrum->FrustrumBox, Light->LightViewMatrix);
	
	PAxisAlignedBoundingBox LightSpaceAABB = BoundingHelper::CalculateAABBFromCorners(LightSpaceTransform.Corners);

	//TArray<PAxisAlignedBoundingBox> ShadowCasters = gSceneManager.GetActiveScene()->GetShadowCasters(Cascades[Index]->Frustrum->BoundingBox, Vector3f(Cascades[Index]->CascadeDebugColour));
	//if (ShadowCasters.IsNotEmpty()) {
	//	for (int i = 0; i < ShadowCasters.Size(); ++i) {
	//		BoundingHelper::Transform(ShadowCasters[i], Light->LightViewMatrix);
	//	}
	//	// get the union of all shadow casters in light space
	//	PAxisAlignedBoundingBox LightSpaceCasters = BoundingHelper::UnionAABB(ShadowCasters);

	//	LightSpaceAABB.Min.X = SMath::Min(LightSpaceAABB.Min.X, LightSpaceCasters.Min.X);
	//	LightSpaceAABB.Max.X = SMath::Max(LightSpaceAABB.Max.X, LightSpaceCasters.Max.X);
	//	LightSpaceAABB.Min.Y = SMath::Min(LightSpaceAABB.Min.Y, LightSpaceCasters.Min.Y);
	//	LightSpaceAABB.Max.Y = SMath::Max(LightSpaceAABB.Max.Y, LightSpaceCasters.Max.Y);
	//	LightSpaceAABB.Min.Z = SMath::Min(LightSpaceAABB.Min.Z, LightSpaceCasters.Min.Z);
	//	LightSpaceAABB.Max.Z = SMath::Max(LightSpaceAABB.Max.Z, LightSpaceCasters.Max.Z);
	//}

	if (SquareBox) {
		float Extent = SMath::Max(LightSpaceAABB.Extents.X, LightSpaceAABB.Extents.Y);

		float DiffW = Extent - LightSpaceAABB.Extents.X;
		if (DiffW > 0) {
			LightSpaceAABB.Min.X -= DiffW * 0.5f;
			LightSpaceAABB.Max.X += DiffW * 0.5f;
		}
		float DiffH = Extent - LightSpaceAABB.Extents.Y;
		if (DiffH > 0) {
			LightSpaceAABB.Min.Y -= DiffH * 0.5f;
			LightSpaceAABB.Max.Y += DiffH * 0.5f;
		}
		LightSpaceAABB.Extents.X = Extent;
		LightSpaceAABB.Extents.Y = Extent;
	}

	// Texel Snapping 
	float WorldUnitsPerTexel = LightSpaceAABB.Extents.X / static_cast<float>(Cascades[Index]->Resolution);
	LightSpaceAABB.Min.X = std::round(LightSpaceAABB.Min.X / WorldUnitsPerTexel) * WorldUnitsPerTexel;
	LightSpaceAABB.Max.X = std::round(LightSpaceAABB.Max.X / WorldUnitsPerTexel) * WorldUnitsPerTexel;
	LightSpaceAABB.Min.Y = std::round(LightSpaceAABB.Min.Y / WorldUnitsPerTexel) * WorldUnitsPerTexel;
	LightSpaceAABB.Max.Y = std::round(LightSpaceAABB.Max.Y / WorldUnitsPerTexel) * WorldUnitsPerTexel;

	L = LightSpaceAABB.Min.X;
	R = LightSpaceAABB.Max.X;
	B = LightSpaceAABB.Min.Y;
	T = LightSpaceAABB.Max.Y;
	N = 0.1;
	F = Light->LightDistance * 2.0f;


	Cascades[Index]->LightProjectionMatrix =  Ortho(L, R, B, T, N, F);

	Cascades[Index]->Near = N;

	Cascades[Index]->Far = F;

	Cascades[Index]->LightViewMatrix = Light->LightViewMatrix;



	Cascades[Index]->CascadePVMatrix = Cascades[Index]->LightProjectionMatrix * Light->LightViewMatrix;

}

// NOT USED
void RCascadeShadowMap::UpdateCascadeBuffer(Matrix4f CameraViewMatrix) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, CascadesMVPBuffer);

	Matrix4f* CascadeMatrices = (Matrix4f*)glMapBufferRange(
		GL_SHADER_STORAGE_BUFFER, 0,
		sizeof(Matrix4f) * NumCascades,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	Matrix4f TextureSpaceMatrix = Scale(0.5, Translate(Vector3f(0.5), Matrix4f::IDENTITY));

	//In texture space!
	for (uint32 i = 0; i < NumCascades; i++)
	{
		CascadeMatrices[i] = TextureSpaceMatrix * Cascades[i]->CascadePVMatrix * Inverse(CameraViewMatrix);
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void RCascadeShadowMap::BindSSBO(uint32 Slot)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, Slot, ShadowMapSSBO);

}

void RCascadeShadowMap::CleanUp()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (int i = 0; i < NumCascades; ++i) {
		glActiveTexture(GL_TEXTURE10 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);
}

void RCascadeShadowMap::PrepareForDraw(
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

	Matrix4f TextureSpaceMatrix = Scale(Vector3f(0.5f, 0.5f, 0.5f), Translate(Vector3f(0.5f, 0.5f, 0.5f), Matrix4f::IDENTITY));
	Vector4f FarPlanes = Vector4f::ONE;
	Vector4f LightNear = Vector4f::ONE;
	Vector4f LightFar = Vector4f::ONE;
	Vector4f LightFrustrumWidth = Vector4f::ONE;
	Vector4f Resolutions = Vector4f::ZERO;
	
	//In texture space!
	for (uint32 i = 0; i < NumCascades; i++)
	{
		// from light projection space (Crop * Projection * View) to Texture Space
		CascadeMatrices[i] = Cascades[i]->CascadePVMatrix;
		
		//Bind Textures
		glActiveTexture(GL_TEXTURE10 + i);

		glBindTexture(GL_TEXTURE_2D, Cascades[i]->CascadeFBO->ColourAttachments[0]->TextureID);
	
		std::string index = "shadowMap[" + std::to_string(i) + "]";
		ActiveShader->SetInt(index.c_str(), 10 + i);
		
		LightFrustrumWidth[i] = 2.0 / Cascades[i]->LightProjectionMatrix[0][0];

		Resolutions[i] = Cascades[i]->Resolution;

		// in world space
		FarPlanes[i] = Cascades[i]->Frustrum->FarPlane;
		//NearPlanes[i] = Cascades[i]->Frustrum->NearPlane;

		//in world space

		//float m22 = Cascades[i]->LightProjectionMatrix[2][2]; // m[2][2]
		//float m23 = Cascades[i]->LightProjectionMatrix[3][2]; // m[2][3]

		//float d = -2.0f / m22;
		//float nearPlane = (-d * (1.0f + m23)) * 0.5f;
		LightNear[i] = Cascades[i]->Near;
		LightFar[i] = Cascades[i]->Far;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	ActiveShader->SetInt("numOfCascades", NumCascades);
	ActiveShader->SetVec4("farPlanes", FarPlanes);
	ActiveShader->SetVec4("far", LightFar);
	ActiveShader->SetVec4("near", LightNear); 
	ActiveShader->SetVec4("lightFrustrumWidth", LightFrustrumWidth);
	ActiveShader->SetVec4("resolution", Resolutions);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, CascadesMVPBuffer);
}

RCascade * RCascadeShadowMap::GetCascade(uint32 Index)
{
	return Cascades[Index];
}

RCascade::RCascade()
{
	Frustrum = new PFrustrum();
	CascadeFBO = new FBORenderTarget();
}

RCascade::RCascade(std::string Name, uint32 Width, uint32 Height)
{
	Frustrum = new PFrustrum();
	CascadeFBO = new FBORenderTarget(Name, Width, Height);
}
RCascade::~RCascade() {
	delete Frustrum;
	delete CascadeFBO;
}