#include <Managers/RenderManager.h>
#include <Components/RenderSystem.h>


#include <Managers/WindowManager.h>
#include <Managers/ShaderManager.h>
#include <Managers/GUIManager.h>
#include <Managers/SceneManager.h>
#include <Utilities/AssetLoader.h>


#include<Core/Time.h>

#include <unordered_map>

#include <glm/glm.hpp>

#include <Components/Shader.h>
#include <Components/Scene.h>
#include <Components/RenderActor.h>
#include <Components/SkyBox.h>


#include<Components/RenderComponents/StaticMeshComponent.h>

#include <Components/FBORenderTarget.h>
#include <Components/CascadeShadowMap.h>
#include <Components/LightComponents/DirectionalLightComponent.h>
#include <Utilities/CameraUtiltities.h>

#include <ParabolaMath.h>


template<> GRenderManager* SingletonManagerBase<GRenderManager>::instance = 0;
GRenderManager & GRenderManager::getInstance()
{
	//assert?
	return (*instance);
}

GRenderManager * GRenderManager::getInstancePtr()
{
	if (instance == nullptr)
	{
		instance = new GRenderManager();
	}
	return instance;
}

GRenderManager::GRenderManager()
{
	OpenGLSystem = new RenderSystem();

	ShaderManager = GShaderManager::getInstancePtr();

	AssetLoader = GAssetLoader::getInstancePtr();

}

GRenderManager::~GRenderManager()
{
	delete OpenGLSystem;

	ShaderManager = nullptr;
}

void GRenderManager::GetContextInfo()
{
	OpenGLSystem->Info();
}


RenderSystem * GRenderManager::GetContext()
{
	return OpenGLSystem;
}

//Per Frame Rendering :
//
//Fetch Scene Data :
//Query the SceneManager for visible meshes, active camera, and lights.
//Set Uniforms :
//For each visible mesh :
//Bind the appropriate shader.
//Set shader uniforms using data from the SceneManager :
//Transformation Matrices : Model, View, Projection matrices.
//Lighting Information : Positions, colors, intensities of lights.
//Camera Information : Camera position and orientation.
//Issue the draw call.


void GRenderManager::Render(double DeltaTime)
{

	ActiveScene = gSceneManager.GetActiveScene();

	ActiveScene->GetActiveCameraActor()->ControlCamera(1280, 720);

	ShadowMapPass();

	RenderPass();

	// Draw GUI
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 1920, 1080);
	glClearColor(0.3f, 0.2f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gGuiManager.Draw();

	glUseProgram(0);	
}

void GRenderManager::ShadowMapPass() {
	Shader* DepthShader = gShaderManager.GetShader("DepthShader");
	DepthShader->Enable();

	if (ShadowMap == nullptr) {
		ShadowMap = new PCascadeShadowMap(
			4,
			ActiveScene->GetActiveCameraActor()->Camera->FieldOfView,
			ActiveScene->GetActiveCameraActor()->Camera->AspectRatio,
			ActiveScene->GetActiveCameraActor()->Camera->ZNear,
			ActiveScene->GetActiveCameraActor()->Camera->ZFar);

		//set shader

		DepthShader->SetFloat("near_plane", ActiveScene->GetActiveCameraActor()->Camera->ZNear);
		DepthShader->SetFloat("far_plane", ActiveScene->GetActiveCameraActor()->Camera->ZFar);

		ShadowMap->Init();

	}

	for (int i = 0; i < 4; i++) {
		FBORenderTarget* ShadowMapRenderTarget = ShadowMap->GetCascade(i).CascadeFBO;
		ShadowMapRenderTarget->Bind();

		glViewport(0, 0, ShadowMap->GetCascade(i).Resolution, ShadowMap->GetCascade(i).Resolution);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glCullFace(GL_FRONT);
		ShadowMap->CalculateLightProjection(i, ActiveScene->GetActiveCameraActor()->Camera, ActiveScene->SceneLights.Front()->Light);

		DrawScene(DepthShader, ShadowMap->GetCascade(i).LightViewMatrix, ShadowMap->GetCascade(i).LightProjectionMatrix);
		glCullFace(GL_BACK);
	}

	glUseProgram(0);
}

