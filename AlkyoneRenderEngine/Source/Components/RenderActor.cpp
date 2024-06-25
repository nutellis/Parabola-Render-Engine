#include <Components\RenderActor.h>
#include <ParabolaMath.h>

RenderActor::RenderActor()
{
    std::cout << "This is an Actor\n";
  //  RootComponent = new PSceneComponent();
}

RenderActor::RenderActor(const char* NodeName): ObjectName(NodeName)
{
    ObjectPosition = Vector3f::ZERO;
    ObjectRotation = Vector3f::ZERO;
    ObjectScale = Vector3f::ONE;

}

void RenderActor::AddLight()
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

void RenderActor::AddMesh(const char* path)
{
    StaticMesh = new PStaticMeshComponent(this, path);

    ActorType = EntityType::MODEL;
}

void RenderActor::AddCamera()
{
   // RootComponent->RelativeLocation = Vector3f(0.0f, 0.0f, 3.f);

    Camera = new PCameraComponent(this);

    //Camera->LookAt(ObjectPosition, Vector3f(0.0f, 0.0f, 5.0f), Camera->Up);

    //Camera->Perspective(Camera->Zoom, (float)1280 / (float)720, 0.1f, 100.0f);

    ActorType = EntityType::CAMERA;
}

void RenderActor::AddChild(RenderActor* Child)
{
    Child->Parent = this;
    Children.PushBack(Child);
}

void RenderActor::RemoveChild(RenderActor* Child)
{
    //Children.Remove(Child);
}

void RenderActor::RemoveAllChildren()
{
    Children.Clear();
}



Vector3f RenderActor::GetPosition()
{
    return this->ObjectPosition;
}

void RenderActor::SetPosition(Vector3f inPosition)
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

Vector3f RenderActor::GetRotation()
{
    return ObjectRotation;
}

void RenderActor::SetRotation(Vector3f inRotation)
{
    //TODO: nope. Fix dis
    ObjectRotation = inRotation;
}

void RenderActor::resetOrientation()
{
    //TODO: resetOrientation
}

Vector3f RenderActor::GetScale()
{
    //TODO: do something for uniform scaling? Maybe return a float?
    return ObjectScale;

}

void RenderActor::SetScale(Vector3f inScale)
{
    ObjectScale = inScale;
}

void RenderActor::Scaling(Vector3f Scaling)
{
    ObjectScale = Scaling;
}

void RenderActor::DrawMeshChildren(Shader * ActiveShader) {
    if (StaticMesh != nullptr && ActorType == MODEL) {
        
        StaticMesh->Model = Matrix4f::IDENTITY;

        StaticMesh->Model = Scale(this->ObjectScale, StaticMesh->Model);

        // fix angle parameter (StaticMesh->angle)
        // Model = Rotate(this->ObjectRotation, StaticMesh->angle, Model);

        StaticMesh->Model = Translate(this->ObjectPosition, StaticMesh->Model);


        StaticMesh->DrawComponent(ActiveShader);
    }
    for (auto i = 0; i < Children.Size(); i++) {
        Children[i]->DrawMeshChildren(ActiveShader);
    }
}


void RenderActor::ControlCamera() {

    Camera->LookAt(ObjectPosition, ObjectPosition + Camera->Front, Camera->Up);

    Camera->Perspective(Camera->Zoom, (float)1280 / (float)720, 0.1f, 100.0f);
}