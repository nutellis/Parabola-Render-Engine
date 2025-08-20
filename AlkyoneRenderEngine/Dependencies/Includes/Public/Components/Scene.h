#pragma once

#include <Utilities/Containers/Array.h>
#include <ParabolaMath.h>

class RRenderActor;
class PCameraActor;
class GSceneManager;
class PCameraComponent;
class RSkyBoxActor;
class PAxisAlignedBoundingBox;

// Contains a scenegraph of Actors. Nothing else

class Scene 
{

public:

	Scene();
	Scene(GSceneManager* Parent);
	~Scene();

	RRenderActor* GetRoot();

	void AddChild(RRenderActor* Child);

	void AddToRoot(RRenderActor* Root);

	void SortChild(RRenderActor* Child);

	void SortCamera(PCameraActor* Child);

	void InitScene();

	void SortScene();

	void ResetScene();

	void RegisterMeshes(RStaticMeshGroup * NewMeshGroup);

	PCameraActor* GetActiveCamera();
	RSkyBoxActor* GetSkyBox();

	void SetActiveCamera(PCameraActor* Camera);

	TArray<PAxisAlignedBoundingBox> GetObjectsByIntersection(PAxisAlignedBoundingBox* BoxToCheck);

	TArray<PAxisAlignedBoundingBox> GetShadowCasters(PAxisAlignedBoundingBox* BoxToCheck, Vector3f Colour);

	//cull nodes

	
	//update scene


	//render scene

public:
	TArray<RStaticMesh *> SceneMeshes;
	TArray<RRenderActor *> SceneLights;
	TArray<PCameraActor *> SceneCameras;
private:
	// SceneManager which created this node
	GSceneManager* Creator;

	RRenderActor* Root;
	bool isActive;

	uint32 SceneMeshesTotal = 0;
};