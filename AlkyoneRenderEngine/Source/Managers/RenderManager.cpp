#include <Managers/RenderManager.h>
#include <Components/RenderSystem.h>


#include <Managers/WindowManager.h>
#include <Managers/ShaderManager.h>

#include <Components/Shader.h>

#include <ParabolaMath.h>

#include<Core/Time.h>


#include <glm/glm.hpp>

#include <Utilities/OBJ_Loader.h>
#include <Managers/GUIManager.h>
#include <Managers/SceneManager.h>
#include <Components/Scene.h>
#include <Components/RenderActor.h>
#include <Components/Material.h>




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

	WindowManager = GWindowManager::getInstancePtr();

	OpenGLSystem = new RenderSystem();

	ShaderManager = GShaderManager::getInstancePtr();

	AssetLoader = GAssetLoader::getInstancePtr();

	vao = VertexArrayObject();

}

GRenderManager::~GRenderManager()
{
	WindowManager = nullptr;

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

void GRenderManager::Render(double DeltaTime)
{
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//change to framebuffer
	GUIMANAGER.BindRenderTarget();

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO: remove magic numbers
	glViewport(0, 0, 1280, 720);

	Matrix4f  model = Matrix4f::IDENTITY;

	//TODO: move all that in scene manager
	

	//camera first
	const float radius = 2.0f;
	float lightX = SMath::Sin(glfwGetTime()) * radius;
	float lightZ = SMath::Cos(glfwGetTime()) * radius;

	auto activeCamera = SCENEMANAGER.GetActiveScene()->GetActiveCameraActor();
		
	activeCamera->ControlCamera();

	unsigned int modelLoc;
	unsigned int viewLoc;

	////light
	glUseProgram(SHADERMANAGER.GetShader("Light")->ID);
	light.VAO.Bind();

	model = Matrix4f::IDENTITY;

	model = Scale(Vector3f(0.2f), model);

	model = Translate(Vector3f(lightX, 1.5f, lightZ), model);
	
	modelLoc = glGetUniformLocation(SHADERMANAGER.GetShader("Light")->ID, "model");
	viewLoc = glGetUniformLocation(SHADERMANAGER.GetShader("Light")->ID, "view");

	// TODO: create a getActiveShader function
	SHADERMANAGER.GetShader("Light")->SetMat4("projection", false, activeCamera->Camera->Projection);

	glUniformMatrix4fv(modelLoc, 1, GL_TRUE, model[0]);

	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, activeCamera->Camera->View[0]);
	glDrawElements(GL_TRIANGLES, light.Indices.Size(), GL_UNSIGNED_INT, 0);
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
	auto simpleShader = SHADERMANAGER.GetShader("SimpleShader");
	simpleShader->Enable();
	//
	// be sure to activate shader when setting uniforms/drawing objects
	simpleShader->setVec3("light.position", Vector3f(lightX, 1.5f, lightZ));
	//SHADERMANAGER.GetShader("SimpleShader")->setVec3("light.position", Vector3f(lightX, 1.5f, lightZ));

	simpleShader->setFloat("light.cutOff", SMath::Cos(DegreesToRadians(12.5f)));
	simpleShader->setFloat("light.outerCutOff", SMath::Cos(DegreesToRadians(17.5f)));
	// light properties
	simpleShader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	// we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
	// each environment and lighting type requires some tweaking to get the best out of your environment.
	simpleShader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	simpleShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	simpleShader->setFloat("light.constant", 1.0f);
	simpleShader->setFloat("light.linear", 0.09f);
	simpleShader->setFloat("light.quadratic", 0.032f);

	simpleShader->setVec3("viewPos", activeCamera->ObjectPosition); // Vector3f(0.0f, 1.5f, 4.0f));

	//view
	simpleShader->SetMat4("view", true, activeCamera->Camera->View);

	//projection
	simpleShader->SetMat4("projection", false, activeCamera->Camera->Projection);

	SCENEMANAGER.GetActiveScene()->GetRoot()->DrawMeshChildren(simpleShader);

	std::string fps;
	fps = "Delta Time: "+ std::to_string(glfwGetTime());
	// draw text 
	text->RenderText(*SHADERMANAGER.GetShader("Text"), fps, 1.0f, 1.0f, 0.5f, glm::vec3(1, 1, 1));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 1920, 1080);

	
	GUIMANAGER.Draw();

	glfwSwapBuffers(WINDOWMANAGER.GetWindow());
	
}


void GRenderManager::tempFunction()
{

	//temp
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	text = new Text();

	cmr = new Camera();

	cube =  new Cube();
	pyramid = new Pyramid();
	//cubemesh = PStaticMesh(cube->Vertices, cube->Indices);

	light = PStaticMesh(ASSETLOADER.LoadAsset("Assets/cube.obj"));
	
	light.SetupBuffers();
	
	cubemesh = PStaticMesh(ASSETLOADER.LoadAsset("Assets/cubetest.fbx"));
	
	cubemesh.SetupBuffers();

	pyramidmesh = PStaticMesh(ASSETLOADER.LoadAsset("Assets/tri.fbx"));

	pyramidmesh.SetupBuffers();


	planemesh = PStaticMesh(plane.Vertices, plane.Indices);

	planemesh.SetupBuffers();

	//cubemesh.VAO.Bind();
	//pyramidmesh.VAO.Bind();

	//lightPos = Vector3f(1.2f, 1.0f, 2.0f);
	lightPos = Vector3f(0.0f, 1.0f, 0.0f);

	//test = ModelManager();
	//test.loadModel("dragon.obj");

	//vao.CreateArray();
	//vbo.CreateBuffer(test.vertexBuff.size() * sizeof(float), &test.vertexBuff);//Vertices.SizeOf(), Vertices.Begin());
	//ebo.CreateBuffer(test.indexBuff.size() * sizeof(uint32), &test.indexBuff);

	//vao.SetupAttribute(vbo.GetIndex(), 0, 0);
	////vao.SetupAttribute(vbo.GetIndex(), 2, offsetof(VertexFormat, VertexFormat::TextureCoords));
	////vao.SetupAttribute(vbo.GetIndex(), 1, offsetof(VertexFormat, VertexFormat::Color));


	//vao.AttachVertexBuffer(vbo.GetID(), vbo.GetIndex(), sizeof(VertexFormat));
	//vao.AttachElementBuffer(ebo.GetID());

	//vao.SetupAttribute(vbo.GetIndex(), 0, offsetof(VertexFormat, VertexFormat::Position));


	//vao.AttachVertexBuffer(vbo.GetID(),0, sizeof(float));
	//vao.AttachElementBuffer(ebo.GetID());

}

void GRenderManager::Init()
{
	//TODO i ll probably have to change this. No need for windowmanager to be
	//part of the renderer. Take it out, init and go on with your lives
	LOG(DEBUG, "Initiating RENDER_MANAGER\n");
	GWindowManager::getInstance().Init();
	//first init the window
	
	//init context
	OpenGLSystem->Init();


	//init shaders, gbuffers etc i aint know

	GShaderManager::getInstance().Init();

	GAssetLoader::getInstance().Init();


	tempFunction();
	
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