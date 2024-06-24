//#include <Input.h>
//
//void InputHandler::addBinding(int key, const Callback& callback)
//{
//	mCallbacks[key].push_back(callback);
//}
//
//void InputHandler::onKeyPress(int key)
//{
//	for (Callback& callback : mCallbacks[key])
//	{
//		callback();
//	}
//}
//
//bool InputHandler::isKeyPressed(uint32 keycode) const
//{
//	bool isPressed = false;
//	if (keycode <= MAX_KEYS)
//	{
//		isPressed = wKeys[keycode];
//	}
//	return isPressed;
//}
//
//bool InputHandler::isMouseButtonPressed(uint32 button) const
//{
//	bool isPressed = false;
//	if (button <= MAX_BUTTONS)
//	{
//		isPressed = wMouseButtons[button];
//	}
//	return isPressed;
//}
//
//void InputHandler::getMousePosition(double & x, double & y) const
//{
//	x = mPosX;
//	y = mPosY;
//}
////
////void InputHandler::processInput(GLFWwindow * window)
////{
////		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
////			glfwSetWindowShouldClose(window, true);
////}
////
////void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
////{
////	//wKeys[key] = action != GLFW_RELEASE;
////
////
////}
////
////void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
////{
////	Window *win = (Window*)glfwGetWindowUserPointer(window);
////	win->wMouseButtons[button] = action != GLFW_RELEASE;
////}
////
////void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
////{
////	Window *win = (Window*)glfwGetWindowUserPointer(window);
////	win->mPosX = xpos;
////	win->mPosY = ypos;
////}