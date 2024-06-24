#pragma once

#include <Core/RenderCore.h>

struct GLFWwindow;
struct GLFWmonitor;

class RenderWindow
{
	const char *Name;
	uint32 Width, Height;
	int32 PosX, PosY;

	size_t mFrameCount;

	uint8 bIsReshapable : 1;
	uint8 bIsActive : 1;

public:
	GLFWwindow * Window;
	GLFWmonitor * PrimaryMonitor;

	//uint mFSAA;
	//String mFSAAHint;

private:
	uint8 Primary : 1;

	uint8 VSync : 1;

	void setPrimary() { Primary = true; }

	//friend class Engine;

public:
	RenderWindow();

	~RenderWindow();

	RenderWindow(const char * newName, uint32 newWidth, uint32 newHeight, uint8 FullScreen);

	//	Indicates whether the window is the primary window.
	uint8 IsPrimary(void) const;

	//Overridden from RenderTarget, flags invisible windows as inactive.
	uint8 IsActive(void) const;
	
	//Indicates whether the window has been closed by the user.
	uint8 IsClosed(void) const;
	
	//	Returns true if the window will automatically de - activate itself when it loses focus.
	uint8 IsDeactivatedOnFocusChange() const;
	
	//	Returns true if window is running in fullscreen mode.
	uint8 IsFullScreen(void) const;

	//	Indicates whether the window is visible(not minimized or obscured) More...
	uint8 IsVisible(void) const;

	//	Indicates whether the window was set to hidden(not displayed) More...
	uint8 IsHidden(void) const;

	uint8 IsVSyncEnabled() const;

	void Create(const char * newName, uint32 newWidth, uint32 newHeight, uint8 FullScreen);// , const NameValuePairList *miscParams);
	void Destroy(void);

	// Indicates whether multisampling is performed on rendering and at what level.
	uint32 GetFSAA() const;

	//Gets the FSAA hint
	const char & GetFSAAHint() const;

	float GetLastFPS() const;
	float GetAverageFPS() const;


	uint32 GetHeight(void) const;
	uint32 GetWidth(void) const;
	const char * getName(void) const;

	void getMetrics(unsigned int & width, unsigned int & height, unsigned int & colourDepth);

	//-------------------------------------------------------------------------------------
	//
	//								  WINDOW CALLBACKS
	//
	//-------------------------------------------------------------------------------------

	//TO DO CALL BACKS
	static void ErrorCallback(int ErrorCode, const char* Description);

	static void OnResizeCallback(GLFWwindow* window, int width, int height);

	static void OnCloseCallback(GLFWwindow* window);

	//static void KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	
	//Window size functions
	static void ResizeWindow(int Width, int Height);


public:
	float DeltaX;
	float DeltaY;
	float DeltaZ;


	float MouseLastX;
	float MouseLastY;
	bool FirstMouse;


};