#pragma once
#ifndef SCENE_H
#define SCENE_H

// Is responsible for Culling and Rendering

#include <Core/RenderCore.h>

#include <Core/SingletonBase.h>
#include <Managers/ManagerBase.h>

class Scene;
class RenderActor;
class SceneNode;


class GSceneManager : SingletonBase<GSceneManager>, ManagerBase
{
public:
	static GSceneManager& getInstance();
	static GSceneManager * getInstancePtr();
public:

	GSceneManager();
	~GSceneManager();


	Scene * GetActiveScene();

	//const RenderActor* GetActiveSceneElements();

	//uint8 AddStaticMesh();

	// Inherited via ManagerBase
	virtual void Init() override;
	virtual void Terminate() override;

	void DrawSceneGraph();

	void FrustrumCull();

	RenderActor* RecurseSceneChildren(RenderActor* Root);

private:

	Scene * ActiveScene;

};


#endif // !SCENE_H
