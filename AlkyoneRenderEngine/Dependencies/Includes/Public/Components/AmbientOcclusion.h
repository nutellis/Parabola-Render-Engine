#pragma once
#ifndef AMBIENTOCCLUSION
#define AMBIENTOCCLUSION

#include <ParabolaMath.h>
#include <Utilities/Containers/Array.h>

class FBORenderTarget;
class Shader;
class PCameraComponent;



struct RHBAOOptions {
	bool Enable = true;
	float Radius = 0.5f; // Radius of the ambient occlusion effect
	float Bias = 0.025f; // Bias to avoid self-occlusion
	float Intensity = 1.0f; // Intensity of the ambient occlusion effect
	float Power = 2.0f; // Power to control the falloff of the effect
	bool Blur = true; // Whether to apply a blur to the ambient occlusion
	int32 BlurRadius = 5; // Radius for the blur effect

	int32 SampleCount = 16; // Number of samples to use for ambient occlusion
	int32 StepCount = 8; // Number of steps to take in the ray marching process
};



class RAmbientOcclusion {

public:
	RAmbientOcclusion();
	~RAmbientOcclusion();

	void Init();
	void PrepareForDraw(Shader* ActiveShader, PCameraComponent* Camera);

public:
	FBORenderTarget* AmbientOcclusionInputFBO;
	FBORenderTarget* AmbientOcclusionOutputFBO;
	FBORenderTarget* AmbientOcclusionBlurFBO;



	int NumDirections;
	int NumSteps;

	float Radius = 2.0f;
	float Bias = 0.5f;
	float Exponent = 2.0f;

	TArray<Vector3f> Kernel;

	//TODO: change this
	uint32 NoiseMap;
	int32 FilterSizes[12] = { 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25 };

};


#endif // !AMBIENTOCCLUSION
