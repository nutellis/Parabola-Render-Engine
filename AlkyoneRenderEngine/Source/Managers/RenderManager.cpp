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


#include<Components/RenderComponents/StaticMeshGroup.h>

#include <Components/FBORenderTarget.h>

#include <Components/CascadeShadowMap.h>
#include <Components/AmbientOcclusion.h>

#include <Components/LightComponents/DirectionalLightComponent.h>
#include <Utilities/CameraUtiltities.h>

#include <ParabolaMath.h>
#include <Components/Colliders/AxisAlignedBoundingBox.h>
#include <Components/StaticMesh.h>
#include <RenderHelper.h>
#include <Components/Colliders/IntersectionTests.h>




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
	delete ShadowMap;
	delete AmbientOcclusion;

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


	//TODO: this should be moved inside a generic camera update
	TArray<PAxisAlignedBoundingBox> Receivers = gSceneManager.GetActiveScene()->GetObjectsByIntersection(RenderCamera->Camera->Frustrum->BoundingBox);
	RenderCamera->Camera->AdjustPlanesBasedOnObjects(Receivers);


	//Get all meshgroups
	RenderList.Clear();
	for(RRenderActor * Actors: ActiveScene->SceneActors) 
	{
		RenderList.PushBack(Actors->StaticMeshGroup);
	}

	// Frustrum Cull RenderList
	FrustrumCull(RenderCamera->Camera);



	//TODO: move this somewhere better
	//Update directional light
	PDirectionalLightComponent* Light = ActiveScene->SceneLights.Front()->Light;
	Light->UpdateLight(RenderCamera->Camera->Frustrum->FrustrumBox->Center);

	ShadowMapPass(RenderCamera->Camera);

	AmbientOcclusionPass(ActiveCamera->Camera);

	RenderPass(ActiveCamera->Camera);


	int bitMask = Options.ShowEdges << 1 | Options.ShowPlanes;
	if (Options.ShowCameraFrustrum) {
		
		RenderCamera->Camera->Frustrum->FrustrumBox->RenderDebugBoundingBox(bitMask, Vector4f(1.0, 1.0, 1.0, 0.2), ActiveCamera->Camera->Projection, ActiveCamera->Camera->View);
	}
	if (Options.ShowCameraBoundingBox) {

		RenderCamera->Camera->Frustrum->BoundingBox->RenderDebugBoundingBox(bitMask, Vector4f(1.0, 1.0, 1.0, 0.2), ActiveCamera->Camera->Projection, ActiveCamera->Camera->View);
	}

	if (ShadowMap->NumCascades < ShadowMap->CascadeIndex) {
		ShadowMap->CascadeIndex = ShadowMap->NumCascades;
	}
	if (ShadowMap->ShowCascadeFrustrumDebug) {
		if (ShadowMap->SoloCascadeDebug) {
			ShadowMap->Cascades[ShadowMap->CascadeIndex - 1]->Frustrum->FrustrumBox->RenderDebugBoundingBox(
				bitMask, ShadowMap->Cascades[ShadowMap->CascadeIndex - 1]->CascadeDebugColour, ActiveCamera->Camera->Projection, ActiveCamera->Camera->View);
		}
		else {
			for (auto& Cascade : ShadowMap->Cascades) {
				Cascade->Frustrum->FrustrumBox->RenderDebugBoundingBox(bitMask, Cascade->CascadeDebugColour, ActiveCamera->Camera->Projection, ActiveCamera->Camera->View);
			}
		}
	}

	if (ShadowMap->ShowCascadeBoundingDebug) {
		if (ShadowMap->SoloCascadeDebug) {
			ShadowMap->Cascades[ShadowMap->CascadeIndex - 1]->Frustrum->BoundingBox->RenderDebugBoundingBox(
				bitMask, ShadowMap->Cascades[ShadowMap->CascadeIndex - 1]->CascadeDebugColour, ActiveCamera->Camera->Projection, ActiveCamera->Camera->View);
		}
		else {
			for (auto& Cascade : ShadowMap->Cascades) {
				Cascade->Frustrum->BoundingBox->RenderDebugBoundingBox(bitMask, Cascade->CascadeDebugColour, ActiveCamera->Camera->Projection, ActiveCamera->Camera->View);
			}
		}
	}
	
	if (ShadowMap->ShowLightFrustrumDebug) {
		static TArray<Vector4f> ndcCorners;
		ndcCorners = {
		{-1, 1, -1, 1},
		{ 1, 1, -1, 1},
		{ -1,  -1, -1, 1},
		{1,  -1, -1, 1},
		{-1, 1,  1, 1},
		{ 1, 1,  1, 1},
		{-1,  -1,  1, 1},
		{1,  -1,  1, 1},
			};

		PFrustrum LightFrustrum = PFrustrum();
		TArray<Vector3f> viewCorners;
		for (int i = 0; i < 8; ++i) {
			Vector4f viewPos = Inverse(ShadowMap->Cascades[ShadowMap->CascadeIndex -1]->CascadePVMatrix) * ndcCorners[i];
			viewCorners.PushBack(Vector3f(viewPos) / viewPos.W);  // Perspective divide (though ortho w == 1)
		}
		LightFrustrum.FrustrumBox->Corners = viewCorners;

		LightFrustrum.FrustrumBox->RenderDebugBoundingBox(bitMask, Vector4f(0.85, 0.85, 0.0, 0.6), ActiveCamera->Camera->Projection, ActiveCamera->Camera->View);

		// ugly but it works
		LightFrustrum.FrustrumBox->DebugFrustrumMesh->VAO.Terminate();
		LightFrustrum.FrustrumBox->DebugFrustrumMesh->VBO.Terminate();
	}

	if (Options.ShowBoundingBoxes) {
		TArray<RRenderActor*> VisibleMeshes = ActiveScene->SceneActors;
		for (int i = 0; i < VisibleMeshes.Size(); i++) {
			if (Options.ShowEdges) {
				for (int j = 0; j < VisibleMeshes[i]->StaticMeshGroup->Meshes.Size(); j++) {
					VisibleMeshes[i]->StaticMeshGroup->Meshes[j]->WorldBoundingBox->RenderDebugBoundingBox(
						Options.ShowEdges << 1,
						VisibleMeshes[i]->StaticMeshGroup->Meshes[j]->WorldBoundingBox->DebugColour,
						ActiveCamera->Camera->Projection,
						ActiveCamera->Camera->View
					);
				}
			}
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


void GRenderManager::FrustrumCull(PCameraComponent* Camera) {
	TArray<RStaticMesh*> VisibleMeshes = TArray<RStaticMesh*>();
	PBoundingBox* CameraFrustrumBox = Camera->Frustrum->FrustrumBox;

	bool IsInside = true;
	for (RStaticMeshGroup* Group : RenderList) {
		TArray<RStaticMesh*> Meshes = Group->Meshes;
		for (RStaticMesh* Mesh : Meshes) {
			IsInside = true;
			for (int i = 0; i < 6; ++i)
			{
				PPlane & Plane = Camera->Frustrum->Planes[i];

				// Pick vertex furthest in direction of plane normal
				Vector3f Vertex = Mesh->WorldBoundingBox->Min;
				if (Plane.Normal.X >= 0) Vertex.X = Mesh->WorldBoundingBox->Max.X;
				if (Plane.Normal.Y >= 0) Vertex.Y = Mesh->WorldBoundingBox->Max.Y;
				if (Plane.Normal.Z >= 0) Vertex.Z = Mesh->WorldBoundingBox->Max.Z;

				if (Dot(Plane.Normal, Vertex) + Plane.D < 0) {
					IsInside = false;
					break;
				}
			}
			Mesh->ShouldRender = IsInside;
		}
	}
}


void GRenderManager::ShadowMapPass(PCameraComponent * Camera) {
	Shader* DepthShader = gShaderManager.GetShader("DepthShader");
	//Shader* DepthShader = gShaderManager.GetShader("LinearDepthShader");
	DepthShader->Enable();

	if (ShadowMap == nullptr) {
		ShadowMap = new RCascadeShadowMap(
			Options.NumCascades,
			Camera->Frustrum->FieldOfView,
			Camera->Frustrum->Ratio,
			Camera->Frustrum->NearPlane,
			Camera->Frustrum->FarPlane);

		//set shader
		ShadowMap->Init();

	}
	else if (ShadowMap->ShouldUpdate) {
		delete ShadowMap;

		ShadowMap = new RCascadeShadowMap(
			Options.NumCascades,
			Camera->Frustrum->FieldOfView,
			Camera->Frustrum->Ratio,
			Camera->Frustrum->NearPlane,
			Camera->Frustrum->FarPlane);

		ShadowMap->Init();
	}

	for (int i = 0; i < ShadowMap->NumCascades; i++) {
		//TODO: add a shouldUpdate here to avoid recalculations
		ShadowMap->UpdateCascadeExtends(
			Camera->Frustrum->NearPlane,
			Camera->Frustrum->FarPlane,
			Camera->Frustrum->Ratio,
			Camera->Frustrum->FieldOfView
		);

		ShadowMap->CalculateLightProjection(
			i,
			Camera,
			ActiveScene->SceneLights.Front()->Light,
			ShadowMap->SquareShadowBox
		);

		FBORenderTarget* ShadowMapRenderTarget = ShadowMap->GetCascade(i)->CascadeFBO;
		ShadowMapRenderTarget->Bind();

		glViewport(0, 0, ShadowMap->GetCascade(i)->Resolution, ShadowMap->GetCascade(i)->Resolution);
		//glClearColor(ShadowMap->GetCascade(i)->Far, ShadowMap->GetCascade(i)->Far, ShadowMap->GetCascade(i)->Far, 1.0);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		//glCullFace(GL_FRONT);


		glPolygonOffset(0.8f, 4.0f); 
		glEnable(GL_POLYGON_OFFSET_FILL);



		//DepthShader->SetFloat("near", ShadowMap->GetCascade(i)->Near);
		//DepthShader->SetFloat("far", ShadowMap->GetCascade(i)->Far);
		//DepthShader->SetInt("isOrtho", 1);

		// TODO: draw only shadow casters. Pass a list of meshes to the function
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

void GRenderManager::AmbientOcclusionPass(PCameraComponent* Camera) {
	
	FBORenderTarget* AOInputRenderTarget = AmbientOcclusion->AmbientOcclusionInputFBO;
	AOInputRenderTarget->Bind();

	glViewport(0, 0, AOInputRenderTarget->Width, AOInputRenderTarget->Height);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);
	
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);

	// Render View Space Depth to a texture
	Shader* LinearDepthShader = gShaderManager.GetShader("LinearDepthShader");
	LinearDepthShader->Enable();

	LinearDepthShader->SetFloat("near", Camera->Frustrum->NearPlane);
	LinearDepthShader->SetFloat("far", Camera->Frustrum->FarPlane);
	LinearDepthShader->SetInt("isOrtho", 0);
		
	GLenum depthBuffer[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, depthBuffer);
	float far[4] = { Camera->Frustrum->FarPlane, Camera->Frustrum->FarPlane, Camera->Frustrum->FarPlane, 1.0 };
	glClearBufferfv(GL_COLOR, 0, far);

	DrawScene(
		LinearDepthShader,
		Camera->View,
		Camera->Projection
	);

	// Reconstruct Normals from Depth to a texture
	GLenum normalBuffer[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(1, normalBuffer);
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };
	glClearBufferfv(GL_COLOR, 1, white);

	Shader* ReconstructNormalShader = gShaderManager.GetShader("ReconstructNormalShader");
	ReconstructNormalShader->Enable();

	ReconstructNormalShader->SetFloat("aspectRatio", Camera->Frustrum->Ratio);
	ReconstructNormalShader->SetFloat("fieldOfView", DegreesToRadians(Camera->Frustrum->FieldOfView));
	ReconstructNormalShader->SetFloat("width", (float)AOInputRenderTarget->Width);
	ReconstructNormalShader->SetFloat("height", (float)AOInputRenderTarget->Height);

	// Bind depth
	glActiveTexture(GL_TEXTURE21);
	glBindTexture(GL_TEXTURE_2D, AOInputRenderTarget->ColourAttachments[0]->TextureID);

	RenderHelper::GetQuadActor().DrawFullScreenQuad();

	glUseProgram(0);

	Shader* AOOutputShader = gShaderManager.GetShader("HBAOOutputShader");
	AOOutputShader->Enable();
	
	FBORenderTarget* AOOutputRenderTarget = AmbientOcclusion->AmbientOcclusionOutputFBO;
	AOOutputRenderTarget->Bind();

	glViewport(0, 0, AOOutputRenderTarget->Width, AOOutputRenderTarget->Height);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Prepare data
	AmbientOcclusion->PrepareForDraw(AOOutputShader, Camera);

	// Draw the AO effect
	RenderHelper::GetQuadActor().DrawFullScreenQuad();

	//glBindTexture(GL_TEXTURE_2D, 0);
	//glActiveTexture(GL_TEXTURE14);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glActiveTexture(GL_TEXTURE13);
	//glBindTexture(GL_TEXTURE_2D, 0);

	// Blur the AO
	Shader* AOBlurShader = gShaderManager.GetShader("SSAOBlurShader");
	AOBlurShader->Enable();

	FBORenderTarget* AOBlurRenderTarget = AmbientOcclusion->AmbientOcclusionBlurFBO;
	AOBlurRenderTarget->Bind();
	
	glViewport(0, 0, AOBlurRenderTarget->Width, AOBlurRenderTarget->Height);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	AOBlurShader->SetInt("filterSize", AmbientOcclusion->FilterSizes[AmbientOcclusion->BlurFilter - 1]);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, AOOutputRenderTarget->ColourAttachments[0]->TextureID);

	RenderHelper::GetQuadActor().DrawFullScreenQuad();

	glBindTexture(GL_TEXTURE_2D, 0);

	//AmbientOcclusion->CleanUp();

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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Shader* RenderShader;
		if (ShadowMap->ShowShadowMapDebug) {
			
			RenderShader = gShaderManager.GetShader("CSM_Debug");
		}
		else {
			RenderShader = gShaderManager.GetShader("BRDF_Default");
		}
		RenderShader->Enable();

		//TODO: move this from here
		PCameraActor* RenderCamera = ActiveScene->SceneCameras.FindFirst([](const PCameraActor* At) {

			return At->ObjectName == "Camera";
			});

		RenderShader->SetMat4("cameraView", false, RenderCamera->Camera->View);

		// Shader Options
		RenderShader->SetBool("usePCSS", Options.UsePCSS);
		RenderShader->SetFloat("wLight", Options.LightSize);
		RenderShader->SetBool("enableAO", Options.UseAO);
		RenderShader->SetBool("enableCSM", Options.UseShadows);
		RenderShader->SetBool("debugAO", Options.DebugAO);

		//TODO: remove this from here. change to deffered shading
		//bind normal map 
		glActiveTexture(GL_TEXTURE20);
		glBindTexture(GL_TEXTURE_2D, AmbientOcclusion->AmbientOcclusionInputFBO->ColourAttachments[0]->TextureID);

		if (Options.UseAO) {
			// Bind the SSAO texture
			glActiveTexture(GL_TEXTURE16);
			glBindTexture(GL_TEXTURE_2D, AmbientOcclusion->AmbientOcclusionBlurFBO->ColourAttachments[0]->TextureID);
		}

		//for now get light but later we will need lights!
		TArray<RRenderActor*> Lights = ActiveScene->SceneLights;
		Lights.Front()->Light->SetupShaderLight(RenderShader, Camera->GetViewMatrix());

		//Prepare shadow Data
		if (Options.UseShadows) {
			ShadowMap->PrepareForDraw(RenderShader, Camera->GetViewMatrix(), Camera->GetProjectionMatrix());
		}

		DrawScene(RenderShader, Camera->GetViewMatrix(), Camera->GetProjectionMatrix());

		ShadowMap->CleanUp();

		glDisable(GL_BLEND);
	}
}

