#include <Managers/SceneManager.h>

#include <Managers/LogManager.h>

#include <Components/Scene.h> 
#include <Components/RenderActor.h>
#include <Components/LightComponents/DirectionalLightComponent.h>
#include <Components/CameraComponents/Camera.h>
#include <Components/RenderComponents/StaticMeshGroup.h>
#include <Components/StaticMesh.h>
#include <Components/Colliders/AxisAlignedBoundingBox.h>

template<> GSceneManager* SingletonManagerBase<GSceneManager>::instance = 0;
GSceneManager & GSceneManager::getInstance()
{
	//TODO :assert?
	return (*instance);
}

GSceneManager * GSceneManager::getInstancePtr()
{
	if (instance == nullptr)
	{
		instance = new GSceneManager();
	}
	return instance;
}

GSceneManager::GSceneManager()
{
	ActiveScene = new Scene(this);
}

GSceneManager::~GSceneManager()
{}

Scene* GSceneManager::GetActiveScene()
{
	if (ActiveScene != nullptr) {
		return ActiveScene;
	}
	return nullptr;
}


//const TArray<RRenderActor> GSceneManager::GetActiveSceneElements() {
//	
//
//	RRenderActor* Root = ActiveScene->GetRoot();
//
//	for (auto i = 0; i < Root->Children.Size(); i++) {
//		
//	}
//
//	return nullptr;
//}



void GSceneManager::Init()
{
	LOG(DEBUG, "Initiating SCENE_MANAGER\n");


	ActiveScene->InitScene();

	ActiveScene->SortScene();

	//Root = new Octree(NULL);
	//MaxDepth = 6;

	//LOG(DEBUG, "Min:%f \n Max:%f",Root->Box.GetMinimum(), Root->Box.GetMaximum());


	//OctreeNode * temp = new OctreeNode();

	//LOG(DEBUG, "Min:%f \n Max:%f", temp->GetBoundingBox().GetMinimum(), temp->GetBoundingBox().GetMaximum());

	//AddOctreeNode(temp, Root);


	//OctreeNode * temp2 = new OctreeNode();

	//temp2->GetBoundingBox().SetExtents(Point(1.0f, 1.0f, 1.0f), Point(2.0f, 2.0f, 2.0f));

	//LOG(DEBUG, "Min:%f \n Max:%f", temp2->GetBoundingBox().GetMinimum(), temp2->GetBoundingBox().GetMaximum());

	//AddOctreeNode(temp2, Root);


	//OctreeNode * temp3 = new OctreeNode();

	//temp3->GetBoundingBox().SetExtents(Point(1.0f, 1.0f, 1.0f), Point(9.0f, 9.0f,9.0f));

	//LOG(DEBUG, "Min:%f \n Max:%f", temp3->GetBoundingBox().GetMinimum(), temp3->GetBoundingBox().GetMaximum());

	//AddOctreeNode(temp3, Root);


	LOG(DEBUG, "SCENE_MANAGER INITIATED!!!\n");
}

void GSceneManager::Terminate()
{
}

