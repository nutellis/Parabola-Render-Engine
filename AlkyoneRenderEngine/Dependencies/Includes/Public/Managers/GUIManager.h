#pragma once

#include <Core/RenderCore.h>

#include <Core/SingletonBase.h>

#include <Managers/LogManager.h>


class FBORenderTarget;
class SceneNode;

class GGUIManager : SingletonManagerBase<GGUIManager>
{
public:

	static GGUIManager & getInstance();
	static GGUIManager * getInstancePtr();


	GGUIManager();
	~GGUIManager();

	void Draw();

	void DrawLog(const char* title, bool* p_open = NULL);

	// Inherited via ManagerBase
	virtual void Init() override;

	virtual void Terminate() override;

private:
	// keep a list of gui items to be rendered.

};