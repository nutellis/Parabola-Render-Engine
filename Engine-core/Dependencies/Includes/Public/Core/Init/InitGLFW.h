#pragma once

#include <Core/Init/InitGlew.h>


struct WindowInfo;
struct ContextInfo;
struct FramebufferInfo;

class InitGLFW 
{

public:
	static void Init(const WindowInfo &window, const ContextInfo &context, const FramebufferInfo &framebuffer);

	static void Run();
	static void Close();

	void EnterFullscreen();
	void ExitFullscreen();

	static void PrintOpenGLinfo(const WindowInfo& windowInfo,
		const ContextInfo& context);
private:
	
	static void IdleCallback(void);
	static void DisplayCallback(void);
	static void ReshapeCallback(int width, int height);
	static void CloseCallback();



};