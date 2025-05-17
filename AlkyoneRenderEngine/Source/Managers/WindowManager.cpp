#include <Managers/WindowManager.h>


#include <Managers/RenderManager.h>

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <Components/RenderWindow.h>
#include <Components/RenderSystem.h>

#include <Managers/LogManager.h>

#include <ParabolaMath.h>




template<> GWindowManager* SingletonManagerBase<GWindowManager>::instance = 0;
GWindowManager & GWindowManager::getInstance()
{
	//assert?
	return (*instance);
}

GWindowManager * GWindowManager::getInstancePtr()
{
	if (instance == nullptr)
	{
		instance = new GWindowManager();
	}
	return instance;
}

GWindowManager::GWindowManager() 
{
	bInitSuccessful = false;
}

//GWindowManager::GWindowManager(const WindowInfo & Winfo,const ContextInfo & Cinfo)
//{}

GWindowManager::~GWindowManager()
{}



void GWindowManager::Init()
{
	LOG(INFO, "Initiating WINDOW_MANAGER\n");

	//Init GLFW
	if (glfwInit()) 
	{
		LOG(INFO, "GLFW: Initialized");
		if (GRenderManager::getInstance().GetContext()->CoreProfile)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GRenderManager::getInstance().GetContext()->MajorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GRenderManager::getInstance().GetContext()->MinorVersion);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}
		else
		{
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		}


		Window = new RenderWindow();
		Window->Create("Parabola Engine", 2048, 1152, false );

		bInitSuccessful = true;
		LOG(INFO, "WINDOW_MANAGER INITIATED\n");
	}
	else
	{
		LOG(ERROR, "GLFW cannot be initialized");
		LOG(ERROR, "It's fine, really. Nothing is working though. :) ");
		
		LOG(FATAL, "WINDOW MANAGER FAILED INITIATON\n");
		//send terminating signal

		exit(EXIT_FAILURE);
	}

}



void GWindowManager::Terminate()
{
	
	Window->Destroy();

	delete Window;

	glfwTerminate();
	LOG(DEBUG, "GLFW Terminated!");

	LOG(DEBUG, "WINDOW_MANAGER Terminated");
}

RenderWindow * GWindowManager::GetRenderWindow()
{
	return Window;
}

GLFWwindow * GWindowManager::GetWindow()
{
	return Window->Window;
}








/*
bool Window::closed() const
{
	return (bool)glfwWindowShouldClose(wWindow);
}

void Window::update(double time)
{

	processInput(wWindow);
	glfwSwapBuffers(wWindow);
	glfwPollEvents();
	
}

int Window::getWidth() const
{
	return wWidth;
}

int Window::getHeight() const
{
	return wHeight;
}

bool Window::isKeyPressed(uint32 keycode) const
{
	bool isPressed=false;
	if (keycode <= MAX_KEYS)
	{
		isPressed = wKeys[keycode];
	}
	return isPressed;
}

bool Window::isMouseButtonPressed(uint32 button) const
{
	bool isPressed = false;
	if (button <= MAX_BUTTONS)
	{
		isPressed = wMouseButtons[button];
	}
	return isPressed;
}

void Window::getMousePosition(double & x, double & y) const
{
	x = mPosX;
	y = mPosY;
}

void Window::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::init()
{
	bool initStatus = false;
	
	if (glfwInit()) {

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		initStatus = true;
		wWindow = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

		if (!wWindow)
		{
			std::cout << "ERROR::Window cannot be initialized" << std::endl;
			initStatus = false;
		}

		else
		{
			glfwMakeContextCurrent(wWindow);
			glfwSetWindowUserPointer(wWindow, this);
			glfwSetKeyCallback(wWindow, key_callback);
			glfwSetMouseButtonCallback(wWindow, mouse_button_callback);
			glfwSetCursorPosCallback(wWindow, cursor_position_callback);
			glfwSetWindowSizeCallback(wWindow, windowResize);

			if (GLEW_OK != glewInit())
			{
				std::cout << "Failed to initialize GLEW" << std::endl;
				initStatus = false;
			}
		}
	}
	return initStatus;
}

void windowResize(GLFWwindow *window,int width, int height)	
{
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	Window *win = (Window*)glfwGetWindowUserPointer(window);
	win->wKeys[key] = action != GLFW_RELEASE;

	
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	Window *win = (Window*)glfwGetWindowUserPointer(window);
	win->wMouseButtons[button] = action != GLFW_RELEASE;
}

void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	Window *win = (Window*)glfwGetWindowUserPointer(window);
	win->mPosX = xpos;
	win->mPosY = ypos;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}
*/

