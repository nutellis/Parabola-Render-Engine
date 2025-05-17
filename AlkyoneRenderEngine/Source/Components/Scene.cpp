#include <Components\Scene.h>
#include <Components/RenderActor.h>
#include <Components/SkyBox.h>
#include <Components/CameraComponents/Camera.h>

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

void Scene::SortCamera(PCameraActor* Child) {
	SceneCameras.PushBack(Child);
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

	//PRenderActor* sponza = new PRenderActor("sponza");
	//this->AddChild(sponza);

	//sponza->SetPosition(Vector3f(0.0, 0.0, 0.0));
	//sponza->SetRotation(Vector3f(0.0, 90.0, 0.0));
	//sponza->SetScale(0.2f);
	//sponza->AddMesh("Assets/sponza.obj");

	//SortChild(sponza);

	PRenderActor* sponza = new PRenderActor("Scene");
	this->AddChild(sponza);

	sponza->SetPosition(Vector3f(0.0, 0.0, 0.0));
	sponza->SetRotation(Vector3f(0.0, 0.0, 0.0));
	sponza->SetScale(1.0f);
	sponza->AddMesh("Assets/city test/city.obj");

	SortChild(sponza);
	


	PRenderActor* cube = new PRenderActor("Weastley");
	this->AddChild(cube);

	cube->SetPosition(Vector3f(0.0f, 17.5f, 0.0));
	cube->SetScale(10.0);
	cube->AddMesh("Assets/wheatley.obj");
	SortChild(cube);


	/*PRenderActor* landingpad1 = new PRenderActor("landingpad_1");
	this->AddChild(landingpad1);

	landingpad1->SetPosition(Vector3f(15.0, 14.0, -25.0));
	landingpad1->AddMesh("Assets/landingpad.obj");
	SortChild(landingpad1);*/
	/*for (int i = 0; i < 15; i++) {
		PRenderActor* plane1 = new PRenderActor("spaceship_" + std::to_string(i));
		this->AddChild(plane1);

		plane1->SetPosition(Vector3f(0.0, 20.0,-i * 15));
		plane1->AddMesh("Assets/space-ship.obj");
		SortChild(plane1);
	}*/

	/*PRenderActor* plane1 = new PRenderActor("spaceship_1");
	this->AddChild(plane1);

	plane1->SetPosition(Vector3f(0.0, 20.0, 0.0));
	plane1->AddMesh("Assets/space-ship.obj");
	SortChild(plane1);


	PRenderActor* plane2 = new PRenderActor("spaceship_2");
	this->AddChild(plane2);

	plane2->SetPosition(Vector3f(15.0, 25.0, -35.0));
	plane2->AddMesh("Assets/space-ship.obj");
	SortChild(plane2);*/

	PCameraActor* camera = new PCameraActor("Camera");
	this->AddChild(camera);

	camera->SetPosition(Vector3f(-300.0f, 25.0f, 10.0f));
	 camera->SetRotation(Vector3f(0.0f, 90.0f, 0.0f));
	SortCamera(camera);

	PCameraActor* secondarycamera = new PCameraActor("Free Camera");
	this->AddChild(secondarycamera);

	secondarycamera->SetPosition(Vector3f(-280.0f, 111.0f, -290.0f));
	secondarycamera->SetRotation(Vector3f(-25.0f, 170.0f, 0.0f));
	SortCamera(secondarycamera);
	this->SetActiveCamera(camera);

	PRenderActor* light = new PRenderActor("light");
	this->AddChild(light);

	light->SetPosition(Vector3f(0.0f, 0.0f, 0.0f));

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

PCameraActor* Scene::GetActiveCamera()
{
	for (PCameraActor* CameraActor : SceneCameras) {
		if (CameraActor->Camera->IsActiveCamera == true) {
			return CameraActor;
		}
	}
}

PSkyBox* Scene::GetSkyBox()
{
	//TODO: add checks, it might fail
	return static_cast<PSkyBox * >(Root);
}

void Scene::SetActiveCamera(PCameraActor* Camera)
{
	for (PCameraActor* CameraActor : SceneCameras) {
		if (CameraActor == Camera) {
			CameraActor->Camera->IsActiveCamera = true;
		} else {
			CameraActor->Camera->IsActiveCamera = false;
		}
	}
}


