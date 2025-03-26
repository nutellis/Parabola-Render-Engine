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
	}
}

void Scene::SortChild(RenderActor* Child) {
	switch (Child->ActorType)
	{
	case CAMERA:
		SceneCameras.PushBack(Child);
		break;
	case LIGHT:
		SceneLights.PushBack(Child);
		break;
	case MODEL:
		SceneMeshes.PushBack(Child);
		break;
	default:
		break;
	}
}

void Scene::InitScene() {

	// init scene from a previous saved one. for now just init one for testing
	RenderActor* triangle = new RenderActor("triangle");
	//this->AddChild(triangle);

	//triangle->SetPosition(Vector3f(-1.0, 0.0, 0.0));
	//triangle->AddMesh("Assets/triangle.fbx");
	////pyramid->SetScale(Vector3f(0.02));
	//SortChild(triangle);

	RenderActor* plane = new RenderActor("plane");
	this->AddChild(plane);

	plane->SetPosition(Vector3f(1.0, 0.0, 0.0));
	plane->AddMesh("Assets/plane.fbx");
	//pyramid->SetScale(Vector3f(0.02));
	SortChild(plane);

	RenderActor* camera = new RenderActor("camera");
	this->AddChild(camera);

	camera->SetPosition(Vector3f(0.0f, 0.0f, -5.0f));
	camera->AddCamera();
	SortChild(camera);
	

	RenderActor* light = new RenderActor("light");
	this->AddChild(light);

	light->SetPosition(Vector3f(3.0, 0.5, 1.0));
	light->AddLight();
	SortChild(light);

	/*RenderActor* lightMesh = new RenderActor("lightMesh");
	this->AddChild(lightMesh);

	lightMesh->SetPosition(Vector3f(3.0, 1.5, 1.0));
	lightMesh->AddMesh("Assets/cube.obj");
	lightMesh->SetScale(Vector3f(0.3));
	SortChild(lightMesh);

	RenderActor* aMesh = new RenderActor("torusMeshActor");
	this->AddChild(aMesh);

	aMesh->SetPosition(Vector3f(-3.0, 1.5, 1.0));
	aMesh->AddMesh("Assets/torus.fbx");
	aMesh->SetScale(Vector3f(0.3));
	SortChild(aMesh);*/
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


