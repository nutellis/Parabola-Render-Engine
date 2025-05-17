#pragma once

#include <Utilities/Containers/Array.h>

class PRenderActor;
class PCameraActor;
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

	void SortCamera(PCameraActor* Child);

	void InitScene();

	void ResetScene();

	PCameraActor* GetActiveCamera();
	PSkyBox* GetSkyBox();

	void SetActiveCamera(PCameraActor* Camera);


	//cull nodes

	
	//update scene


	//render scene

public:
	TArray<PRenderActor *> SceneMeshes;
	TArray<PRenderActor *> SceneLights;
	TArray<PCameraActor *> SceneCameras;
private:
	// SceneManager which created this node
	GSceneManager* Creator;

	PRenderActor* Root;
	bool isActive;
};