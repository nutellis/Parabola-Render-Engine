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

	Cascades = TArray<PCascade>(InNumCascades);

	for (uint32 i = 0; i < NumCascades; i++)
	{
		PCascade Cascade = PCascade();

		float LogStepNear = ZNear * pow((ZFar / ZNear), static_cast<float>(i) / static_cast<float>(NumCascades));
		float UniStepNear = ZNear + (ZFar - ZNear) * static_cast<float>(i) / static_cast<float>(NumCascades);

		Cascade.Frustrum.NearPlane = Lambda * LogStepNear + (1.0 - Lambda) * UniStepNear;


		float LogStepFar = ZNear * pow((ZFar / ZNear), static_cast<float>(i + 1) / static_cast<float>(NumCascades));
		float UniStepFar = ZNear + (ZFar - ZNear) * static_cast<float>(i + 1) / static_cast<float>(NumCascades);

		Cascade.Frustrum.FarPlane = i < NumCascades-1 ? (Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar) * 1.005f : Lambda * LogStepFar + (1.0 - Lambda) * UniStepFar;


		uint64 Resolution = 1024;// SMath::HighestPowerOf2(4096 / (static_cast<uint64>(i) + 1));
		
		Cascade.Resolution = static_cast<uint32>(Resolution);
		
		Cascade.CascadeFBO = new FBORenderTarget("Cascade_"+ i, Resolution, Resolution);

		Cascade.Frustrum.Ratio = InAspectRatio;
		Cascade.Frustrum.FieldOfView = InFieldOfView;
		Cascade.Frustrum.Center = Vector3f::ZERO;

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
		Cascades[i].CascadeFBO->Init(true);
		
		ShadowMapTextureHandles[i] = Cascades[i].CascadeFBO->DepthStencilAttachment->TextureHandle;
		
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

void PCascadeShadowMap::CalculateLightProjection(uint32 Index, PCameraComponent * Camera, PDirectionalLightComponent * Light) {

	// Calculate the Corners 
	Cascades[Index].Frustrum.CalculateFrustrumCorners(Camera);
	
	Matrix4f LightViewMatrix = LookAt(Vector3f::ZERO, -Light->LightDirection, Vector3f(-1.0f, 0.0f, 0.0f));

	Cascades[Index].LightViewMatrix = LightViewMatrix;

	// Calculate the Crop Matrix
	Vector4f TransformedCorner = LightViewMatrix * Vector4f(Cascades[Index].Frustrum.Corners[0], 1.0f);
	Vector3f Min = Vector3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), TransformedCorner.Z);
	Vector3f Max = Vector3f(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), TransformedCorner.Z);

	for (int i = 1; i < 8; i++)
	{
		TransformedCorner = LightViewMatrix * Vector4f(Cascades[Index].Frustrum.Corners[i], 1.0f);
		Min.Z = SMath::Min(Min.Z, TransformedCorner.Z);
		Max.Z = SMath::Max(Max.Z, TransformedCorner.Z);
	}

	Matrix4f LightProjectionMatrix = Ortho(-1.0, 1.0, -1.0, 1.0, -Max.Z, -Min.Z);

	Matrix4f LightViewProjectionMatrix = LightProjectionMatrix * LightViewMatrix;


	for (int i = 1; i < 8; i++)
	{
		TransformedCorner = LightViewProjectionMatrix * Vector4f(Cascades[Index].Frustrum.Corners[i], 1.0f);
		
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

	Matrix4f CropMatrix = Translate(Vector3f(OffsetX, OffsetY, 0.0f), Matrix4f::IDENTITY);
	
	CropMatrix = Scale(Vector3f(ScaleX, ScaleY, 1.0f), CropMatrix);

	Cascades[Index].CropMatrix = CropMatrix;

	Cascades[Index].LightProjectionMatrix = CropMatrix * LightProjectionMatrix;

	Cascades[Index].CascadeCPVMatrix = CropMatrix * LightProjectionMatrix * LightViewMatrix;
	

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
		CascadeMatrices[i] = TextureSpaceMatrix * Cascades[i].CascadeCPVMatrix * Inverse(CameraViewMatrix);
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

void PCascadeShadowMap::PrepareForDraw(Shader* ActiveShader, Matrix4f CameraViewMatrix, Matrix4f CameraProjectionMatrix)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, CascadesMVPBuffer);

	Matrix4f* CascadeMatrices = (Matrix4f*)glMapBufferRange(
		GL_SHADER_STORAGE_BUFFER, 0,
		sizeof(Matrix4f) * NumCascades,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	Matrix4f TextureSpaceMatrix = Scale(0.5, Translate(Vector3f(0.5), Matrix4f::IDENTITY));

	//In texture space!
	for (uint32 i = 0; i < NumCascades; i++)
	{
		CascadeMatrices[i] = TextureSpaceMatrix * Cascades[i].CascadeCPVMatrix * Inverse(CameraViewMatrix);
		
		//Bind Textures
		glActiveTexture(GL_TEXTURE10 + i);
		glBindTexture(GL_TEXTURE_2D, Cascades[i].CascadeFBO->DepthStencilAttachment->TextureID);
		std::string index = "shadowMap[" + std::to_string(i) + "]";
		ActiveShader->SetInt(index.c_str(), 10 + i);
	
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	ActiveShader->SetInt("numOfCascades", NumCascades);

	// set far distance of each cascade.
	ActiveShader->SetVec4("FragmentDistance", 
		Vector4f(
			0.5f * (-Cascades[0].Frustrum.FarPlane * CameraProjectionMatrix[2][2] + CameraProjectionMatrix[3][2]) / Cascades[0].Frustrum.FarPlane + 0.5f,
			0.5f * (-Cascades[1].Frustrum.FarPlane * CameraProjectionMatrix[2][2] + CameraProjectionMatrix[3][2]) / Cascades[1].Frustrum.FarPlane + 0.5f,
			0.5f * (-Cascades[2].Frustrum.FarPlane * CameraProjectionMatrix[2][2] + CameraProjectionMatrix[3][2]) / Cascades[2].Frustrum.FarPlane + 0.5f,
			0.5f * (-Cascades[3].Frustrum.FarPlane * CameraProjectionMatrix[2][2] + CameraProjectionMatrix[3][2]) / Cascades[3].Frustrum.FarPlane + 0.5f
		)
	);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, CascadesMVPBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ShadowMapSSBO);
}

