#pragma once

#ifndef ENGINE_H
#define ENGINE_H


#include <Core/Core.h>

#include <Core/SingletonBase.h>
#include <Core/Managers/ManagerBase.h>




class Engine : SingletonBase<Engine>, ManagerBase
{
public:

	static Engine& getInstance();
	static Engine * getInstancePtr();

	Engine();
	//Engine(const uint32 Width, const uint32 Height, const char* WindowName);
	~Engine();
	

	void Run();

	std::unique_ptr<GRenderManager> RenderManager;

	std::unique_ptr<GLogManager> LogManager;


	

	//void framebuffer_size(GLFWwindow* window, int width, int height);
	//void mouse_move(GLFWwindow* window, double xpos, double ypos);
	//void scroll_move(GLFWwindow* window, double xoffset, double yoffset);
	//void processInput(GLFWwindow *window);

	//void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	//
	//// timing
	//float deltaTime = 0.0f;
	//float lastFrame = 0.0f;

	//GLFWwindow* window;
	//uint32 SCR_WIDTH;
	//uint32 SCR_HEIGHT;

	// Inherited via ManagerBase
	virtual void Init() override;

	virtual void Terminate() override;

};

#endif // !ENGINE_H
