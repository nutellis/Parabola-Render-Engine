#pragma once

#include <Utilities/Containers/Array.h>

class RenderActor;
class GSceneManager;
class PCameraComponent;

// Contains a scenegraph of Actors. Nothing else

class Scene 
{

public:

	Scene();
	Scene(GSceneManager* Parent);
	~Scene();

	RenderActor* GetRoot();

	void AddChild(RenderActor* Child);

	void SortChild(RenderActor* Child);

	void InitScene();

	void ResetScene();

	RenderActor* GetActiveCameraActor();


	//cull nodes

	
	//update scene


	//render scene

public:
	TArray<RenderActor *> SceneMeshes;
	TArray<RenderActor *> SceneLights;
	TArray<RenderActor *> SceneCameras;
private:
	// SceneManager which created this node
	GSceneManager* Creator;

	RenderActor* Root;
	bool isActive;
};