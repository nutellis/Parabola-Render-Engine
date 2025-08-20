#pragma once

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H


#include <Core/RenderCore.h>

#include <GLFW/glfw3.h>

#include <Core/SingletonBase.h>
#include <Utilities/Containers/Array.h>
#include <ParabolaMath.h>



class RenderSystem;
class Scene;
class RCascadeShadowMap;
class GGUIManager;
class Shader;
class FBORenderTarget;
class PCameraComponent;
class RAmbientOcclusion;
class RRenderActor;

struct RenderOptions {
public:
	bool ShowCameraFrustrum = false;
	bool ShowCameraBoundingBox = false;
	bool ShowEdges = false;
	bool ShowPlanes = false;
	bool ShowBoundingBoxes = false;

	//Cascade Shadow Mapping
	bool UseShadows = true;
	int32 NumCascades = 4;


	//Percentage-Closer Soft Shadows
	bool UsePCSS = false;
	float LightSize = 0.1f;

	// SSAO
	bool UseAO = false;
	bool DebugAO = false;

	int32 TargetToRender = 100;

	RenderOptions() {}
};

class GRenderManager : SingletonManagerBase<GRenderManager>
{
public:

	static GRenderManager & getInstance();
	static GRenderManager * getInstancePtr();


	GRenderManager();
	~GRenderManager();

	GShaderManager * ShaderManager;

	GAssetLoader * AssetLoader;
	
private:
	RenderSystem * OpenGLSystem;

	Scene* ActiveScene;

	RCascadeShadowMap* ShadowMap;
	RAmbientOcclusion* AmbientOcclusion;

	TArray<FBORenderTarget* > RenderTargets;

	RenderOptions Options;

public:

	void GetContextInfo();

	RenderSystem * GetContext();

	//void SetCommonUniforms(const PCameraComponent& Camera);

	void Render(double currentTime);

	void ShadowMapPass(PCameraComponent* Camera);

	void AmbientOcclusionPass(PCameraComponent* Camera);

	void RenderPass(PCameraComponent* Camera);

	void DrawSkyBox(PCameraComponent* Camera);

	void DrawScene(Shader* ShaderToUse, Matrix4f ViewMatrix, Matrix4f ProjectionMatrix);

	void Update(double DeltaTime);

	// Inherited via ManagerBase
	virtual void Init() override;

	virtual void Terminate() override;

	void DrawOptions();

	void DrawPreview();

	void DrawDepthMaps();


};


#endif // !RENDERER_H
