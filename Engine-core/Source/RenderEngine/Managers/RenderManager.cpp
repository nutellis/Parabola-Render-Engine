#include <RenderEngine\Managers\RenderManager.h>
#include <RenderEngine/Components/RenderSystem.h>


#include <RenderEngine/Managers/WindowManager.h>
#include <RenderEngine/Managers/ShaderManager.h>

#include <RenderEngine/Components/Shader.h>

#include <ParabolaMath.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<> GRenderManager* SingletonBase<GRenderManager>::instance = 0;
GRenderManager & GRenderManager::getInstance()
{
	//assert?
	return (*instance);
}

GRenderManager * GRenderManager::getInstancePtr()
{
	return instance;
}

GRenderManager::GRenderManager()
{

	

	WindowManager.reset(new GWindowManager());

	OpenGLSystem = new RenderSystem();

	ShaderManager.reset(new GShaderManager());

	vao = VertexArrayObject();

}

GRenderManager::~GRenderManager()
{
	WindowManager.reset();

	delete OpenGLSystem;

	ShaderManager.reset();
}

void GRenderManager::GetContextInfo()
{
	OpenGLSystem->Info();
}


RenderSystem * GRenderManager::GetContext()
{
	return OpenGLSystem;
}


void GRenderManager::Render(double currentTime)
{
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Matrix4f  model = Matrix4f::IDENTITY;
	


	Matrix4f  view = Matrix4f::IDENTITY;
	//glm::mat4 view = glm::mat4(1.0f);


	glm::mat4 projection = glm::mat4(1.0f);

	model = Scale(Vector3f(2.5f), model);
	

	model = Rotate(Vector3f(0.0f, 0.0f, 1.0f), currentTime*10.f, model);

	

	view = Translate(Vector3f(0.0f, 0.0f, -1.0f), view);

	projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);


	unsigned int modelLoc = glGetUniformLocation(GShaderManager::getInstance().GetShader("SimpleShader")->ID, "model");
	unsigned int viewLoc = glGetUniformLocation(GShaderManager::getInstance().GetShader("SimpleShader")->ID, "view");


	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
	GShaderManager::getInstance().GetShader("SimpleShader")->setMat4("projection",projection);


	glDrawArrays(GL_TRIANGLES, 0, 6);


	glfwSwapBuffers(GWindowManager::getInstance().GetWindow());
	glfwPollEvents();
}

void GRenderManager::tempFunction()
{

	vbo = VertexBufferObject();

	vao.CreateArray();

	vbo.CreateBuffer(sizeof(plane.Vertices), plane.Vertices);
		
	vao.SetupArray(vbo.Index, 0, offsetof(VertexFormat, VertexFormat::Position));
	vao.SetupArray(vbo.Index, 1, offsetof(VertexFormat, VertexFormat::Color));

	vao.BindBuffer(vbo.ID, vbo.Index, sizeof(VertexFormat));
	
	vao.Bind();
	
	glUseProgram(GShaderManager::getInstance().GetShader("SimpleShader")->ID);

}

void GRenderManager::Init()
{

	LOG(DEBUG, "Initiating RENDER_MANAGER\n");
	GWindowManager::getInstance().Init();
	//first init the window
	
	//init context
	OpenGLSystem->Init();

	//init shaders, gbuffers etc i aint know

	GShaderManager::getInstance().Init();

	tempFunction();
	LOG(INFO, "RENDER_MANAGER INITIATED\n");
}

void GRenderManager::Terminate()
{
	
	GWindowManager::getInstance().Terminate();

	GShaderManager::getInstance().Terminate();

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
//	//camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
//	object2->StaticMesh->GetMaterial()->mDiffuse->ch_Colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
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
//	//glBindVertexArray(VAO);
//	//glDrawElements(GL_TRIANGLES, mesh->GetMesh()->indices.size(), GL_UNSIGNED_INT, 0);
//	//glBindVertexArray(0);
//
//
//	//---------------!TEMP
//
//}
//
