#pragma once
#ifndef WINDOW_H
#define WINDOW_H


#include <iostream>
#include <vector>

#include <fstream>

#include <Core/SingletonBase.h>
#include <Managers/ManagerBase.h>


#define MAX_KEYS	512
#define MAX_BUTTONS	16

struct GLFWwindow;

class RenderWindow;


//create a list for multiple RenderWindows.
//Manages windows.
class GWindowManager : SingletonBase<GWindowManager>, ManagerBase
{

public:

	//GWindowManager(GWindowManager const&) = delete;
	//void operator=(GWindowManager const&) = delete;



	static GWindowManager & getInstance();

	static GWindowManager * getInstancePtr();


	GWindowManager();
	//GWindowManager(const WindowInfo & Winfo, const ContextInfo & Cinfo);
	//GWindowManager(int width,int height,const char* title);
	~GWindowManager();

	RenderWindow * Window;

	virtual void Init() override;

	virtual void Terminate() override;


	//inline void SetWindowInfo(WindowInfo newInfo) { wInfo = newInfo; }
	
	RenderWindow * GetRenderWindow();
	GLFWwindow * GetWindow();

	//inline uint32 GetWidth() const { return wInfo.Width; }
	//inline uint32 GetHeight() const { return wInfo.Height; }
//	inline const char* GetTitle() const { return wInfo.Title; }

	
};

#endif // !WINDOW_H

//
////OpenGL versions
//struct ContextInfo
//{
//	int major_version, minor_version;
//	bool core;
//
//	ContextInfo()//default
//	{
//		major_version = 3;
//		minor_version = 3;
//		core = true;
//	}
//
//	ContextInfo(int major_version, int minor_version, bool core)
//	{
//		this->major_version = major_version;
//		this->minor_version = minor_version;
//		this->core = core;
//	}
//
//	//just like windowInfo.h
//	//implement copy constructor
//	//implement assignment operator
//	//these implementations are optional because are special functions
//  //http://en.wikipedia.org/wiki/Special_member_functions
//};
//
//struct FramebufferInfo {
//
//	uint32 flags;
//	bool msaa;//to enable or disable it when wee need it
//
//	FramebufferInfo()
//	{
//		//default
//		flags = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH;
//		msaa = false;
//	}
//
//	FramebufferInfo(bool color, bool depth, bool stencil, bool msaa)
//	{
//		flags = GLUT_DOUBLE; //this is a must
//		if (color)
//			flags |= GLUT_RGBA | GLUT_ALPHA;
//		if (depth)
//			flags |= GLUT_DEPTH;
//		if (stencil)
//			flags |= GLUT_STENCIL;
//		if (msaa)
//			flags |= GLUT_MULTISAMPLE;
//		this->msaa = msaa;
//	}
//
//	//(optional)implement copy constructor and assignment operator
//};

