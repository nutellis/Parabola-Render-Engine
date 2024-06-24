//#include "GameWorld/Scene.h"
//
//Scene::Scene()
//{
//	std::cout << "Init Scene" << std::endl;
//}
//
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
//		//std::cout << it->second->StaticMesh->RelativeLocation.x << std::endl;
//		light.enable();
//		it->second->StaticMesh->SetShader(light);
//		it->second->StaticMesh->GetMesh()->Draw(light);
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
//	//ActorMap.find(ActorType::ACTOR)->second->StaticMesh->SetShader(shader);
//	
//	//Draw everything
//	//it = ActorMap.begin();
//	while (it != ActorMap.end() )
//	{
//		if (it->first == ActorType::ACTOR) {
//			it->second->StaticMesh->SetShader(shader);
//			it->second->StaticMesh->GetMesh()->Draw(shader);
//			//ActorMap.find(ActorType::ACTOR)->second->StaticMesh->GetMesh()->Draw(shader);
//		}
//		it++;
//	}
//}
//
//bool Scene::AddActor(ActorType type, Actor * actor)
//{
//	bool ActorAdded = false;
//
//	ActorMap.insert(std::pair<ActorType, Actor*>(type, actor));
//
//	//std::cout <<"The MAP has SPOKEN:"<< ActorMap.count(ActorType::LIGHT) << std::endl;
//	return ActorAdded;
//}
