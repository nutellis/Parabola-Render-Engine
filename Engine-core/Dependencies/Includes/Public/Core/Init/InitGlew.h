#pragma once



class InitGlew 
{


};





















//
//
//bool Window::init()
//{
//	bool initStatus = false;
//
//	if (glfwInit()) {
//
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//		initStatus = true;
//		wWindow = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
//
//		if (!wWindow)
//		{
//			std::cout << "ERROR::Window cannot be initialized" << std::endl;
//			initStatus = false;
//		}
//
//		else
//		{
//			glfwMakeContextCurrent(wWindow);
//			glfwSetWindowUserPointer(wWindow, this);
//			glfwSetKeyCallback(wWindow, key_callback);
//			glfwSetMouseButtonCallback(wWindow, mouse_button_callback);
//			glfwSetCursorPosCallback(wWindow, cursor_position_callback);
//			glfwSetWindowSizeCallback(wWindow, windowResize);
//
//			if (GLEW_OK != glewInit())
//			{
//				std::cout << "Failed to initialize GLEW" << std::endl;
//				initStatus = false;
//			}
//		}
//	}
//	return initStatus;
//}