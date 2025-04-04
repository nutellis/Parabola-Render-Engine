#include <Components\Scene.h>
#include <Components/RenderActor.h>
#include <Components/SkyBox.h>

Scene::Scene() {
}

Scene::Scene(GSceneManager * Parent) {
	Creator = Parent;
	 
	Root = new PRenderActor("Root"); //TODO: pool allocator 
}

Scene::~Scene() {
}

PRenderActor* Scene::GetRoot() {
	return Root;
}

void Scene::AddChild(PRenderActor* Child)
{
	if(Child != nullptr) {
		Root->AddChild(Child);
	}
}

void Scene::AddToRoot(PRenderActor* Root)
{
	if (Root != nullptr) {
		this->Root = Root;
	}
}

void Scene::SortChild(PRenderActor* Child) {
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

	PSkyBox* SkyBox = new PSkyBox("SkyBox");
	this->AddToRoot(SkyBox);
	SkyBox->SetPosition(Vector3f(0.0, 0.0, 0.0));

	const int roughnesses = 8;
	TArray<std::string> filenames;
	for (int i = 0; i < roughnesses; i++)
		filenames.PushBack("Assets/envmaps/001_dl_" + std::to_string(i) + ".hdr");

	SkyBox->CreateSkyBox(
		"Assets/envmaps/001.hdr",
		"Assets/envmaps/001_irradiance.hdr", 
		filenames);


	// init scene from a previous saved one. for now just init one for testing
	PRenderActor* triangle = new PRenderActor("triangle");
	//this->AddChild(triangle);

	//triangle->SetPosition(Vector3f(-1.0, 0.0, 0.0));
	//triangle->AddMesh("Assets/triangle.fbx");
	////pyramid->SetScale(Vector3f(0.02));
	//SortChild(triangle);

	PRenderActor* plane = new PRenderActor("spaceship");
	this->AddChild(plane);

	plane->SetPosition(Vector3f(1.0, 0.0, 0.0));
	plane->AddMesh("Assets/space-ship.obj");
	SortChild(plane);

	PRenderActor* camera = new PRenderActor("camera");
	this->AddChild(camera);

	camera->SetPosition(Vector3f(-70.0f, 50.0f, 70.0f));
	camera->AddCamera();
	SortChild(camera);
	

	PRenderActor* light = new PRenderActor("light");
	this->AddChild(light);

	light->SetPosition(Vector3f(3.0, 0.5, 1.0));

	light->AddLight();
	SortChild(light);

	/*PRenderActor* lightMesh = new PRenderActor("lightMesh");
	this->AddChild(lightMesh);

	lightMesh->SetPosition(Vector3f(3.0, 1.5, 1.0));
	lightMesh->AddMesh("Assets/cube.obj");
	lightMesh->SetScale(Vector3f(0.3));
	SortChild(lightMesh);

	PRenderActor* aMesh = new PRenderActor("torusMeshActor");
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

PRenderActor* Scene::GetActiveCameraActor()
{
	PRenderActor* CameraActor = Root->Children.FindFirst([](const PRenderActor* Child) {
		return (Child->ActorType == EntityType::CAMERA && Child->Camera != nullptr && Child->Camera->IsActiveCamera == true);
		});

	/*if (CameraActor->Camera != nullptr) {
		return CameraActor->Camera;
	}*/
	return CameraActor;
}

PSkyBox* Scene::GetSkyBox()
{
	//TODO: add checks, it might fail
	return static_cast<PSkyBox * >(Root);
}


