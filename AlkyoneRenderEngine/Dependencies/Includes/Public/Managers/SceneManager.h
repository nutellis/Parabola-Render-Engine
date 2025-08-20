#pragma once
#ifndef SCENE_H
#define SCENE_H

// Is responsible for Culling and Rendering

#include <Core/RenderCore.h>

#include <Core/SingletonBase.h>

class Scene;
class RRenderActor;
class SceneNode;
class RStaticMesh;


class GSceneManager : SingletonManagerBase<GSceneManager>
{
public:
	static GSceneManager& getInstance();
	static GSceneManager * getInstancePtr();
public:

	GSceneManager();
	~GSceneManager();


	Scene * GetActiveScene();

	//const RRenderActor* GetActiveSceneElements();

	//uint8 AddStaticMesh();

	// Inherited via ManagerBase
	virtual void Init() override;
	virtual void Terminate() override;

	void DrawSceneGraph();

	void DrawScene();

	void FrustrumCull();

	RRenderActor* RecurseSceneChildren(RRenderActor* Root);

	RStaticMesh* RecurseActorsMeshes(RRenderActor* Root);

private:

	Scene * ActiveScene;

};


#endif // !SCENE_H