void GRenderManager::RenderPass()
{
	CHECK_GL_ERROR();
	FBORenderTarget* MainRenderTarget = RenderTargets.FindFirst([](const FBORenderTarget* At) {
		return At->Name == "Main";
		});

	if (MainRenderTarget) {
		MainRenderTarget->Bind();

		glViewport(0, 0, MainRenderTarget->Width, MainRenderTarget->Height);
		glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader* SkyBoxShader = gShaderManager.GetShader("SkyBoxShader");
		SkyBoxShader->Enable();
		DrawSkyBox(SkyBoxShader);

		Shader* BrdfShader = gShaderManager.GetShader("BRDF_Default");
		BrdfShader->Enable();

		//get Camera
		PCameraComponent* Camera = ActiveScene->GetActiveCameraActor()->Camera;

		//for now get light but later we will need lights!
		TArray<PRenderActor*> Lights = ActiveScene->SceneLights;
		Lights.Front()->Light->SetupShaderLight(BrdfShader);

		//Prepare shadow Data
		ShadowMap->PrepareForDraw(BrdfShader, Camera->GetViewMatrix(), Camera->GetProjectionMatrix());

		DrawScene(BrdfShader, Camera->GetViewMatrix(), Camera->GetProjectionMatrix());

		ShadowMap->UnbindBuffers();
	}
}

void GRenderManager::DrawSkyBox(Shader* SkyBoxShader) {
	

	Matrix4f viewMatrix = ActiveScene->GetActiveCameraActor()->Camera->GetViewMatrix();
	Matrix4f projectionMatrix = ActiveScene->GetActiveCameraActor()->Camera->GetProjectionMatrix();

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, ActiveScene->GetSkyBox()->EnviromentMap->TextureID);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, ActiveScene->GetSkyBox()->IrradianceMap->TextureID);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, ActiveScene->GetSkyBox()->ReflectionMap->TextureID);

	float environment_multiplier = 1.3f;
	SkyBoxShader->SetFloat("environment_multiplier", environment_multiplier);
	SkyBoxShader->SetMat4("inv_PV", false, Inverse(projectionMatrix * viewMatrix));
	SkyBoxShader->SetVec3("camera_pos", ActiveScene->GetActiveCameraActor()->GetPosition());

	ActiveScene->GetSkyBox()->Draw();

	SkyBoxShader->Disable();
}

void GRenderManager::DrawScene(Shader * ShaderToUse, Matrix4f ViewMatrix, Matrix4f ProjectionMatrix)
{
	std::unordered_map<uint32, TArray<PStaticMeshComponent*>> ShaderPerMeshMap;

	if (ActiveScene != nullptr) {

		//get visible meshes. For now just get all meshes.
		TArray<PRenderActor *> VisibleMeshes = ActiveScene->SceneMeshes;

		ShaderToUse->SetMat4("viewInverse", false, Inverse(ViewMatrix));

		for (int i = 0; i < VisibleMeshes.Size(); i++) {
			VisibleMeshes[i]->SetupModelMatrix(ShaderToUse);

			Matrix4f ModelViewMatrix = ViewMatrix * VisibleMeshes[i]->ModelMatrix;
			Matrix4f ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;

			ShaderToUse->SetMat4("modelViewMatrix", false, ModelViewMatrix);

			ShaderToUse->SetMat4("modelViewProjectionMatrix", false, ModelViewProjectionMatrix);

			ShaderToUse->SetMat4("normalMatrix", false, Inverse(ModelViewMatrix.GetTransposed()));
			
			VisibleMeshes[i]->StaticMesh->DrawComponent(ShaderToUse);
		}


		//for (int i = 0; i < VisibleMeshes.Size(); i++) {
		//	PStaticMeshComponent * MeshToRender = VisibleMeshes[i]->StaticMesh;
		//	
		//	//TODO: for now we are using hardcoded BRDF_Default for all the meshes
		//	if (MeshToRender != nullptr) {
		//		uint32 ShaderUsed = gShaderManager.GetShader("BRDF_Default")->ID;
		//		ShaderPerMeshMap[ShaderUsed].PushBack(MeshToRender);
		//	}
		//}

		//for (const auto& Pair : ShaderPerMeshMap) {
		//	Shader* ShaderToUse = gShaderManager.GetShader(Pair.first);
		//	TArray <PStaticMeshComponent*> MeshesToRender = Pair.second;
		//	
		//	ShaderToUse->Enable();
		//	//camera
		//	Camera->SetupShaderCamera(ShaderToUse);

		//	// set lights
		//	// Lights.Front()->SetPosition(Vector3f(lightX, 1.5, lightZ));
		//	Lights.Front()->Light->SetupShaderLight(ShaderToUse);

		//	for (int i = 0; i < MeshesToRender.Size(); i++) {
		//		MeshesToRender[i]->SetupModelMatrix(ShaderToUse);
		//		MeshesToRender[i]->DrawComponent(ShaderToUse);
		//	}
		//}
	}
}

