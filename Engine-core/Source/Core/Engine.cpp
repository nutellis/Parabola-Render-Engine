#include <Core/Engine.h>

#include <RenderEngine/Managers/RenderManager.h>


#include <RenderEngine/Managers/WindowManager.h>



#include <Core/LogManager.h>



template<> Engine* SingletonBase<Engine>::instance = 0;
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
	if (!GLogManager::getInstancePtr()) {

		LogManager.reset(new GLogManager());
	}

	RenderManager.reset(new GRenderManager());

	


	LOG(DEBUG, "All systems initialized");
	
}

Engine::~Engine()
{
	RenderManager.reset();

	if (GLogManager::getInstancePtr()) {

		LogManager.reset();
	}
}

void Engine::Run()
{
	LOG(INFO, "Engine is Running Full SPEEED :)\n");
	while (!glfwWindowShouldClose(GWindowManager::getInstance().GetWindow()))
	{

		RenderManager->Render(glfwGetTime());
	}
}

void Engine::Init()
{

	GLogManager::getInstance().Init();


	GRenderManager::getInstance().Init();

}

void Engine::Terminate()
{
	GRenderManager::getInstance().Terminate();

	GLogManager::getInstance().Terminate();

	LOG(DEBUG, "All systems went to sleep. Engine is no more");
}
