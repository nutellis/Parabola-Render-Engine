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
		Root->IsMovable = false;
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

	PRenderActor* terrain = new PRenderActor("terrain");
	this->AddChild(terrain);

	terrain->SetPosition(Vector3f(0.0, 0.0, 0.0));
	terrain->SetScale(10.0f);
	terrain->AddMesh("Assets/SnowTerrain.obj");
	SortChild(terrain);

	// init scene from a previous saved one. for now just init one for testing
	//PRenderActor* triangle = new PRenderActor("triangle");
	//this->AddChild(triangle);

	//triangle->SetPosition(Vector3f(-1.0, 0.0, 0.0));
	//triangle->AddMesh("Assets/triangle.fbx");
	////pyramid->SetScale(Vector3f(0.02));
	//SortChild(triangle);

	//PRenderActor* sponza = new PRenderActor("sponza");
	//this->AddChild(sponza);

	//sponza->SetPosition(Vector3f(1.0, 0.0, 0.0));
	//sponza->SetScale(0.1f);
	//sponza->AddMesh("Assets/sponza.obj");

	//SortChild(sponza);


	PRenderActor* plane = new PRenderActor("spaceship");
	this->AddChild(plane);

	plane->SetPosition(Vector3f(150.0, 10.0, 0.0));
	plane->AddMesh("Assets/space-ship.obj");
	SortChild(plane);


	/*PRenderActor* landingpad1 = new PRenderActor("landingpad_1");
	this->AddChild(landingpad1);

	landingpad1->SetPosition(Vector3f(15.0, 14.0, -25.0));
	landingpad1->AddMesh("Assets/landingpad.obj");
	SortChild(landingpad1);*/

	PRenderActor* plane1 = new PRenderActor("spaceship_1");
	this->AddChild(plane1);

	plane1->SetPosition(Vector3f(0.0, 20.0, 0.0));
	plane1->AddMesh("Assets/space-ship.obj");
	SortChild(plane1);


	PRenderActor* plane2 = new PRenderActor("spaceship_2");
	this->AddChild(plane2);

	plane2->SetPosition(Vector3f(15.0, 25.0, -35.0));
	plane2->AddMesh("Assets/space-ship.obj");
	SortChild(plane2);

	PRenderActor* camera = new PRenderActor("camera");
	this->AddChild(camera);

	camera->SetPosition(Vector3f(-5.0f, 30.0f, 10.0f));
	// camera->SetRotation(Vector3f(0.0f, 0.0f, 0.0f));
	camera->AddCamera();
	SortChild(camera);
	

	PRenderActor* light = new PRenderActor("light");
	this->AddChild(light);

	light->SetPosition(Vector3f(0.2f, 0.99f, 0.0f));

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


