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
#include <Components/CameraComponents/Camera.h>
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

	PCameraActor* ActiveCamera = ActiveScene->GetActiveCamera();


	PCameraActor* RenderCamera = ActiveScene->SceneCameras.FindFirst([](const PCameraActor* At) {

		return At->ObjectName == "Camera";
		});
		
	if (ActiveCamera == nullptr || RenderCamera == nullptr) {
		return;
	}

	ActiveCamera->ControlCamera(1366, 768);
	RenderCamera->ControlCamera(1366, 768);

	ShadowMapPass(RenderCamera->Camera);

	RenderPass(ActiveCamera->Camera);

	PDirectionalLightComponent * light = ActiveScene->SceneLights.Front()->Light;
	int bitMask = Options.ShowCameraPlanes << 1 | Options.ShowCameraEdges;
	if (Options.ShowCameraFrustrum) {
		
		RenderCamera->Camera->Frustrum.RenderDebugFrustrum(bitMask, Vector4f(1.0, 1.0, 1.0, 0.2), ActiveCamera->Camera->Projection, ActiveCamera->Camera->View, light->LightDirection);
	}

	if (Options.ShowCascadeFrustrumDebug) {
		for (auto& Cascade : ShadowMap->Cascades) {
			Cascade->Frustrum->RenderDebugFrustrum(bitMask, Cascade->CascadeDebugColour, ActiveCamera->Camera->Projection, ActiveCamera->Camera->View, light->LightDirection);
		}
	}

	// Draw GUI
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 2048, 1152);
	glClearColor(0.3f, 0.2f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gGuiManager.Draw();

	glUseProgram(0);	
}

void GRenderManager::ShadowMapPass(PCameraComponent * Camera) {
	Shader* DepthShader = gShaderManager.GetShader("DepthShader");
	DepthShader->Enable();

	if (ShadowMap == nullptr || ShadowMap->NumCascades != Options.NumCascades) {
		ShadowMap = new PCascadeShadowMap(
			Options.NumCascades,
			Camera->Frustrum.FieldOfView,
			Camera->Frustrum.Ratio,
			Camera->Frustrum.NearPlane,
			Camera->Frustrum.FarPlane);

		//set shader

		DepthShader->SetFloat("near_plane",Camera->Frustrum.NearPlane);
		DepthShader->SetFloat("far_plane", Camera->Frustrum.FarPlane);

		ShadowMap->Init();

	}

	for (int i = 0; i < ShadowMap->NumCascades; i++) {
		FBORenderTarget* ShadowMapRenderTarget = ShadowMap->GetCascade(i)->CascadeFBO;
		ShadowMapRenderTarget->Bind();

		glViewport(0, 0, ShadowMap->GetCascade(i)->Resolution, ShadowMap->GetCascade(i)->Resolution);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		//glCullFace(GL_FRONT);


		glPolygonOffset(1.0f, 4.0f); 
		glEnable(GL_POLYGON_OFFSET_FILL);

		//TODO: add a shouldUpdate here to avoid recalculations
		ShadowMap->UpdateCascadeExtends(
			Camera->Frustrum.NearPlane,
			Camera->Frustrum.FarPlane,
			Camera->Frustrum.Ratio,
			Camera->Frustrum.FieldOfView
		);

		ShadowMap->CalculateLightProjection(
			i, 
			Camera,
			ActiveScene->SceneLights.Front()->Light
		);

		DrawScene(
			DepthShader, 
			ShadowMap->GetCascade(i)->LightViewMatrix, 
			ShadowMap->GetCascade(i)->LightProjectionMatrix
		);

		glCullFace(GL_BACK);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	glUseProgram(0);
}

void GRenderManager::RenderPass(PCameraComponent* Camera)
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

		DrawSkyBox(Camera);

		Shader* RenderShader;
		if (Options.ShowShadowMapDebug) {
			RenderShader = gShaderManager.GetShader("CSM_Debug");
		}
		else {
			RenderShader = gShaderManager.GetShader("BRDF_Default");
		}
		RenderShader->Enable();
		
		// Shader Options
		RenderShader->SetBool("usePCSS", Options.UsePCSS);
		RenderShader->SetFloat("wLight", Options.LightSize);

		//for now get light but later we will need lights!
		TArray<PRenderActor*> Lights = ActiveScene->SceneLights;
		Lights.Front()->Light->SetupShaderLight(RenderShader, Camera->GetViewMatrix());

		//Prepare shadow Data
		ShadowMap->PrepareForDraw(RenderShader, Camera->GetViewMatrix(), Camera->GetProjectionMatrix());

		DrawScene(RenderShader, Camera->GetViewMatrix(), Camera->GetProjectionMatrix());

		ShadowMap->UnbindBuffers();
	}
}

