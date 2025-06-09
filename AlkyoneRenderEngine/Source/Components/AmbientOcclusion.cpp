#include "Components/AmbientOcclusion.h"

#include <Core/RenderCore.h>
#include <Components/FBORenderTarget.h>
#include <Components/Shader.h>
#include <Components/Texture.h>
#include "RenderHelper.h"
#include <Components/VertexBuffers.h>
#include <Components/CameraComponents/Camera.h>





RAmbientOcclusion::RAmbientOcclusion()
{
	NumDirections = 8;
	NumSteps = 8;
	AmbientOcclusionInputFBO = new FBORenderTarget("AmbientOcclusionInput", 1366, 768, 2);
	AmbientOcclusionOutputFBO = new FBORenderTarget("AmbientOcclusionOutput", 1366, 768);
	AmbientOcclusionBlurFBO = new FBORenderTarget("AmbientOcclusionBlur", 1366, 768);


	Kernel = TArray<Vector3f>(NumDirections);
	for (int i = 0; i < NumDirections; i++) {
		// Generate a cosine-weighted sample in the unit hemisphere
		Vector3f Sample = CosineSampleHemisphere();

		// Scale the sample to create a volume by multiplying with a random factor [0, 1]
		float Scale = RandomFloat();
		Sample = Sample * Scale;

		float ScaleBias = float(i) / float(NumDirections); // Scale biasing factor

		Sample = Sample * Lerp(0.1f, 1.0f, ScaleBias);       // Push samples closer to the origin for earlier indices

		Kernel.PushBack(Sample);
	}

	// generate noise texture
	// ----------------------
	TArray<Vector3f> NoiseArray = TArray<Vector3f>(16);
	for (unsigned int i = 0; i < 16; i++)
	{
		float Angle = RandomFloat() * 2.0f * PI / NumDirections;

		// we need cos and sin of the angle to jitter the sample directions
		Vector3f Noise(SMath::Cos(Angle), SMath::Sin(Angle), RandomFloat()); 
		NoiseArray.PushBack(Noise);
	}

	glGenTextures(1, &NoiseMap);
	glBindTexture(GL_TEXTURE_2D, NoiseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &NoiseArray[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

RAmbientOcclusion::~RAmbientOcclusion()
{
	if (AmbientOcclusionInputFBO) {
		delete AmbientOcclusionInputFBO;
		AmbientOcclusionInputFBO = nullptr;
	}	
	if (AmbientOcclusionOutputFBO) {
		delete AmbientOcclusionOutputFBO;
		AmbientOcclusionOutputFBO = nullptr;
	}
	if (AmbientOcclusionBlurFBO) {
		delete AmbientOcclusionBlurFBO;
		AmbientOcclusionBlurFBO = nullptr;
	}

}

void RAmbientOcclusion::Init()
{

	TArray<RTextureOptions> ColourInputOptions = TArray<RTextureOptions>(2);

	ColourInputOptions.PushBack(RTextureOptions());
	ColourInputOptions.PushBack(RTextureOptions(
		GL_TEXTURE_2D, GL_RGBA32F, GL_RGBA, GL_FLOAT,
		GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
		GL_NONE, GL_LESS, false));

	if (AmbientOcclusionInputFBO) {
		AmbientOcclusionInputFBO->Init(ColourInputOptions);
	}

	RTextureOptions ColourOutputOptions = RTextureOptions(
		GL_TEXTURE_2D, GL_RGBA32F, GL_RGBA, GL_FLOAT,
		GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
		GL_NONE, GL_LESS, false);

	TArray<RTextureOptions> ColourAttachmentOptions = TArray<RTextureOptions>(1, ColourOutputOptions);
	if (AmbientOcclusionOutputFBO) {
		AmbientOcclusionOutputFBO->Init(ColourAttachmentOptions);
	}
	if (AmbientOcclusionBlurFBO) {
		AmbientOcclusionBlurFBO->Init(ColourAttachmentOptions);
	}
}

void RAmbientOcclusion::PrepareForDraw(Shader* ActiveShader, PCameraComponent * Camera)
{
	ActiveShader->SetMat4("projectionMatrix", false, Camera->Projection);
	ActiveShader->SetFloat("radius", 2.0f);
	ActiveShader->SetFloat("bias", 0.1f);
	ActiveShader->SetFloat("Nd", (float)NumDirections);
	ActiveShader->SetFloat("Ns", (float)NumSteps);
	ActiveShader->SetFloat("powerExponent", 2.0f);
	ActiveShader->SetFloat("fieldOfView", DegreesToRadians(Camera->Frustrum->FieldOfView));
	ActiveShader->SetFloat("aspectRatio", Camera->Frustrum->Ratio);

	// Bind textures
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, AmbientOcclusionInputFBO->ColourAttachments[0]->TextureID);
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, AmbientOcclusionInputFBO->ColourAttachments[1]->TextureID);

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, NoiseMap);
}