void GSceneManager::DrawSceneGraph()
{
	ImGui::SetNextWindowSize(ImVec2(600, 1079));
	ImGui::SetNextWindowPos(ImVec2(1440, 0));
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoScrollbar;

	static RRenderActor* selectedIndex = nullptr;
	static RStaticMesh* selectedMesh = nullptr;
	static bool useUniformScaling = true;
	static bool useRelativeTranslation = true;
	ImGui::Begin("SceneGraph", 0, window_flags);
	{
		Scene* ActiveScene = GetActiveScene();
		if (ActiveScene != nullptr) {
			RRenderActor* Root = ActiveScene->GetRoot();
			ImGui::BeginChild("Graph", ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);

			RRenderActor* ReturnedValue = RecurseSceneChildren(Root);
			if (ReturnedValue != nullptr) {
				selectedIndex = ReturnedValue;
			}
			ImGui::EndChild();

			// Selected Details
			if (selectedIndex != nullptr) {
				ImGui::BeginGroup();
				ImGui::BeginChild("Node Details", ImVec2(ImGui::GetContentRegionAvail().x, 250), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 line below us
				ImGui::Text("Object: %s", selectedIndex->ObjectName.c_str());
				ImGui::Separator();
				if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
				{
					if (ImGui::BeginTabItem("Details"))
					{
						//if (useRelativeTranslation) {
						Vector3f LocalTranslation = selectedIndex->ObjectPosition;
						if (ImGui::InputFloat3("Translation", &LocalTranslation[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)) {
							selectedIndex->SetPosition(LocalTranslation);
						}
						/*}
						ImGui::SameLine();
						ImGui::Checkbox("Relative Translation", &useRelativeTranslation);*/
						Vector3f LocalRotation = selectedIndex->ObjectRotation;
						if (ImGui::InputFloat3("Rotation", &LocalRotation[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)) {
							selectedIndex->SetRotation(LocalRotation);
						}
						if (useUniformScaling) {
							if (ImGui::InputFloat("Scale", &selectedIndex->ObjectScale.X, 1.0, 1.0, "%.1f")) {
								selectedIndex->SetScale(selectedIndex->ObjectScale.X);
							}
						}
						else {
							if (ImGui::InputFloat3("Scale", &selectedIndex->ObjectScale.X)) {
								selectedIndex->SetScale(selectedIndex->ObjectScale);
							}
						}
						ImGui::SameLine();
						ImGui::Checkbox("Uniform Scaling", &useUniformScaling);
						if (selectedIndex->ActorType == EntityType::LIGHT) {
							if (ImGui::InputFloat3("Light Direction", &selectedIndex->Light->LightDirection[0], "%.3f")) {
								selectedIndex->Light->SetDirection();
							}
							if (ImGui::SliderFloat("Azimuth", &selectedIndex->Light->Azimuth, 0.00f, 360.0f, "%.1f")) {
								selectedIndex->Light->SetDirection();
							}
							if (ImGui::SliderFloat("Zenith", &selectedIndex->Light->Zenith, 0.10f, 120.0f, "%.1f")) {
								selectedIndex->Light->SetDirection();
							}
							ImGui::SliderFloat("Light Distance", &selectedIndex->Light->LightDistance, 15.0f, 120.0f, "%.1f");

						}
						if (selectedIndex->ActorType == EntityType::CAMERA) {
							PCameraActor* Camera = static_cast<PCameraActor*>(selectedIndex);
							if (Camera != nullptr) {
								ImGui::Separator();
								if (ImGui::InputFloat("Camera Speed", &Camera->Camera->MovementSpeed, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue)) {
								}
								// Make those one line and fit them
								if (ImGui::InputFloat("Near Plane", &Camera->Camera->Frustrum->NearPlane, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue)) {
								}
								if (ImGui::InputFloat("Far Plane", &Camera->Camera->Frustrum->FarPlane, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue)) {
								}
								if (ImGui::InputFloat("Field of View", &Camera->Camera->Frustrum->FieldOfView, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue)) {
								}

								if (ImGui::Checkbox("Set Active Camera", &Camera->Camera->IsActiveCamera)) {
									ActiveScene->SetActiveCamera(Camera);
								}
							}
						}
						
						ImGui::EndTabItem();
					}
					if (selectedIndex->ActorType == MODEL && selectedIndex->StaticMeshGroup != nullptr) {
						if (ImGui::BeginTabItem("Meshes"))
						{
							RStaticMesh* ReturnedValue = RecurseActorsMeshes(selectedIndex);
							if (ReturnedValue != nullptr) {
								selectedMesh = ReturnedValue;
							}
							ImGui::EndTabItem();
						}
					}
					ImGui::EndTabBar();
				}
				ImGui::EndChild();
				/*	if (ImGui::Button("Revert")) {}
					ImGui::SameLine();
					if (ImGui::Button("Save")) {}*/
				ImGui::EndGroup();
			}
			if (selectedMesh != nullptr) {
				ImGui::BeginGroup();
				ImGui::BeginChild("Mesh Details", ImVec2(ImGui::GetContentRegionAvail().x, 500), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 line below us
				ImGui::Text("Mesh: %s", selectedMesh->Name.c_str());
				ImGui::Separator();
				if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
				{
					if (ImGui::BeginTabItem("Details"))
					{
						ImGui::Text("Mesh: %d", selectedMesh->NumVertices);
						ImGui::Checkbox("Cast Shadows", &selectedMesh->IsCastingShadows);
						ImGui::Text("AABB Min: %f %f %f",
							selectedMesh->WorldBoundingBox->Min.X,
							selectedMesh->WorldBoundingBox->Min.Y,
							selectedMesh->WorldBoundingBox->Min.Z
						);
						ImGui::Text("AABB Max: %f %f %f",
							selectedMesh->WorldBoundingBox->Max.X,
							selectedMesh->WorldBoundingBox->Max.Y,
							selectedMesh->WorldBoundingBox->Max.Z
						);
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
				ImGui::EndChild();
				ImGui::EndGroup();
			}
		}
	}
	ImGui::End();
}


RRenderActor* GSceneManager::RecurseSceneChildren(RRenderActor* Root) {
	RRenderActor* SelectedNode = nullptr;
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (ImGui::TreeNodeEx((void*)(intptr_t)0, node_flags | ImGuiTreeNodeFlags_DefaultOpen, Root->ObjectName.c_str()))
	{
		for (int i = 0; i < Root->Children.Size(); i++)
		{
			if (Root->Children[i]->Children.IsNotEmpty()) {
				RRenderActor* ReturnedValue = RecurseSceneChildren(Root->Children[i]);
				if (ReturnedValue != nullptr) {
					SelectedNode = ReturnedValue;
				}
			}
			else {
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

				if (ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, Root->Children[i]->ObjectName.c_str()))
				{
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
						// callback for selection
						SelectedNode = Root->Children[i];
					}
				}
			}
		}
		ImGui::TreePop();
	}

	return SelectedNode;
}

RStaticMesh* GSceneManager::RecurseActorsMeshes(RRenderActor* Root) {
	RStaticMesh* SelectedNode = nullptr;

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (ImGui::TreeNodeEx((void*)(intptr_t)0, node_flags | ImGuiTreeNodeFlags_DefaultOpen, Root->ObjectName.c_str()))
	{
		TArray<RStaticMesh*> Meshes = Root->StaticMeshGroup->Meshes;
		for (int i = 0; i < Meshes.Size(); i++)
		{
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			if (ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, Meshes[i]->Name.c_str()))
			{
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
					// callback for selection
					SelectedNode = Meshes[i];
				}
			}
		}
		ImGui::TreePop();
	}

	return SelectedNode;
}

//
//void GSceneManager::AddOctreeNode(OctreeNode * inNode, Octree * inOctant, int32 inDepth)
//{
//
//	LOG(DEBUG, "ADDING NEW NODE");
//	//skip if tree destroyed
//	if (inOctant)
//	{
//		//find this node's bounding box.
//		const AABB NodeBox = inNode->GetBoundingBox();
//
//		//check max depth to indepth && check if the node can fit inside a child node
//		if((inDepth < MaxDepth) && inOctant->isTwiceSize(NodeBox))
//			//a better way for my taste and understanding for checking the boxes is
//			// using AABB's contain method. 
//		{
//			//get the right octant 
//			int32 Index = inOctant->GetChildrenIndex(NodeBox);
//			
//			//if octant empty
//			if (inOctant->Children.At(Index) == 0)
//			{
//				LOG(DEBUG, "CREATING NEW CHILDREN TREE AT %d", Index);
//				//create new tree 
//				inOctant->Children[Index] = new Octree(inOctant);
//				
//				//set new min/max
//				const Point & octantMin = inOctant->Box.GetMinimum();
//				const Point & octantMax = inOctant->Box.GetMaximum();
//
//				Point TempMin, TempMax;
//				if((Index == 0) || (Index == 1) || (Index == 5) || (Index == 6))
//				{
//					TempMin.X = octantMin.X;
//					TempMax.X = (octantMin.X + octantMax.X) * 0.5;
//				}
//				else
//				{
//					TempMin.X = (octantMin.X + octantMax.X) * 0.5;
//					TempMax.X = octantMax.X;
//				}
//				
//				if ((Index == 0) || (Index == 3) || (Index == 6) || (Index == 7))
//				{
//					TempMin.Y = octantMin.Y;
//					TempMax.Y = (octantMin.Y + octantMax.Y) * 0.5;
//				}
//				else
//				{
//					TempMin.Y = (octantMin.Y + octantMax.X) * 0.5;
//					TempMax.Y = octantMax.Y;
//				}
//				
//				if (Index <= 3)
//				{
//					TempMin.Z = octantMin.Z;
//					TempMax.Z = (octantMin.Z + octantMax.Z) * 0.5;
//				}
//				else
//				{
//					TempMin.Z = (octantMin.Z + octantMax.Z) * 0.5; 
//					TempMax.Z = octantMax.Z;
//				}
//			
//
//
//				inOctant->Children[Index]->Box.SetExtents(TempMin, TempMax);
//
//				inOctant->Children[Index]->HalfSize = (TempMax - TempMin) * 0.5;
//
//			}
//
//			LOG(DEBUG, "Going Deeper at %d",inDepth+1);
//			//either way (empty or not)
//			//recurse through a level lower and check again
//			//AddOctreeNode(inNode, inOctant->Children[Index], ++inDepth);
//		}
//		//ELSE
//		//add node to octant
//		else
//		{
//			
//			inOctant->AddNode(inNode);
//
//			LOG(DEBUG, "Node Added");
//		}
//	}
//}


//void Scene::Draw(Shader shader,Shader light)
//{
//	std::multimap<ActorType, Actor*>::iterator it;
//	it = ActorMap.begin();
//	
//	//Set up Lights
//	while (it != ActorMap.end() && it->first == ActorType::LIGHT)
//	{
//		shader.enable();
//		//std::cout << "Setting Up Lights" << std::endl;
//		it->second->Light->Attributes->SetShader(shader, it->second->RootComponent->RelativeLocation);
//		//std::cout << it->second->StaticMeshGroup->RelativeLocation.x << std::endl;
//		light.enable();
//		it->second->StaticMeshGroup->SetShader(light);
//		it->second->StaticMeshGroup->GetMesh()->Draw(light);
//		it++;
//	}
//	////Set up Camera
//
//
//	//shader for light source.
//	ActorMap.find(ActorType::CAMERA)->second->Camera->SetShader(light);
//
//	shader.enable();
//	ActorMap.find(ActorType::CAMERA)->second->Camera->SetShader(shader);
//	//Set up Actors
//	//ActorMap.find(ActorType::ACTOR)->second->StaticMeshGroup->SetShader(shader);
//	
//	//Draw everything
//	//it = ActorMap.begin();
//	while (it != ActorMap.end() )
//	{
//		if (it->first == ActorType::ACTOR) {
//			it->second->StaticMeshGroup->SetShader(shader);
//			it->second->StaticMeshGroup->GetMesh()->Draw(shader);
//			//ActorMap.find(ActorType::ACTOR)->second->StaticMeshGroup->GetMesh()->Draw(shader);
//		}
//		it++;
//	}
//}

//bool Scene::AddActor(ActorType type, Actor * actor)
//{
//	bool ActorAdded = false;
//
//	ActorMap.insert(std::pair<ActorType, Actor*>(type, actor));
//
//	//std::cout <<"The MAP has SPOKEN:"<< ActorMap.count(ActorType::LIGHT) << std::endl;
//	return ActorAdded;
//}