void GRenderManager::DrawSkyBox(PCameraComponent* Camera) {
	
	RSkyBoxActor* SkyBox = ActiveScene->GetSkyBox();

	Shader* SkyBoxShader = gShaderManager.GetShader("SkyBoxShader");
	SkyBoxShader->Enable();

	Matrix4f viewMatrix = Camera->GetViewMatrix();
	Matrix4f projectionMatrix = Camera->GetProjectionMatrix();
	
	SkyBox->PrepareForDraw(SkyBoxShader, Camera);

	SkyBox->Draw();

	glUseProgram(0);
}

void GRenderManager::DrawScene(Shader * ShaderToUse, Matrix4f ViewMatrix, Matrix4f ProjectionMatrix)
{
	std::unordered_map<uint32, TArray<RStaticMeshGroup*>> ShaderPerMeshMap;

	if (ActiveScene != nullptr) {

		//get visible meshes. For now just get all meshes.
		TArray<RRenderActor *> MeshesToDraw = ActiveScene->SceneActors;

		ShaderToUse->SetMat4("viewInverse", false, Inverse(ViewMatrix));

		for (int i = 0; i < MeshesToDraw.Size(); i++) {

			MeshesToDraw[i]->SetupModelMatrix();

			ShaderToUse->SetMat4("modelMatrix", false, MeshesToDraw[i]->StaticMeshGroup->ModelMatrix);

			Matrix4f ModelViewMatrix = ViewMatrix * MeshesToDraw[i]->StaticMeshGroup->ModelMatrix;
			Matrix4f ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;

			ShaderToUse->SetMat4("modelViewMatrix", false, ModelViewMatrix);

			ShaderToUse->SetMat4("modelViewProjectionMatrix", false, ModelViewProjectionMatrix);

			ShaderToUse->SetMat4("normalMatrix", false, Matrix4f(Inverse(ModelViewMatrix).GetTransposed()));
			
			MeshesToDraw[i]->StaticMeshGroup->DrawGroup(ShaderToUse);
		}

		//for (int i = 0; i < VisibleMeshes.Size(); i++) {
		//	RStaticMeshGroup * MeshToRender = VisibleMeshes[i]->StaticMeshGroup;
		//	
		//	//TODO: for now we are using hardcoded BRDF_Default for all the meshes
		//	if (MeshToRender != nullptr) {
		//		uint32 ShaderUsed = gShaderManager.GetShader("BRDF_Default")->ID;
		//		ShaderPerMeshMap[ShaderUsed].PushBack(MeshToRender);
		//	}
		//}

		//for (const auto& Pair : ShaderPerMeshMap) {
		//	Shader* ShaderToUse = gShaderManager.GetShader(Pair.first);
		//	TArray <RStaticMeshGroup*> MeshesToRender = Pair.second;
		//	
		//	ShaderToUse->Enable();
		//	//camera
		//	Camera->SetupShaderCamera(ShaderToUse);

		//	// set lights
		//	// Lights.Front()->SetPosition(Vector3f(lightX, 1.5, lightZ));
		//	Lights.Front()->Light->SetupShaderLight(ShaderToUse);

		//	for (int i = 0; i < MeshesToRender.Size(); i++) {
		//		MeshesToRender[i]->SetupModelMatrix(ShaderToUse);
		//		MeshesToRender[i]->DrawGroup(ShaderToUse);
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
	FBORenderTarget* MainRenderTarget = new FBORenderTarget("Main", 1366, 768);
	FBORenderTarget * ShadowMapTarget = new FBORenderTarget("ShadowMap",1024, 1024);


	RTextureOptions ColourOutputOptions = RTextureOptions(
		GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT,
		GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
		GL_NONE, GL_LESS, false);
	TArray<RTextureOptions> ColourAttachmentOptions = TArray<RTextureOptions>(1, ColourOutputOptions);

	MainRenderTarget->Init(ColourAttachmentOptions);
	ShadowMapTarget->Init();

	RenderTargets.PushBack(MainRenderTarget);
	RenderTargets.PushBack(ShadowMapTarget);
	
	AmbientOcclusion = new RAmbientOcclusion();

	AmbientOcclusion->Init();
}

void GRenderManager::Terminate()
{
	GShaderManager::getInstance().Terminate();

	GAssetLoader::getInstance().Terminate();

	LOG(DEBUG, "RENDER_MANAGER Terminated");
}

void GRenderManager::DrawOptions() {

	ImGui::SetNextWindowSize(ImVec2(600, 400));
	ImGui::SetNextWindowPos(ImVec2(0, 770));
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
		ImGui::SameLine();
		ImGui::Checkbox("Show Camera Bounding Box", &Options.ShowCameraBoundingBox);
		ImGui::Checkbox("Show Edges", &Options.ShowEdges);
		ImGui::SameLine();
		ImGui::Checkbox("Show Planes", &Options.ShowPlanes);	
		ImGui::SameLine();
		ImGui::Checkbox("Show Bounding Boxes", &Options.ShowBoundingBoxes);
		ImGui::NewLine();
		ImGui::Separator();

		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("CSM")) {
				//Cascade Shadow Map
				ImGui::Checkbox("Enable Shadows", &Options.UseShadows);
				ImGui::Checkbox("CSM Debug View", &ShadowMap->ShowShadowMapDebug);
				ImGui::SameLine();
				ImGui::Checkbox("Square Box", &ShadowMap->SquareShadowBox);
				if (ImGui::SliderInt("Cascades Number", &Options.NumCascades, 1, 4)) {
					ShadowMap->ShouldUpdate = true;
				}
				ImGui::SliderFloat("Lambda", &ShadowMap->Lambda, 0.0, 1.0);
				ImGui::Checkbox("Cascade Frustrums", &ShadowMap->ShowCascadeFrustrumDebug);
				ImGui::SameLine();
				ImGui::Checkbox("Cascade AABB", &ShadowMap->ShowCascadeBoundingDebug);
				ImGui::SameLine();
				ImGui::Checkbox("Solo", &ShadowMap->SoloCascadeDebug);
				ImGui::SameLine();
				ImGui::Checkbox("Light Frustrum", &ShadowMap->ShowLightFrustrumDebug);
				if (ImGui::SliderInt("Cascade Selected", &ShadowMap->CascadeIndex, 1, ShadowMap->NumCascades))

					//for (int i = 0; i < ShadowMap->NumCascades; i++) {
					//	std::string CascadeTitle = "CSM Level " + std::to_string(i);
					//	ImGui::Checkbox(CascadeTitle.c_str(), &Options.ShowCascade[i]);
					//	ImGui::SameLine();
					//}
					//-------------------------------------------------------------------
					ImGui::NewLine();
				ImGui::Text("Percentage-Close Soft Shadows");
				ImGui::Checkbox("Enable PCSS", &Options.UsePCSS);
				ImGui::SliderFloat("Light Size", &Options.LightSize, 0.0f, 5.0f);
				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("HBAO+")) {

				ImGui::Checkbox("Enable HBAO", &Options.UseAO); ImGui::SameLine();
				ImGui::Checkbox("Debug AO", &Options.DebugAO);
				ImGui::SliderFloat("Radius", &AmbientOcclusion->Radius, 0.3f, 5.0f, "%.3f");
				ImGui::SliderFloat("Bias", &AmbientOcclusion->Bias, 0.0f, 2.0f);
				ImGui::SliderFloat("Exponent", &AmbientOcclusion->Exponent, 1.0f, 16.0f);
				ImGui::SliderFloat("Threshold", &AmbientOcclusion->Threshold, 0.0f, 5.0f);
				ImGui::SliderInt("Blur Filter size", &AmbientOcclusion->BlurFilter, 1, 12);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::Separator();
		if(ImGui::Button("Reload Shaders")) {
			gShaderManager.ReloadShaders();
		}
		ImGui::SameLine();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);

	}
	ImGui::End();
}

void GRenderManager::DrawPreview()
{
	uint32 RenderTarget = 0;
	RenderTarget = RenderTargets[0]->ColourAttachments[0]->TextureID;
	//RenderTarget =AmbientOcclusion->AmbientOcclusionInputFBO->ColourAttachments[0]->TextureID;
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

	//DrawDepthMaps();
}

void GRenderManager::DrawDepthMaps() {
	int CascadesToShow = 0;
	for (auto& ShowCascade : ShadowMap->ShowCascade) {
		if (ShowCascade)
			CascadesToShow++;
	}
	CascadesToShow = CascadesToShow > ShadowMap->NumCascades ? ShadowMap->NumCascades : CascadesToShow;
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
				if (ShadowMap->ShowCascade[i]) {
					uint32 Target = ShadowMap->Cascades[i]->CascadeFBO->ColourAttachments[0]->TextureID;
					
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