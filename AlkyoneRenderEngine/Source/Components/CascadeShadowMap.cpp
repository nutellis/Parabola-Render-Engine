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
#include <Components/RenderComponents/StaticMeshComponent.h>
#include <Components/StaticMesh.h>
#include <Components/Colliders/IntersectionTests.h>
#include <Components/Colliders/BoundingHelper.h>


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

		Cascade->Frustrum->FarPlane = Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar * 1.002f;

		uint64 Resolution =  SMath::HighestPowerOf2(4096 / (static_cast<uint64>(i) + 1)); //
		
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

PCascadeShadowMap::~PCascadeShadowMap()
{
	for (PCascade * Cascade : Cascades) {
		delete Cascade->CascadeFBO;
	}
	Cascades.Destroy();
	glDeleteBuffers(1, &CascadesMVPBuffer);
}

void PCascadeShadowMap::Init()
{
	glGenBuffers(1, &ShadowMapSSBO);
	glGenBuffers(1, &CascadesMVPBuffer);

	for (uint32 i = 0; i < NumCascades; i++)
	{
		Cascades[i]->CascadeFBO->Init("Cascade_" + i, Cascades[i]->Resolution, Cascades[i]->Resolution, true);
		
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

		Cascades[i]->Frustrum->FarPlane = Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar * 1.002f;

		Cascades[i]->Resolution = static_cast<uint32>(SMath::HighestPowerOf2(4096 / (static_cast<uint64>(i) + 1)));
		Cascades[i]->Frustrum->Ratio = Ratio;
		Cascades[i]->Frustrum->FieldOfView = FieldOfView;
	}
}
void PCascadeShadowMap::CalculateLightProjection(
	uint32 Index,
	PCameraComponent* Camera,
	PDirectionalLightComponent* Light,
	bool SquareBox
) {
	float L, R, B, T, N, F;

	// Calculate the Corners in world space!
	Cascades[Index]->Frustrum->CalculateFrustrumCorners(Camera);

	Matrix4f LightViewMatrix = LookAt(Vector3f::ZERO, -Light->LightDirection, Vector3f(0.0f, 1.0f, 0.0f));

	// World Space -> Light Space
	PAxisAlignedBoundingBox* LightSpaceAABB = BoundingHelper::Transform(Cascades[Index]->Frustrum->BoundingBox, LightViewMatrix);
	
	L = LightSpaceAABB->Min.X;
	R = LightSpaceAABB->Max.X;
	B = LightSpaceAABB->Min.Y;
	T = LightSpaceAABB->Max.Y;
	N = LightSpaceAABB->Min.Z;
	F = LightSpaceAABB->Max.Z;

	TArray<PAxisAlignedBoundingBox*> ShadowCasters = gSceneManager.GetActiveScene()->GetShadowCasters(Cascades[Index]->Frustrum->BoundingBox, Vector3f(Cascades[Index]->CascadeDebugColour));
	if (ShadowCasters.IsNotEmpty()) {
		for (int i = 0; i < ShadowCasters.Size(); ++i) {
			ShadowCasters[i] = BoundingHelper::Transform(ShadowCasters[i], LightViewMatrix);
		}
		// get the union of all shadow casters in light space
		PAxisAlignedBoundingBox* LightSpaceCasters = BoundingHelper::UnionAABB(ShadowCasters);

		N = SMath::Min(LightSpaceAABB->Min.Z, LightSpaceCasters->Min.Z);
		F = SMath::Max(LightSpaceAABB->Max.Z, LightSpaceCasters->Max.Z);
	}

	float Extent = SMath::Max(R - L, T - B);

	// Make the box square
	if (SquareBox) {
		float W = R - L, H = T - B;
		float Diff = Extent - H;
		if (Diff > 0) {
			T += Diff / 2.0f;
			B -= Diff / 2.0f;
		}
		Diff = Extent - W;
		if (Diff > 0) {
			R += Diff / 2.0f;
			L -= Diff / 2.0f;
		}
	}
	// Texel Snapping (https://www.realtimeshadows.com/sites/default/files/sig2013-course-hardshadows_0.pdf)
	float TexelSize = Extent / Cascades[Index]->Resolution;

	L = std::round(L / TexelSize) * TexelSize;
	R = std::round(R / TexelSize) * TexelSize;
	B = std::round(B / TexelSize) * TexelSize;
	T = std::round(T / TexelSize) * TexelSize;


	Matrix4f LightProjectionMatrix = Ortho(L, R, B, T, -1000, 1000);

	Matrix4f lightSpaceMatrix = LightProjectionMatrix * LightViewMatrix;

	Cascades[Index]->LightViewMatrix = LightViewMatrix;

	Cascades[Index]->LightProjectionMatrix = LightProjectionMatrix;

	Cascades[Index]->CascadeCPVMatrix = lightSpaceMatrix;

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

	Matrix4f TextureSpaceMatrix = Scale(Vector3f(0.5f, 0.5f, 0.5f), Translate(Vector3f(0.5f, 0.5f, 0.5f), Matrix4f::IDENTITY));
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