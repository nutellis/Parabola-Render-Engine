//#pragma once
//#ifndef INPUT_H
//#define INPUT_H
//
////#include <Window.h>
//
//#include <map>
//#include <vector>
//#include <functional>
//
//#include <Actor.h>
//
//#define MAX_KEYS	512
//#define MAX_BUTTONS	16
//
//
//
//using Callback = std::function<void()>;
//
//class InputHandler
//{
//public:
//	InputHandler();
//	~InputHandler();
//
//
//	bool isKeyPressed(uint32 keycode) const;
//	bool isMouseButtonPressed(uint32 button) const;
//	void getMousePosition(double &x, double &y) const;
//
//	//void processInput(GLFWwindow *window);
//
//	void addBinding(int key, const Callback& callback);
//
//
//	void onKeyPress(int key);
//
//	Command* InputHandler::handleInput()
//	{
//		Command *cmd = NULL;
//		/*if (isKeyPressed(BUTTON_X)) return buttonX_;
//		if (isPressed(BUTTON_Y)) return buttonY_;
//		if (isPressed(BUTTON_A)) return buttonA_;
//		if (isPressed(BUTTON_B)) return buttonB_;*/
//
//		// Nothing pressed, so do nothing.
//		return cmd;
//	}
//
//private:
//
//	//Window *activeWin;
//
//	std::map<int, std::vector<Callback>> mCallbacks;
//
//	bool wKeys[MAX_KEYS];
//	bool wMouseButtons[MAX_BUTTONS];
//	double mPosX, mPosY;
//
//
//private:
//
//	/*friend static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
//	friend static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
//	friend static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
//	*/
//
//
//};
//
//class Command 
//{
//
//public:
//	virtual ~Command();
//	virtual void execute(Actor *actor) = 0;
//};
//
//
//
//
//
//
//
//
//
//InputHandler::InputHandler()
//{
//}
//
//InputHandler::~InputHandler()
//{
//}
//
//
//#endif // !INPUT_H
//
//
//
//
