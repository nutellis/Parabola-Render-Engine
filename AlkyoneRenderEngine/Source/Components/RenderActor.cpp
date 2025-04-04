#include <Components\RenderActor.h>
#include <Components\RenderComponents\StaticMeshComponent.h>
#include <ParabolaMath.h>

PRenderActor::PRenderActor()
{
    std::cout << "This is an Actor\n";
  //  RootComponent = new PSceneComponent();
}

PRenderActor::PRenderActor(const char* NodeName): ObjectName(NodeName)
{
    ObjectPosition = Vector3f::ZERO;
    ObjectRotation = Vector3f::ZERO;
    ObjectScale = Vector3f::ONE;

}

void PRenderActor::AddLight()
{

    //StaticMesh = ObjectInitializer::CreateSubObject<PStaticMeshComponent>("Assets/ball.asset", this);//new PStaticMeshComponent("ball.asset", RootComponent, false);
    // 
    //RootComponent->RelativeLocation.x = 0.0;
    //RootComponent->RelativeLocation.y = 0.5;
    //RootComponent->RelativeLocation.z = 1.0;


   // SetPosition(Vector3f(0.0, 0.5, 1.0));

    Light = new PPointLightComponent(this);

    ActorType = EntityType::LIGHT;

    //StaticMesh->RelativeLocation = RootComponent->RelativeLocation;
    //StaticMesh->RelativeScale = glm::vec3(0.1f, 0.1f, 0.1f);

    //StaticMesh->GetMaterial()->mDiffuse->ch_Colour = glm::vec4(0.5f, 1.0f, 1.0f, 1.0f);
    //std::cout << "X " << StaticMesh->GetMaterial()->mDiffuse->ch_Colour.x << std::endl;
    //std::cout << "Y " << StaticMesh->GetMaterial()->mDiffuse->ch_Colour.y << std::endl;
    //std::cout << "Z " << StaticMesh->GetMaterial()->mDiffuse->ch_Colour.z << std::endl;
}

void PRenderActor::AddMesh(const char* path)
{
    StaticMesh = new PStaticMeshComponent(this, path);

    ActorType = EntityType::MODEL;
}

void PRenderActor::AddCamera()
{
   // RootComponent->RelativeLocation = Vector3f(0.0f, 0.0f, 3.f);

    Camera = new PCameraComponent(this);

    //Camera->LookAt(ObjectPosition, Vector3f(0.0f, 0.0f, 5.0f), Camera->Up);

    //Camera->Perspective(Camera->Zoom, (float)1280 / (float)720, 0.1f, 100.0f);

    ActorType = EntityType::CAMERA;
}

void PRenderActor::AddChild(PRenderActor* Child)
{
    Child->Parent = this;
    Children.PushBack(Child);
}

void PRenderActor::RemoveChild(PRenderActor* Child)
{
    //Children.Remove(Child);
}

void PRenderActor::RemoveAllChildren()
{
    Children.Clear();
}



Vector3f PRenderActor::GetPosition()
{
    return this->ObjectPosition;
}

void PRenderActor::SetPosition(Vector3f inPosition)
{
    if (Parent == nullptr) {
        ObjectPosition = Vector3f(0.0, 0.0, 0.0) + inPosition;
    }
    else {
        ObjectPosition = Parent->ObjectPosition + inPosition;
    }

    for (auto i = 0; i < Children.Size(); i++) {
        Children[i]->SetPosition(ObjectPosition);
    }

}

Vector3f PRenderActor::GetRotation()
{
    return ObjectRotation;
}

void PRenderActor::SetRotation(Vector3f inRotation)
{
    //TODO: nope. Fix dis
    ObjectRotation = inRotation;
}

void PRenderActor::resetOrientation()
{
    //TODO: resetOrientation
}

Vector3f PRenderActor::GetScale()
{
    //TODO: do something for uniform scaling? Maybe return a float?
    return ObjectScale;

}

void PRenderActor::SetScale(Vector3f inScale)
{
    ObjectScale = inScale;
}

void PRenderActor::SetScale(float inScale)
{
    ObjectScale = inScale;
}

void PRenderActor::Scaling(Vector3f Scaling)
{
    ObjectScale = Scaling;
}

void PRenderActor::DrawMeshChildren(Shader * ActiveShader) {
    if (StaticMesh != nullptr && ActorType == MODEL) {
        
        StaticMesh->ModelMatrix = Matrix4f::IDENTITY;

        StaticMesh->ModelMatrix = Scale(this->ObjectScale, StaticMesh->ModelMatrix);

        // fix angle parameter (StaticMesh->angle)
        // ModelMatrix = Rotate(this->ObjectRotation, StaticMesh->angle, ModelMatrix);

        StaticMesh->ModelMatrix = Translate(this->ObjectPosition, StaticMesh->ModelMatrix);


        StaticMesh->DrawComponent(ActiveShader);
    }
    for (auto i = 0; i < Children.Size(); i++) {
        Children[i]->DrawMeshChildren(ActiveShader);
    }
}


void PRenderActor::ControlCamera(uint32 Width, uint32 Height) {

    Camera->Perspective(45.0f, (float)Width / (float)Height, 5.0f, 2000.0f);

    Camera->LookAt(ObjectPosition, ObjectPosition + Camera->CameraDirection, Camera->WorldUp);
}