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
class PCascadeShadowMap;
class GGUIManager;
class Shader;
class FBORenderTarget;

struct RenderOptions {
public:
	//Cascade Shadow Mapping
	bool ShowShadowMapDebug = false;
	bool ShowCascade[4];
	bool ToggleShadowView = false;
	int32 NumCascades = 4;
	float Lambda = 0.5;

	//Percentage-Closer Soft Shadows
	bool UsePCSS = false;
	float LightSize = 0.1;

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

	PCascadeShadowMap* ShadowMap;

	TArray<FBORenderTarget* > RenderTargets;

	RenderOptions Options;

public:

	void GetContextInfo();

	RenderSystem * GetContext();

	//void SetCommonUniforms(const PCameraComponent& Camera);

	void Render(double currentTime);

	void ShadowMapPass();
	void RenderPass();

	void DrawSkyBox(Shader* SkyBoxShader);

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
