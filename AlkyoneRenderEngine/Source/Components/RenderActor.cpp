#include <Components\RenderActor.h>
#include <Components\RenderComponents\StaticMeshComponent.h>
#include <ParabolaMath.h>
#include <Components/CameraComponents/Camera.h>
#include <Components/LightComponents/DirectionalLightComponent.h>
#include <Components/StaticMesh.h>
#include <Components/Colliders/BoundingBox.h>
#include <Components/Colliders/BoundingHelper.h>
#include <RenderHelper.h>

RRenderActor::RRenderActor()
{
    std::cout << "This is an Actor\n";
  //  RootComponent = new PSceneComponent();
}

RRenderActor::RRenderActor(std::string NodeName): ObjectName(NodeName)
{
    ObjectPosition = Vector3f::ZERO;
    ObjectRotation = Vector3f::ZERO;
    ObjectScale = Vector3f::ONE;

    IsMovable = true;

}

void RRenderActor::AddLight()
{

    //StaticMesh = ObjectInitializer::CreateSubObject<PStaticMeshComponent>("Assets/ball.asset", this);//new PStaticMeshComponent("ball.asset", RootComponent, false);
    
    Light = new PDirectionalLightComponent(this);

    ActorType = EntityType::LIGHT;
}

void RRenderActor::AddMesh(const char* path)
{
    StaticMesh = new PStaticMeshComponent(this, path);

    ActorType = EntityType::MODEL;
}

void RRenderActor::AddChild(RRenderActor* Child)
{
    Child->Parent = this;
    Children.PushBack(Child);
}

void RRenderActor::RemoveChild(RRenderActor* Child)
{
    //Children.Remove(Child);
}

void RRenderActor::RemoveAllChildren()
{
    Children.Clear();
}



Vector3f RRenderActor::GetPosition()
{
    return this->ObjectPosition;
}

void RRenderActor::SetPosition(Vector3f inPosition)
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
        if (ActorType == MODEL) {
            UpdateWorldBoundingBox();
        }
    }
}

Vector3f RRenderActor::GetRotation()
{
    return ObjectRotation;
}

void RRenderActor::SetRotation(Vector3f inRotation)
{
    if (Parent == nullptr) {
        ObjectRotation = inRotation;
    }
    else {
        ObjectRotation = Parent->ObjectRotation + inRotation;
    }

    for (auto i = 0; i < Children.Size(); i++) {
        Children[i]->SetRotation(ObjectRotation);
    }
    //TODO: need to care about children
    if (ActorType == MODEL) {
        UpdateWorldBoundingBox();
    }
}

void RRenderActor::AddRotation(Vector3f inRotation)
{
    ObjectRotation = inRotation;

}

void RRenderActor::resetOrientation()
{
    //TODO: resetOrientation
}

Vector3f RRenderActor::GetScale()
{
    //TODO: do something for uniform scaling? Maybe return a float?
    return ObjectScale;

}

void RRenderActor::SetScale(Vector3f inScale)
{
    ObjectScale = inScale;
    if (ActorType == MODEL) {
        UpdateWorldBoundingBox();
    }
}

void RRenderActor::SetScale(float inScale)
{
    ObjectScale = inScale;
    if (ActorType == MODEL) {
        UpdateWorldBoundingBox();
    }
}

void RRenderActor::DrawMeshChildren(Shader * ActiveShader) {
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

//TODO: Use this only after a transformation to reduce recalculations
void RRenderActor::SetupModelMatrix() {

    StaticMesh->ModelMatrix = Matrix4f::IDENTITY;

    Matrix4f S = Scale(GetScale(), Matrix4f::IDENTITY);
    Matrix4f R = Rotate(GetRotation(), Matrix4f::IDENTITY);
    Matrix4f T = Translate(GetPosition(), Matrix4f::IDENTITY);

    StaticMesh->ModelMatrix = T * R * S;
}


void RRenderActor::UpdateWorldBoundingBox() {
    SetupModelMatrix();

    for (PStaticMesh* Mesh : StaticMesh->Meshes) {
        if (Mesh->WorldBoundingBox == nullptr) {
            Mesh->WorldBoundingBox = new AABB();
        }
        *Mesh->WorldBoundingBox = BoundingHelper::TransformAABB(
                Mesh->LocalBoundingBox, StaticMesh->ModelMatrix
        );
    }
}



// RQUAD RENDER ACTOR

RQuadRenderActor::RQuadRenderActor() {
    Vertices = CreateFullScreenQuad();

    RenderHelper::SetupBuffers(VAO, VBO, Vertices, PVertexComponentCount(2, 3, 2), GL_STATIC_DRAW);
 }

RQuadRenderActor::~RQuadRenderActor() {
    VAO.Terminate();
    VBO.Terminate();
}

VertexArray RQuadRenderActor::CreateFullScreenQuad()
{
    VertexArray FullScreenQuad = VertexArray(6);

    const Vector3f positions[] = {
        { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f },  { -1.0f, 1.0f, 0.0f }
    };
    const Vector2f uvs[] = {
    { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
    { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
    };

    for (int i = 0; i < 6; i++)
    {
        FullScreenQuad.PushBack(VertexFormat(positions[i], Vector3f(0.0f, 0.0f, 1.0f), uvs[i]));
    }

    return FullScreenQuad;

}

void RQuadRenderActor::DrawFullScreenQuad() 
{
    GLboolean DepthState;
    glGetBooleanv(GL_DEPTH_TEST, &DepthState);
    glDisable(GL_DEPTH_TEST);

    VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (DepthState) {
        glEnable(GL_DEPTH_TEST);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
