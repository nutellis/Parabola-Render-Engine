#include <Components\Scene.h>
#include <Components/RenderActor.h>

Scene::Scene() {
}

Scene::Scene(GSceneManager * Parent) {
	Creator = Parent;
	 
	Root = new RenderActor("Root"); //TODO: pool allocator 
}

Scene::~Scene() {
}

RenderActor* Scene::GetRoot() {
	return Root;
}

void Scene::AddChild(RenderActor* Child)
{
	if(Child != nullptr) {
		Root->AddChild(Child);
		SortChild(Child);
	}
}

void Scene::SortChild(RenderActor* Child) {
	switch (Child->ActorType)
	{
	case CAMERA:
		SceneCameras.PushBack(*Child);
		break;
	case LIGHT:
		SceneLights.PushBack(*Child);
		break;
	case MODEL:
		SceneMeshes.PushBack(*Child);
		break;
	default:
		break;
	}
}

void Scene::InitScene() {

	// init scene from a previous saved one. for now just init one for testing
	RenderActor* pyramid = new RenderActor("pyramid");
	this->AddChild(pyramid);

	pyramid->SetPosition(Vector3f(0.0, 0.0, 0.0));
	pyramid->AddMesh("Assets/tri.fbx");

	RenderActor* camera = new RenderActor("camera");
	this->AddChild(camera);

	camera->SetPosition(Vector3f(-1.0f, 2.0f, 5.0f));
	camera->AddCamera();
	

	//RenderActor* light = new RenderActor("light");
	//this->AddChild(light);
	//light->AddLight();
	//light->SetPosition(Vector3f(3.0, 0.5, 1.0));

	RenderActor* lightMesh = new RenderActor("lightMesh");
	this->AddChild(lightMesh);

	lightMesh->SetPosition(Vector3f(3.0, 1.5, 1.0));
	lightMesh->AddMesh("Assets/cube.obj");
	lightMesh->SetScale(Vector3f(0.3));


	RenderActor* aMesh = new RenderActor("torusMeshActor");
	this->AddChild(aMesh);

	aMesh->SetPosition(Vector3f(-3.0, 1.5, 1.0));
	aMesh->AddMesh("Assets/torus.fbx");
	aMesh->SetScale(Vector3f(0.3));

	//RenderActor* cube = new RenderActor("cube");
	//this->AddChild(cube);
	//cube->SetPosition(Vector3f(1.0, 2.0, 1.0));

	//cube->AddMesh("Assets/cubetest.fbx");

	
}

void Scene::ResetScene() {
	// Root->RemoveAllChildren();
	Root = nullptr;
}

RenderActor* Scene::GetActiveCameraActor()
{
	RenderActor* CameraActor = Root->Children.FindFirst([](const RenderActor* Child) {
		return (Child->ActorType == EntityType::CAMERA && Child->Camera != nullptr && Child->Camera->IsActiveCamera == true);
		});

	/*if (CameraActor->Camera != nullptr) {
		return CameraActor->Camera;
	}*/
	return CameraActor;
}


