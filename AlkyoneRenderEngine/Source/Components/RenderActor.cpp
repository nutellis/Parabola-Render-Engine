#include <Components\RenderActor.h>
#include <Components\RenderComponents\StaticMeshComponent.h>
#include <ParabolaMath.h>
#include <Components/Camera.h>
#include <Components/LightComponents/DirectionalLightComponent.h>

PRenderActor::PRenderActor()
{
    std::cout << "This is an Actor\n";
  //  RootComponent = new PSceneComponent();
}

PRenderActor::PRenderActor(std::string NodeName): ObjectName(NodeName)
{
    ObjectPosition = Vector3f::ZERO;
    ObjectRotation = Vector3f::ZERO;
    ObjectScale = Vector3f::ONE;

    IsMovable = true;

}

void PRenderActor::AddLight()
{

    //StaticMesh = ObjectInitializer::CreateSubObject<PStaticMeshComponent>("Assets/ball.asset", this);//new PStaticMeshComponent("ball.asset", RootComponent, false);
    
    Light = new PDirectionalLightComponent(this);

    ActorType = EntityType::LIGHT;
}

void PRenderActor::AddMesh(const char* path)
{
    StaticMesh = new PStaticMeshComponent(this, path);

    ActorType = EntityType::MODEL;
}

void PRenderActor::AddCamera()
{
    Camera = new PCameraComponent(this);

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
    if (IsMovable) {
        if (Parent == nullptr) {
            ObjectPosition = inPosition;
        }
        else {
            ObjectPosition = Parent->ObjectPosition + inPosition;
        }

        for (auto i = 0; i < Children.Size(); i++) {
            Children[i]->SetPosition(ObjectPosition);
        }
    }
}

Vector3f PRenderActor::GetRotation()
{
    return ObjectRotation;
}

void PRenderActor::SetRotation(Vector3f inRotation)
{
    ObjectRotation = inRotation;

    //TODO: please make this better, it is ugly
    if (ActorType == EntityType::CAMERA) {
		this->Camera->RotateCamera(inRotation.X, inRotation.Y);
    }

    //TODO: need to care about children
}

void PRenderActor::AddRotation(Vector3f inRotation)
{
    ObjectRotation += inRotation;

    //TODO: need to care about children
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

void PRenderActor::SetupModelMatrix(Shader* ActiveShader) {

    ModelMatrix = Matrix4f::IDENTITY;

    Matrix4f S = Scale(GetScale(), Matrix4f::IDENTITY);
    Matrix4f R = Rotate(GetRotation(), Matrix4f::IDENTITY);
    Matrix4f T = Translate(GetPosition(), Matrix4f::IDENTITY);

    ModelMatrix = T * R * S;

    //ActiveShader->SetMat4(ActiveShader->Uniforms.ModelLocation, false, ModelMatrix);

}