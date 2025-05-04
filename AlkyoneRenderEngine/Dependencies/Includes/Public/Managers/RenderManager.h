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

	int32 TargetToRender = 0;

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

	void DrawDepthMap(FBORenderTarget * DepthMap);

};


#endif // !RENDERER_H
