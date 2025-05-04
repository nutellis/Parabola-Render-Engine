#include <Core/Engine.h>


#include <Managers/RenderManager.h>


#include <Managers/WindowManager.h>

#include <Managers/InputManager.h>

#include <Managers/LogManager.h>

#include <Managers/GuiManager.h>

#include <Core/Time.h>
#include <Core/Editor.h>


template<> Engine* SingletonManagerBase<Engine>::instance = 0;
Engine & Engine::getInstance()
{
	//assert?
	return (*instance);
}

Engine * Engine::getInstancePtr()
{
	return instance;
}

Engine::Engine()
{
	LogManager = GLogManager::getInstancePtr();

	WindowManager = GWindowManager::getInstancePtr();

	RenderManager = GRenderManager::getInstancePtr();
	
	InputManager = GInputManager::getInstancePtr();

	GUIManager = GGUIManager::getInstancePtr();

	Editor = GEditor::getInstancePtr();

	// LOG(DEBUG, "All systems initialized");
	
}

Engine::~Engine()
{
	RenderManager = nullptr;

	InputManager = nullptr;

	if (GLogManager::getInstancePtr()) {

		LogManager = nullptr;
	}

	GUIManager = nullptr;

	WindowManager = nullptr;
}

void Engine::Run()
{

	double accum = 0.0;

	
	Time::LastTime = Time::Seconds();

	LOG(INFO, "Engine is Running Full SPEEED :)\n");
	
	while (!glfwWindowShouldClose(GWindowManager::getInstance().GetWindow()))
	{
		Time::CurrentTime = Time::Seconds();

		Time::DeltaTime = Time::CurrentTime - Time::LastTime;
		

		Time::LastTime = Time::CurrentTime;

		//accum += Time::DeltaTime;
		
		glfwPollEvents();
		
	
		//while (accum < Time::CurrentTime)
		//{
			
		RenderManager->Render(Time::DeltaTime);
			//LOG(CMD, "FPS: %f", 1/Time::FixedDeltaTime);
			
	
			//LOG(CMD, "TEST: %f", Time::DeltaTime*100.0);


		//	accum += Time::FixedDeltaTime;
		//}
		glfwSwapBuffers(gWindowManager.GetWindow());
			
	}
}

void Engine::Init()
{
	//Set up time stuff before everything else.
	Time::InitTime();

	GLogManager::getInstance().Init();

	GWindowManager::getInstance().Init();

	GRenderManager::getInstance().Init();

	GInputManager::getInstance().Init();

	GGUIManager::getInstance().Init();

	GEditor::getInstance().Init();

}

void Engine::Terminate()
{

	GRenderManager::getInstance().Terminate();

	GInputManager::getInstance().Terminate();

	GLogManager::getInstance().Terminate();

	GGUIManager::getInstance().Terminate();

	GEditor::getInstance().Terminate();

	GWindowManager::getInstance().Terminate();

	LOG(DEBUG, "All systems went to sleep. Engine is no more");
}
