#include <Managers\InputManager.h>


#include <Managers/RenderManager.h>
#include <Managers/SceneManager.h>

#include <Managers/LogManager.h>

#include <Components/Scene.h>
#include <Components/RenderActor.h>
#include <Components/RenderComponents/Components.h>

template<> GInputManager* SingletonBase<GInputManager>::instance = 0;
GInputManager & GInputManager::getInstance()
{
	//assert?
	return (*instance);
}


GInputManager * GInputManager::getInstancePtr()
{
	if (instance == nullptr)
	{
		instance = new GInputManager();
	}
	return instance;
}

GInputManager::GInputManager()
{
	lastX = 1280 / 2.0f;
	lastY = 720 / 2.0f;
	firstMouse = true;
}

GInputManager::~GInputManager()
{

}


void GInputManager::KeyPressCallback(GLFWwindow* Window, int key, int scancode, int action, int mods)
{
	PRenderActor* cmr = gSceneManager.GetActiveScene()->GetActiveCameraActor();
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		LOG(INFO, "Attempt to close the window");
		glfwSetWindowShouldClose(Window, true);
	}

	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) {
		cmr->Camera->ProcessKeyboard(FORWARD, 0.3f);
	}
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
		cmr->Camera->ProcessKeyboard(BACKWARD, 0.3f);
	}
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) {
		cmr->Camera->ProcessKeyboard(LEFT, 0.3f);
	}
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) {
		cmr->Camera->ProcessKeyboard(RIGHT, 0.3f);
	}
	if (glfwGetKey(Window, GLFW_KEY_R) == GLFW_PRESS) {
		cmr->Camera->ProcessKeyboard(UP, 0.3f);
	}
	if (glfwGetKey(Window, GLFW_KEY_F) == GLFW_PRESS) {
		cmr->Camera->ProcessKeyboard(DOWN, 0.3f);
	}
	if (glfwGetKey(Window, GLFW_KEY_K) == GLFW_PRESS) {
		cmr->ObjectPosition = Vector3f(-0.0f, 2.0f, -5.0f);
	}

}

void GInputManager::MouseCallback(GLFWwindow * Window, double xpos, double ypos)
{
	RenderWindow *win = gWindowManager.GetRenderWindow();
	PRenderActor* cmr = gSceneManager.GetActiveScene()->GetActiveCameraActor();
	if (glfwGetKey(Window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
		if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && xpos > 375) {
			PRenderActor* cmr = gSceneManager.GetActiveScene()->GetActiveCameraActor();
			
			float xoffset = xpos - win->MouseLastX;
			float yoffset = win->MouseLastY - ypos; // reversed since y-coordinates go from bottom to top

			cmr->Camera->ProcessMouseMovement(xoffset, yoffset);
			
		}
			}
	win->MouseLastY = ypos;
	win->MouseLastX = xpos;

}

void GInputManager::MouseButtonCallback(GLFWwindow * Window, int button, int action, int mods)
{
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
	{
		RenderWindow *win = gWindowManager.GetRenderWindow();

		//std::cout << win->MouseLastX << "\t " << win->MouseLastY << "\n";
	}
}

void GInputManager::ScrollCallback(GLFWwindow * Window, double xoffset, double yoffset)
{
	PRenderActor* cmr = gSceneManager.GetActiveScene()->GetActiveCameraActor();
	cmr->Camera->Zoom -= (float)yoffset * 1.5;
	if (cmr->Camera->Zoom < 1.0f)
		cmr->Camera->Zoom = 1.0f;
	if (cmr->Camera->Zoom > 90.0f)
		cmr->Camera->Zoom = 90.0f;

}


void GInputManager::Init()
{

	LOG(DEBUG, "Initiating INPUT_MANAGER\n");


	ActiveWindow = gWindowManager.GetWindow();

	//Input Callbacks
	glfwSetCursorPosCallback(ActiveWindow,MouseCallback);
	glfwSetMouseButtonCallback(ActiveWindow, MouseButtonCallback);

	glfwSetScrollCallback(ActiveWindow, ScrollCallback);

	glfwSetKeyCallback(ActiveWindow, KeyPressCallback);


	glfwSetInputMode(ActiveWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//cmr = gRenderManager.getActiveCamera();

	LOG(INFO, "INPUT_MANAGER INITIATED\n");
}

void GInputManager::Terminate()
{
	//nothing for now.
	LOG(DEBUG, "INPUT_MANAGER Terminated");
}