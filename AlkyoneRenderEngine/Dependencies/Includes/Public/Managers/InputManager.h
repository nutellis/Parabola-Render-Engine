#pragma once

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Core/RenderCore.h>
#include <GLFW\glfw3.h>



#include <Core/SingletonBase.h>
#include <Managers/ManagerBase.h>


#include <Managers/WindowManager.h>

#include <Components/Camera.h>
#include <Components/RenderWindow.h>


class GInputManager : SingletonBase<GInputManager>, ManagerBase
{
public:


	static GInputManager & getInstance();

	static GInputManager * getInstancePtr();

	GInputManager();
	~GInputManager();

	static void KeyPressCallback(GLFWwindow * Window, int key, int scancode, int action, int mods);

	static void MouseCallback(GLFWwindow* Window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* Window, int button, int action, int mods);


	static void ScrollCallback(GLFWwindow* Window, double xoffset, double yoffset);

	GLFWwindow * ActiveWindow;


	// Inherited via ManagerBase
	virtual void Init() override;

	virtual void Terminate() override;



private:
	//Camera * cmr;

	//TVector2<int32> MouseDrag;
};


#endif //!INPUTMANAGER_H