PCascade PCascadeShadowMap::GetCascade(uint32 Index)
{
	return Cascades[Index];
}


void PFrustrum::CalculateFrustrumCorners(PCameraComponent* Camera)
{
	float TanHalfVFOV = SMath::Tan(DegreesToRadians(FieldOfView) * 0.5f);

	float HNear = 2 * TanHalfVFOV * NearPlane;
	float HFar = 2 * TanHalfVFOV * FarPlane;

	float WNear = HNear * Ratio;
	float WFar = HFar * Ratio;

	Vector3f CenterNear = Camera->GetPosition() + (Camera->CameraDirection * NearPlane);
	Vector3f CenterFar = Camera->GetPosition() + (Camera->CameraDirection * FarPlane);

	Vector3f RightVector = Camera->Right * (WNear * 0.5f);
	Vector3f UpVector = Camera->Up * (HNear * 0.5f);

	// Near Plane
	Corners[0] = CenterNear + UpVector - RightVector;
	Corners[1] = CenterNear + UpVector + RightVector;
	Corners[2] = CenterNear - UpVector - RightVector;
	Corners[3] = CenterNear - UpVector + RightVector;

	RightVector = Camera->Right * (WFar * 0.5f);
	UpVector = Camera->Up * (HFar * 0.5f);

	// Far Plane
	Corners[4] = CenterFar + UpVector - RightVector;
	Corners[5] = CenterFar + UpVector + RightVector;
	Corners[6] = CenterFar - UpVector - RightVector;
	Corners[7] = CenterFar - UpVector + RightVector;


	Center = (CenterFar + CenterNear) * 0.5;
}