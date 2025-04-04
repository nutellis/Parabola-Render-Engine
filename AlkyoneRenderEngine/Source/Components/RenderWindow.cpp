#include <Components/RenderWindow.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <Managers/LogManager.h>

RenderWindow::RenderWindow()
{
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	//LOG(INFO, "%d", count);
	PrimaryMonitor =glfwGetPrimaryMonitor();//monitors[1];//
	

	MouseLastX = 0;
	MouseLastY = 0;
	FirstMouse = true;

	VSync = false;
	
}

RenderWindow::~RenderWindow()
{}
	

RenderWindow::RenderWindow(const char * newName, uint32 newWidth, uint32 newHeight, uint8 FullScreen)
{

}


uint8 RenderWindow::IsVSyncEnabled() const
{
	return VSync;
}

void RenderWindow::Create(const char * newName, uint32 newWidth, uint32 newHeight, uint8 FullScreen)
{

	Name = newName;
	Width = newWidth;
	Height = newHeight;

	//const GLFWvidmode* Mode = glfwGetVideoMode(PrimaryMonitor);

	//Window Initialization
	
	if (FullScreen) {
		Window = glfwCreateWindow(Width, Height, Name, PrimaryMonitor, NULL);
	}
	else
	{
		Window = glfwCreateWindow(Width, Height, Name, NULL, NULL);
	}
	if (!Window)
	{
		LOG(ERROR, "Window cannot be initialized");
	}
	else
	{
		LOG(INFO, "WINDOW: Initialized \t Width: %d Height : %d Title : %s", Width, Height, Name);
		
		bIsActive = 1;

		glfwSetWindowSizeLimits(Window, 720, 480,
		glfwGetVideoMode(PrimaryMonitor)->width,
		glfwGetVideoMode(PrimaryMonitor)->height);
		
		
		glfwSetWindowAspectRatio(Window, 16, 9);
		
		// Make the window's context current 
		glfwMakeContextCurrent(Window);
		glfwSetWindowUserPointer(Window, this);
		
		//Vsync
		//glfwSwapInterval(VSync);
		glfwSwapInterval(1);

		//callbacks

		glfwSetWindowCloseCallback(Window, OnCloseCallback);

		//Set GLFW error callback
		//	glfwSetErrorCallback(ErrorCallback);

		glfwSetFramebufferSizeCallback(Window, OnResizeCallback);
		glfwSetWindowSizeCallback(Window, OnResizeCallback);

		//Input Callbacks
		//glfwSetCursorPosCallback(wWindow, CursorPositionCallback);
		//glfwSetKeyCallback(Window, KeyPressCallback);
	}

}

void RenderWindow::Destroy(void)
{

	glfwDestroyWindow(Window);
	//TODO: assert some shit
	LOG(DEBUG, "Window Destroyed");
	
}


//glfwGetWindowParam(intparam )


void RenderWindow::getMetrics(unsigned int& OutWidth, unsigned int& OutHeight, unsigned int& OutColourDepth)
{
	OutWidth = Width;
	OutHeight = Height;
	//OutColourDepth = ColourDepth;
}


//Callbacks

void RenderWindow::OnCloseCallback(GLFWwindow * window)
{
	LOG(INFO, "User attempted to close the window!!");
}

//void RenderWindow::KeyPressCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//	{
//		LOG(INFO, "Attempt to close the window");
//		glfwSetWindowShouldClose(window, true);
//	}
//	//float cameraSpeed = 2.5 * glfwGetTime();
//	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//	//	cameraPos += cameraSpeed * cameraFront;
//	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//	//	cameraPos -= cameraSpeed * cameraFront;
//	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//	//	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//	//	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//	//Todo : heyoooo
//
//}

//void RenderWindow::CursorPositionCallback(GLFWwindow * window, double xpos, double ypos)
//{
//	LOG(CMD, "X: %f \t y: %f", xpos, ypos);
//}

void RenderWindow::ResizeWindow(int Width, int Height)
{
	LOG(DEBUG, "Resizing Window");
//	glfwSetWindowSize(Window, Width, Height);
}

//Error
void RenderWindow::ErrorCallback(int ErrorCode, const char * Description)
{
	LOG(ERROR, "%s", Description);
}

void RenderWindow::OnResizeCallback(GLFWwindow * window, int width, int height)
{
	int newWidth, newHeight;
	glfwGetFramebufferSize(window, &newWidth, &newHeight);
	glViewport(0, 0, newWidth, newHeight);

	//LOG(CMD, "Window is resizing. Width: %d Height: %d", width, height);
}

uint8 RenderWindow::IsFullScreen(void) const
{

	return PrimaryMonitor != NULL;
}
