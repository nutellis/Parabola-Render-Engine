#include <Components\Scene.h>
#include <Components/RenderActor.h>
#include <Components/SkyBox.h>
#include <Components/CameraComponents/Camera.h>
#include <Components/Colliders/IntersectionTests.h>
#include <Components/StaticMesh.h>
#include <Components/RenderComponents/StaticMeshGroup.h>
#include <Components/Colliders/AxisAlignedBoundingBox.h>
#include <Components/LightComponents/DirectionalLightComponent.h>

Scene::Scene() {
}

Scene::Scene(GSceneManager * Parent) {
	Creator = Parent;
	 
	Root = new RRenderActor("Root"); //TODO: pool allocator 
}

Scene::~Scene() {
}

RRenderActor* Scene::GetRoot() {
	return Root;
}

void Scene::AddChild(RRenderActor* Child)
{
	if(Child != nullptr) {
		Root->AddChild(Child);
	}
}

void Scene::AddToRoot(RRenderActor* Root)
{
	if (Root != nullptr) {
		this->Root = Root;
		Root->IsMovable = false;
	}
}

void Scene::SortChild(RRenderActor* Child) {
	switch (Child->ActorType)
	{
	case LIGHT:
		SceneLights.PushBack(Child);
		break;
	case MODEL:
		SceneActors.PushBack(Child);
		break;
	default:
		break;
	}
}

void Scene::SortCamera(PCameraActor* Child) {
	SceneCameras.PushBack(Child);
}

void Scene::InitScene() {

	RSkyBoxActor* SkyBox = new RSkyBoxActor("SkyBox");
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

	RRenderActor* sponza = new RRenderActor("sponza");
	this->AddChild(sponza);
	sponza->AddMesh("Assets/sponza/sponza.obj");
	sponza->SetPosition(Vector3f(0.0, 0.0, 0.0));
	sponza->SetRotation(Vector3f(0.0, 90.0, 0.0));
	sponza->SetScale(1.0f);


	//RRenderActor* sponza = new RRenderActor("Scene");
	//this->AddChild(sponza);
	//
	//sponza->AddMesh("Assets/city test/city3.obj");
	//
	//sponza->SetPosition(Vector3f(0.0, 0.0, 0.0));
	//sponza->SetRotation(Vector3f(0.0, 0.0, 0.0));
	//sponza->SetScale(1.0f);

	//RegisterMesh(sponza->StaticMeshGroup);
	SortChild(sponza);


	RRenderActor* city = new RRenderActor("Scene2");
	this->AddChild(city);

	city->AddMesh("Assets/city test/city3.obj");

	city->SetPosition(Vector3f(32.0, 0.0, 0.0));
	city->SetRotation(Vector3f(0.0, 90.0, 0.0));
	city->SetScale(1.0f);

	//RegisterMesh(sponza->StaticMeshGroup);
	SortChild(city);
	
	/*RRenderActor* landingpad1 = new RRenderActor("landingpad_1");
	this->AddChild(landingpad1);

	landingpad1->SetPosition(Vector3f(15.0, 14.0, -25.0));
	landingpad1->AddMesh("Assets/landingpad.obj");
	SortChild(landingpad1);*/
	/*for (int i = 0; i < 15; i++) {
		RRenderActor* plane1 = new RRenderActor("spaceship_" + std::to_string(i));
		this->AddChild(plane1);

		plane1->SetPosition(Vector3f(0.0, 20.0,-i * 15));
		plane1->AddMesh("Assets/space-ship.obj");
		SortChild(plane1);
	}*/

	/*RRenderActor* plane1 = new RRenderActor("spaceship_1");
	this->AddChild(plane1);

	plane1->SetPosition(Vector3f(0.0, 20.0, 0.0));
	plane1->AddMesh("Assets/space-ship.obj");
	SortChild(plane1);


	RRenderActor* plane2 = new RRenderActor("spaceship_2");
	this->AddChild(plane2);

	plane2->SetPosition(Vector3f(15.0, 25.0, -35.0));
	plane2->AddMesh("Assets/space-ship.obj");
	SortChild(plane2);*/

	PCameraActor* camera = new PCameraActor("Camera");
	this->AddChild(camera);

	camera->SetPosition(Vector3f(0.f, 0.f, -7.0f));// 58.280, 65.168, -208.757));
	camera->SetRotation(Vector3f::ZERO);// -50.580, 339.361, 0.0f));
	camera->InitCamera();
	SortCamera(camera);

	PCameraActor* secondarycamera = new PCameraActor("Free Camera");
	this->AddChild(secondarycamera);

	secondarycamera->SetPosition(Vector3f(-15.0f, 2.0f, 0.0f));
	secondarycamera->SetRotation(Vector3f(-11.0f, 42.0f, 0.0f));
	camera->InitCamera();
	SortCamera(secondarycamera);
	this->SetActiveCamera(camera);

	RRenderActor* light = new RRenderActor("light");
	this->AddChild(light);

	light->SetPosition(Vector3f(0.0f, 0.0f, 0.0f));

	light->AddLight();
	SortChild(light);

	/*RRenderActor* lightMesh = new RRenderActor("lightMesh");
	this->AddChild(lightMesh);

	lightMesh->SetPosition(Vector3f(3.0, 1.5, 1.0));
	lightMesh->AddMesh("Assets/cube.obj");
	lightMesh->SetScale(Vector3f(0.3));
	SortChild(lightMesh);

	RRenderActor* aMesh = new RRenderActor("torusMeshActor");
	this->AddChild(aMesh);

	aMesh->SetPosition(Vector3f(-3.0, 1.5, 1.0));
	aMesh->AddMesh("Assets/torus.fbx");
	aMesh->SetScale(Vector3f(0.3));
	SortChild(aMesh);*/
}


