#include <Managers/RenderManager.h>
#include <Components/RenderSystem.h>


#include <Managers/WindowManager.h>
#include <Managers/ShaderManager.h>

#include <Components/Shader.h>

#include <ParabolaMath.h>

#include<Core/Time.h>

#include <unordered_map>


#include <glm/glm.hpp>

#include <Utilities/OBJ_Loader.h>
#include <Managers/GUIManager.h>
#include <Managers/SceneManager.h>
#include <Components/Scene.h>
#include <Components/RenderActor.h>
#include <Components/SkyBox.h>
#include <Components/Material.h>
#include<Components/RenderComponents/StaticMeshComponent.h>

#include <Components/FBORenderTarget.h>


template<> GRenderManager* SingletonBase<GRenderManager>::instance = 0;
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

	vao = VertexArrayObject();

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

//void GRenderManager::SetCommonUniforms(const PCameraComponent& Camera) {
//
//	//get active shader (TEMP)
//	Shader* tempActiveShader = new Shader();
//
//	// Set view and projection matrices
//	Matrix4f ViewMatrix = Camera.GetViewMatrix();
//	Matrix4f ProjectionMatrix = Camera.GetProjectionMatrix();
//
//	tempActiveShader->SetMat4(tempActiveShader->Uniforms.ViewLocation, ViewMatrix);
//	tempActiveShader->SetMat4(tempActiveShader->Uniforms.ProjectionLocation, ProjectionMatrix);
//}


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

float lightX;
float lightZ;
void GRenderManager::Render(double DeltaTime)
{

	//change to framebuffer
	gGuiManager.BindRenderTarget();

	glViewport(0, 0, gGuiManager.GetRenderTarget()->Width, gGuiManager.GetRenderTarget()->Height);
	glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ActiveScene = gSceneManager.GetActiveScene();
	
	ActiveScene->GetActiveCameraActor()->ControlCamera(gGuiManager.GetRenderTarget()->Width, gGuiManager.GetRenderTarget()->Height);

	DrawSkyBox();
	DrawScene();


	// Draw GUI
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 1920, 1080);


	gGuiManager.Draw();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);	
}

void GRenderManager::DrawSkyBox() {
	
	Shader* shader = gShaderManager.GetShader("SkyBoxShader");
	shader->Enable();
	
	Matrix4f viewMatrix = ActiveScene->GetActiveCameraActor()->Camera->GetViewMatrix();
	Matrix4f projectionMatrix = ActiveScene->GetActiveCameraActor()->Camera->GetProjectionMatrix();

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, ActiveScene->GetSkyBox()->EnviromentMap->textureID);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, ActiveScene->GetSkyBox()->IrradianceMap->textureID);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, ActiveScene->GetSkyBox()->ReflectionMap->textureID);
	glActiveTexture(GL_TEXTURE0);

	float environment_multiplier = 1.3f;
	shader->setFloat("environment_multiplier", environment_multiplier);
	shader->SetMat4("inv_PV", false, (projectionMatrix * viewMatrix).Inverse());
	shader->setVec3("camera_pos", ActiveScene->GetActiveCameraActor()->GetPosition());

	ActiveScene->GetSkyBox()->Draw();

	shader->Disable();
}

