#pragma once

#include <Utilities/Containers/Array.h>

class PRenderActor;
class GSceneManager;
class PCameraComponent;
class PSkyBox;

// Contains a scenegraph of Actors. Nothing else

class Scene 
{

public:

	Scene();
	Scene(GSceneManager* Parent);
	~Scene();

	PRenderActor* GetRoot();

	void AddChild(PRenderActor* Child);

	void AddToRoot(PRenderActor* Root);

	void SortChild(PRenderActor* Child);

	void InitScene();

	void ResetScene();

	PCameraComponent* GetActiveCameraActor();
	PSkyBox* GetSkyBox();


	//cull nodes

	
	//update scene


	//render scene

public:
	TArray<PRenderActor *> SceneMeshes;
	TArray<PRenderActor *> SceneLights;
	TArray<PRenderActor *> SceneCameras;
private:
	// SceneManager which created this node
	GSceneManager* Creator;

	PRenderActor* Root;
	bool isActive;
};