void Scene::SortScene() {
	//// sort scene objects with transparent objects last
	//for (RRenderActor* Actor : SceneActors) {
	//	if (Actor->HasTransparency == true) {
	//		for (int i = SceneActors.Size() - 1; i >= 0; i--) {
	//			if (SceneActors[i]->HasTransparency == false) {
	//				Utilities::Swap(Actor, SceneActors[i]);
	//				break;
	//			}
	//		}
	//	}
	//}

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

RSkyBoxActor* Scene::GetSkyBox()
{
	//TODO: add checks, it might fail
	return static_cast<RSkyBoxActor * >(Root);
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



TArray<PAxisAlignedBoundingBox> Scene::GetObjectsByIntersection(PAxisAlignedBoundingBox * BoxToCheck)
{
	TArray<PAxisAlignedBoundingBox> ObjectsToReturn = TArray<PAxisAlignedBoundingBox>(100) ;
	for (RRenderActor * Actor : SceneActors) {
		TArray<RStaticMesh*> Meshes = Actor->StaticMeshGroup->Meshes;
		for (RStaticMesh * Mesh : Meshes) {
			if (IntersectionTest(Mesh->WorldBoundingBox, BoxToCheck)) {
				ObjectsToReturn.PushBack(*Mesh->WorldBoundingBox);
				//Mesh->WorldBoundingBox->DebugColour = Vector3f(0.0, 1.0, 0.0);
			}
			else {
				//Mesh->WorldBoundingBox->DebugColour = Vector3f(0.0, 0.0, 1.0);
			}
		}
	}
	return ObjectsToReturn;
}

TArray<PAxisAlignedBoundingBox> Scene::GetShadowCasters(PAxisAlignedBoundingBox* BoxToCheck,Vector3f Colour)
{
	TArray<PAxisAlignedBoundingBox> ObjectsToReturn = TArray<PAxisAlignedBoundingBox>(SceneActors.Size());
	for (RRenderActor* Actor : SceneActors) {
		TArray<RStaticMesh*> Meshes = Actor->StaticMeshGroup->Meshes;
		for (RStaticMesh * Mesh : Meshes) {
			if (Mesh->IsCastingShadows && SweepIntersectionTest(Mesh->WorldBoundingBox, BoxToCheck, SceneLights.Front()->Light->LightDirection)) {
				//if (Mesh->LocalBoundingBox->Max.Y - Mesh->LocalBoundingBox->Min.Y > 0.8) {
					ObjectsToReturn.PushBack(*Mesh->WorldBoundingBox);
					Mesh->WorldBoundingBox->DebugColour = Colour;
				//}
			}
			else {
				//Mesh->WorldBoundingBox->DebugColour = Vector3f(1.0, 1.0, 0.0);
			}
		}
	}
	return ObjectsToReturn;
}


//Muff_Mesh.053