void GRenderManager::DrawSkyBox(PCameraComponent* Camera) {
	
	PSkyBox* SkyBox = ActiveScene->GetSkyBox();

	Shader* SkyBoxShader = gShaderManager.GetShader("SkyBoxShader");
	SkyBoxShader->Enable();

	Matrix4f viewMatrix = Camera->GetViewMatrix();
	Matrix4f projectionMatrix = Camera->GetProjectionMatrix();

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, SkyBox->EnviromentMap->TextureID);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, SkyBox->IrradianceMap->TextureID);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, SkyBox->ReflectionMap->TextureID);

	float environment_multiplier = 1.3f;
	SkyBoxShader->SetFloat("environment_multiplier", environment_multiplier);
	SkyBoxShader->SetMat4("inv_PV", false, Inverse(projectionMatrix * viewMatrix));
	SkyBoxShader->SetVec3("camera_pos", Camera->GetPosition());

	SkyBox->Draw();

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
			VisibleMeshes[i]->SetupModelMatrix();

			Matrix4f ModelViewMatrix = ViewMatrix * VisibleMeshes[i]->ModelMatrix;
			Matrix4f ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;

			ShaderToUse->SetMat4("modelViewMatrix", false, ModelViewMatrix);

			ShaderToUse->SetMat4("modelViewProjectionMatrix", false, ModelViewProjectionMatrix);

			ShaderToUse->SetMat4("normalMatrix", false, Inverse(ModelViewMatrix).GetTransposed());
			
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
	FBORenderTarget* MainRenderTarget = new FBORenderTarget("Main", 1600, 900);
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
	ImGui::SetNextWindowPos(ImVec2(1440, 601));
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Options", 0, window_flags);
	{
		//General Options
		ImGui::Text("Camera");
		ImGui::Checkbox("Show Camera Frustrum", &Options.ShowCameraFrustrum);
		ImGui::Checkbox("Show Planes", &Options.ShowCameraEdges);
		ImGui::Checkbox("Show Edges", &Options.ShowCameraPlanes);
		ImGui::Separator();
		//Cascade Shadow Map
		ImGui::Text("Cascade Shadow Map");
		ImGui::SliderInt("Cascades Number", &Options.NumCascades, 1, 4);
		if (ImGui::SliderFloat("Lambda", &Options.Lambda, 0.0, 1.0)) {
			ShadowMap->Lambda = Options.Lambda;
		}
		ImGui::Checkbox("Show Cascade Frustrums", &Options.ShowCascadeFrustrumDebug);
		ImGui::Checkbox("CSM Debug View", &Options.ShowShadowMapDebug);

		for (int i = 0; i < ShadowMap->NumCascades; i++) {
			std::string CascadeTitle = "CSM Level " + std::to_string(i);
			ImGui::Checkbox(CascadeTitle.c_str(), &Options.ShowCascade[i]);
			ImGui::SameLine();
		}
		//-------------------------------------------------------------------
		ImGui::NewLine();
		ImGui::Text("Percentage-Close Soft Shadows");
		ImGui::Checkbox("Enable PCSS", &Options.UsePCSS);
		ImGui::SliderFloat("Light Size", &Options.LightSize, 0.0f, 1.0f);
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
	RenderTarget = RenderTargets[0]->GetTexture();
	if (RenderTarget != 0) {
		ImGui::SetNextWindowSize(ImVec2(1366, 768));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGuiWindowFlags window_flags = 0;
		window_flags =
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoBringToFrontOnFocus;

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

	DrawDepthMaps();
}

void GRenderManager::DrawDepthMaps() {
	int CascadesToShow = 0;
	for (auto& ShowCascade : Options.ShowCascade) {
		if (ShowCascade)
			CascadesToShow++;
	}
	CascadesToShow = CascadesToShow > Options.NumCascades ? Options.NumCascades : CascadesToShow;
	if (CascadesToShow > 0) {
		float Width = (132 * CascadesToShow) + (8 * (CascadesToShow - 1)) + (2 * ImGui::GetStyle().WindowPadding.x);
		ImGui::SetNextWindowSize(ImVec2(Width, 168));
		ImGui::SetNextWindowPos(ImVec2(0, 768 - 168));
		ImGuiWindowFlags window_flags = 0;
		window_flags =
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoFocusOnAppearing;

		ImGui::Begin("CSM Levels", 0, window_flags);
		{
			for (int i = 0; i < ShadowMap->NumCascades; i++) {
				if (Options.ShowCascade[i]) {
					uint32 Target = ShadowMap->Cascades[i]->CascadeFBO->GetTexture();
					
					ImGui::Image(
						(ImTextureID)Target,
						ImVec2(132, 132),
						ImVec2(0, 1),
						ImVec2(1, 0)
					);
					ImGui::SameLine();
				}
			}

		}
		ImGui::End();
	}
	/*ImGui::SetNextWindowSize(ImVec2(128,128));
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
	ImGui::End();*/
}