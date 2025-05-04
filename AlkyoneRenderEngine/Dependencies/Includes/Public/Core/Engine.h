#pragma once

#ifndef ENGINE_H
#define ENGINE_H


#include <Core/RenderCore.h>

#include <Core/SingletonBase.h>




class Engine : SingletonManagerBase<Engine>
{
public:

	static Engine& getInstance();
	static Engine * getInstancePtr();

	Engine();
	//Engine(const uint32 Width, const uint32 Height, const char* WindowName);
	~Engine();

	void Run();

	GWindowManager* WindowManager;

	GRenderManager * RenderManager;

	//TODO TEMP INPUT MANAGER
	GInputManager * InputManager;

	GLogManager * LogManager;

	GGUIManager* GUIManager;

	GEditor * Editor;


	

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



private:


};

#endif // !ENGINE_H
