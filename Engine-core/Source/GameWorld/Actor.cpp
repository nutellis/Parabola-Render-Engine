//#include <GameWorld/Actor.h>
//
//Actor::Actor()
//{
//	std::cout << "This is an Actor\n";
//	RootComponent = new PSceneComponent();
//}
//
//Actor::Actor(const char * dontknowyet)
//{ 
//	//TODO
//} 
//
//void Actor::AddLight() 
//{
//
//	StaticMesh = ObjectInitializer::CreateSubObject<PStaticMeshComponent>("Assets/ball.asset", this);//new PStaticMeshComponent("ball.asset", RootComponent, false);
//	 
//	RootComponent->RelativeLocation.x = 0.0;
//	RootComponent->RelativeLocation.y = 0.5;
//	RootComponent->RelativeLocation.z = 1.0;
//
//
//
//	Light = new PPointLightComponent(RootComponent);
//
//	StaticMesh->RelativeLocation = RootComponent->RelativeLocation;
//	StaticMesh->RelativeScale = glm::vec3(0.1f, 0.1f, 0.1f);
//
//	StaticMesh->GetMaterial()->mDiffuse->ch_Colour = glm::vec4(0.5f, 1.0f, 1.0f, 1.0f);
//	std::cout << "X " << StaticMesh->GetMaterial()->mDiffuse->ch_Colour.x << std::endl;
//	std::cout << "Y " << StaticMesh->GetMaterial()->mDiffuse->ch_Colour.y << std::endl;
//	std::cout << "Z " << StaticMesh->GetMaterial()->mDiffuse->ch_Colour.z << std::endl;
//}
//
//PWorld * Actor::GetWorld() const
//{
//	//check for flags (?)
//
//
//	//For now. Later on use Level->OwningWorld;
//	return nullptr;// this->GetTypedOuter<PWorld*>();
//}
//
//void Actor::ObjectS()
//{
//	StaticMesh = ObjectInitializer::CreateSubObject<PStaticMeshComponent>("Assets/ball.asset",this);
//
//
//	//StaticMesh = new PStaticMeshComponent("cube.asset", RootComponent,false);
//	//StaticMesh->RelativeRotation.y = 0.0;
//	StaticMesh->GetMaterial()->mDiffuse->ch_Colour = glm::vec4(1.0f, 0.2f, 0.6f, 1.0f);
//
//}
//
//void Actor::AddCamera()
//{
//	RootComponent->RelativeLocation = glm::vec3(0.0f, 0.0f, 3.f);
//
//	Camera = new PCameraComponent(RootComponent->RelativeLocation);
//}
//
//void Actor::Update()
//{
//
//}
///*Base* pBase = new Derived; // OK, the dynamic type of pBase is Derived
//Derived* pDerived = dynamic_cast<Derived*>(pBase);
//if(pDerived) // always test  
//{
//    // success
//}
//else
//{
//    // fail to down-cast
//	}*/