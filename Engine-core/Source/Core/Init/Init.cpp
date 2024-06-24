//#include <Core\Init\Init.h>
//
//#include <iostream>
//
//void InitGraphics::InitGlew()
//{
//
//	if (glewInit() == GLEW_OK)
//	{
//		std::cout << "GLEW: Initialize" << std::endl;
//	}
//
//	if (glewIsSupported("GL_VERSION_4_5"))
//	{
//		std::cout << "GLEW GL_VERSION_4_5 is 4.5\n ";
//	}
//	else
//	{
//		std::cout << " GLEW GL_VERSION_4_5 not supported\n ";
//	}
//}
//
//
//void InitGraphics::InitGLFW(Window * initWindow, const ContextInfo & context, const FramebufferInfo & framebuffer)
//{
//	if (glfwInit()) {
//
//
//		if (context.core)
//		{
//			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context.MajorVersion);
//			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context.MinorVersion);
//			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//		}
//		else
//		{
//			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
//		}
//
//		initWindow->wWindow = glfwCreateWindow(initWindow->wInfo.wWidth, initWindow->wInfo.wHeight, initWindow->wInfo.wTitle, NULL, NULL);
//
//		if (!initWindow->wWindow)
//		{
//			std::cout << "ERROR::Window cannot be initialized" << std::endl;
//		}
//		else
//		{
//			glfwMakeContextCurrent(initWindow->wWindow);
//			glfwSetWindowPos(initWindow->wWindow, 100, 100);
//
//			//glfwSetWindowSize(initWindow->wWindow, initWindow->wInfo.wWidth, initWindow->wInfo.wHeight);
//
//
//			glfwSetWindowUserPointer(initWindow->wWindow, initWindow);
//			glfwSetKeyCallback(initWindow->wWindow, key_callback);
//			glfwSetMouseButtonCallback(initWindow->wWindow, mouse_button_callback);
//			glfwSetCursorPosCallback(initWindow->wWindow, cursor_position_callback);
//			//glfwSetWindowSizeCallback(wWindow->wWindow, windowResize);
//
//			InitGlew();
//
//		}
//	}
//	else 
//	{
//		
//	}
//}
//