void GRenderManager::DrawScene()
{
	std::unordered_map<uint32, TArray<PStaticMeshComponent*>> ShaderPerMeshMap;

	if (ActiveScene != nullptr) {

		//get Camera
		PCameraComponent* Camera = ActiveScene->GetActiveCameraActor()->Camera;

		//for now get light but later we will need lights!
		TArray<PRenderActor*> Lights = ActiveScene->SceneLights;

		//get visible meshes. For now just get all meshes.
		TArray<PRenderActor *> VisibleMeshes = ActiveScene->SceneMeshes;
		for (int i = 0; i < VisibleMeshes.Size(); i++) {
			PStaticMeshComponent * MeshToRender = VisibleMeshes[i]->StaticMesh;
			
			//TODO: for now we are using hardcoded BRDF_Default for all the meshes
			if (MeshToRender != nullptr) {
				uint32 ShaderUsed = gShaderManager.GetShader("BRDF_Default")->ID;
				ShaderPerMeshMap[ShaderUsed].PushBack(MeshToRender);
			}
		}

		for (const auto& Pair : ShaderPerMeshMap) {
			Shader* ShaderToUse = gShaderManager.GetShader(Pair.first);
			TArray <PStaticMeshComponent*> MeshesToRender = Pair.second;
			
			ShaderToUse->Enable();
			//camera
			Camera->SetupShaderCamera(ShaderToUse);

			// set lights
			// Lights.Front()->SetPosition(Vector3f(lightX, 1.5, lightZ));
			Lights.Front()->Light->SetupShaderLight(ShaderToUse);

			for (int i = 0; i < MeshesToRender.Size(); i++) {
				MeshesToRender[i]->SetupModelMatrix(ShaderToUse);
				MeshesToRender[i]->DrawComponent(ShaderToUse);
			}
		}
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
}

void GRenderManager::Terminate()
{
	

	GWindowManager::getInstance().Terminate();

	GShaderManager::getInstance().Terminate();

	GAssetLoader::getInstance().Terminate();

	LOG(DEBUG, "RENDER_MANAGER Terminated");
}



//
//Renderer::Renderer()
//{
//	s = Shader("H:/Users/Nutellis/Documents/Projects/OpenGLEngine/Engine-core/Shaders/vertexshader.vs", "H:/Users/Nutellis/Documents/Projects/OpenGLEngine/Engine-core/Shaders/fragmentshader.fs");
//
//	l = Shader("H:/Users/Nutellis/Documents/Projects/OpenGLEngine/Engine-core/Shaders/lamp.vs", "H:/Users/Nutellis/Documents/Projects/OpenGLEngine/Engine-core/Shaders/lightshader.fs");
//
//	t = Shader("H:/Users/Nutellis/Documents/Projects/OpenGLEngine/Engine-core/Shaders/TextShader.vs", "H:/Users/Nutellis/Documents/Projects/OpenGLEngine/Engine-core/Shaders/TextFragShader.fs");
//
//	//lampShader = Shader("C:/Users/Nutellis/Documents/Visual Studio 2015/Projects/OpenGLTutorial/Engine-core/lamp.vs", "C:/Users/Nutellis/Documents/Visual Studio 2015/Projects/OpenGLTutorial/Engine-core/lamp.fs");
//camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
//	
//
//	light = new Actor();
//	light->AddLight();
//	
//	camera = new Actor();
//
//	camera->AddCamera();
//
//	object = new Actor();
//	object->ObjectS();
//	object->SetOuter(scene);
//
//	object2= new Actor();
//	object2->SetOuter(scene);
//	
//	
//	object2->RootComponent->RelativeLocation = glm::vec3(1.3f, 0.6f, -1.1f);
//	object2->ObjectS();
//	object2->RootComponent->RelativeLocation = glm::vec3(1.3f, 0.6f, -1.1f);
//	object2->StaticMesh->RelativeLocation = glm::vec3(1.3f, 0.6f, -1.1f);
//	object2->StaticMesh->GetMaterial()->mDiffuse->Colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
//	scene = new Scene();
//
//	scene->AddActor(ActorType::LIGHT, light);
//	
//	scene->AddActor(ActorType::CAMERA, camera);
//
//	scene->AddActor(ActorType::ACTOR, object);
//	
//	scene->AddActor(ActorType::ACTOR, object2);
//
//
//	//std::cout << mesh->GetMaterial();
//	//s.enable();
//	//s.setMaterial(mesh->GetMaterial());
//	angle = 45.0f;
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	text = new Text();
//	
//}
//
//
//void Renderer::Draw(double time)
//{
//
//	//----------------TEMP
//
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glm::mat4 rotation = glm::mat4();
//	rotation = glm::rotate(rotation, 0.01f, glm::vec3(0, 1, 0));
//	light->RootComponent->RelativeLocation = rotation * glm::vec4(light->RootComponent->RelativeLocation,1);
//	light->StaticMesh->RelativeLocation = light->RootComponent->RelativeLocation;
//
//	scene->Draw(s,l);
//	//if (time > 59.0) {
//		text->RenderText(t, "fps" + std::to_string(time), 15, 5, 1, glm::vec3(1, 1, 1));
//	//}
//	//text->RenderText(t, std::to_string(object->StaticMesh->angle), 400, 6, 1, glm::vec3(0.1, 0.6, 0.8));
//	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
//	//glm::mat4 view = camera->Camera->GetViewMatrix();
//	//s.setMat4("projection", projection);
//	//s.setMat4("view", view);
//
//	//angle += 0.010f;
//	//// draw mesh
//	//glm::mat4 model;
//	//model = glm::mat4();
//	//model =
//		//glm::scale(model, glm::vec3(0.5f));
//	//
//	//s.setMat4("model", model);
//	//s.setVec3("pLight.position", glm::vec3(0.5f, 0.5f, 0.5f));
//
//	//mesh->GetMesh()->Draw(s);
//	//object->StaticMesh->GetMesh()->Draw(s);
//	//
//	//lampShader.enable();
//	//lampShader.setMat4("projection", projection);
//	//lampShader.setMat4("view", view);
//	//
//
//	//
//	//glm::mat4 model;
//	//model = glm:: mat4();
//	//model = glm::translate(model,light->RootComponent->Location);
//	//model = glm::scale(model, glm::vec3(0.09f));
//	//lampShader.setMat4("model", model);
//
//	//light->StaticMesh->GetMesh()->Draw(s);
//
//	//glBindVertexArray(vao);
//	//glDrawElements(GL_TRIANGLES, mesh->GetMesh()->indices.size(), GL_UNSIGNED_INT, 0);
//	//glBindVertexArray(0);
//
//
//	//---------------!TEMP
//
//}
//