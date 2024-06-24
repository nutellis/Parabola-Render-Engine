#pragma once

#include <Core/RenderCore.h>

#include <Core/SingletonBase.h>
#include <Managers/ManagerBase.h>

#include <Managers/LogManager.h>


class FBORenderTarget;
class SceneNode;

class GGUIManager : SingletonBase<GGUIManager>, ManagerBase
{
public:

	static GGUIManager & getInstance();
	static GGUIManager * getInstancePtr();


	GGUIManager();
	~GGUIManager();

	void Draw();

	void DrawLog(const char* title, bool* p_open = NULL);

	void DrawPreview();

	void DrawSceneGraph();

	/*void DrawPreview(FBORenderTarget* RenderTarget);*/

	void BindRenderTarget();

	// Inherited via ManagerBase
	virtual void Init() override;

	virtual void Terminate() override;

private:
	SceneNode* RecurseSceneChildren(SceneNode* Root);

private:
	// keep a list of gui items to be rendered.

	//TODO: delete temp var
	FBORenderTarget* RenderTarget;
};