void GRenderManager::Init()
{
	LOG(DEBUG, "Initiating RENDER_MANAGER\n");
	
	//init context
	OpenGLSystem->Init();

	//init shaders, gbuffers etc i aint know

	GShaderManager::getInstance().Init();

	GAssetLoader::getInstance().Init();
	
	LOG(INFO, "RENDER_MANAGER INITIATED\n");


	// init render targets
	FBORenderTarget * MainRenderTarget = new FBORenderTarget("Main", 1280, 720);
	FBORenderTarget * ShadowMapTarget = new FBORenderTarget("ShadowMap",1024, 1024);

	MainRenderTarget->Init();
	ShadowMapTarget->Init();

	RenderTargets.PushBack(MainRenderTarget);
	RenderTargets.PushBack(ShadowMapTarget);


}

void GRenderManager::Terminate()
{
	GShaderManager::getInstance().Terminate();

	GAssetLoader::getInstance().Terminate();

	LOG(DEBUG, "RENDER_MANAGER Terminated");
}

void GRenderManager::DrawOptions() {

	ImGui::SetNextWindowSize(ImVec2(600, 400));
	ImGui::SetNextWindowPos(ImVec2(1290, 601));
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Options", 0, window_flags);
	{
		ImGui::RadioButton("Main", &TargetToRender, 100);
		for (int i = 0; i < ShadowMap->NumCascades; i++) {
			std::string CascadeTitle = "CSM Level " + std::to_string(i);
			ImGui::RadioButton(CascadeTitle.c_str(), &TargetToRender, i);
		}
		ImGui::Separator();
		if(ImGui::Button("Reload Shaders")) {
			gShaderManager.ReloadShaders();
		}
	}
	ImGui::End();
}

void GRenderManager::DrawPreview()
{
	uint32 RenderTarget = 0;
	if (TargetToRender == 100) {
		RenderTarget = RenderTargets[0]->GetTexture();
		if (RenderTarget != 0) {
			ImGui::SetNextWindowSize(ImVec2(1280, 720));
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoResize;

			ImGui::Begin("Editor", 0, window_flags);
			{
				ImGui::Image(
					(ImTextureID)RenderTarget,
					ImGui::GetContentRegionAvail(),
					ImVec2(0, 1),
					ImVec2(1, 0)
				);
			}
			ImGui::End();
		}
	}
	else {
		DrawDepthMap(ShadowMap->GetCascade(TargetToRender).CascadeFBO);
	}
}

void GRenderManager::DrawDepthMap(FBORenderTarget * DepthMap) {
	ImGui::SetNextWindowSize(ImVec2(DepthMap->Width,DepthMap->Height));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;

	ImGui::Begin("DepthMap", 0, window_flags);
	{
		ImGui::Image(
			(ImTextureID)DepthMap->GetTexture(),
			ImGui::GetContentRegionAvail(),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
	}
	ImGui::End();
}