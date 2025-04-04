#include <Core/RenderCore.h>

#include <Components/SkyBox.h>
#include <string>
#include <Components/Texture.h>
#include <Utilities/Containers/Array.h>
#include <Components/RenderComponents/StaticMeshComponent.h>

PSkyBox::PSkyBox() : PRenderActor()
{
    ActorType = EntityType::SKYBOX;
    EnviromentMap = nullptr;
    IrradianceMap = nullptr;
    ReflectionMap = nullptr;
}

PSkyBox::PSkyBox(const char* Name) : PRenderActor(Name)
{
    ActorType = EntityType::SKYBOX;

    EnviromentMap = nullptr;
    IrradianceMap = nullptr;
    ReflectionMap = nullptr;

	StaticMesh = new PStaticMeshComponent(this);

    //VertexArray SkyVertices;

    // const Vector3f positions[] = { 
    //    { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f },
    //    { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f },  { -1.0f, 1.0f, 0.0f }
    //};

    // for (int i = 0; i < 6; i++)
    // {
    //     SkyVertices.PushBack(VertexFormat(positions[i], Vector3f(), Vector2f()));
    // }
    // StaticMesh->SetupVertexBuffers(SkyVertices);
    // StaticMesh->SetupBuffers(PVertexComponentCount(2,3,2));
}

PSkyBox::~PSkyBox()
{
    delete EnviromentMap;
    delete IrradianceMap;
    delete ReflectionMap;
}

void PSkyBox::Draw()
{
 //   GLboolean previous_depth_state;
 //   glGetBooleanv(GL_DEPTH_TEST, &previous_depth_state);
 //   glDisable(GL_DEPTH_TEST);
 //   
	//StaticMesh->BindVAO();
 //   glDrawArrays(GL_TRIANGLES, 0, 6);

 //   if (previous_depth_state)
 //       glEnable(GL_DEPTH_TEST);
 //   glBindVertexArray(0);
 //   glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLboolean previous_depth_state;
    glGetBooleanv(GL_DEPTH_TEST, &previous_depth_state);
    glDisable(GL_DEPTH_TEST);
    static GLuint vertexArrayObject = 0;
    static int nofVertices = 6;
    // do this initialization first time the function is called...
    if (vertexArrayObject == 0)
    {
        glCreateVertexArrays(1, &vertexArrayObject);
        static const glm::vec2 positions[] = { { -1.0f, -1.0f }, { 1.0f, -1.0f }, { 1.0f, 1.0f },
                                               { -1.0f, -1.0f }, { 1.0f, 1.0f },  { -1.0f, 1.0f } };
        GLuint buffer;
        glCreateBuffers(1, &buffer);
        glNamedBufferData(buffer, sizeof(positions), positions, GL_STATIC_DRAW);
        glVertexArrayVertexBuffer(vertexArrayObject, 0, buffer, 0, sizeof(glm::vec2));
        glEnableVertexArrayAttrib(vertexArrayObject, 0);
        glVertexArrayAttribFormat(vertexArrayObject, 0, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vertexArrayObject, 0, 0);
    }
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, nofVertices);
    if (previous_depth_state)
        glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);


}

void PSkyBox::CreateSkyBox(std::string EnviromentPath, std::string IrradiancePath, TArray<std::string> ReflectionPaths)
{
    if (!EnviromentPath.empty())
    {
		EnviromentMap = new Texture();
		EnviromentMap->SetWrapModeS(GL_CLAMP_TO_EDGE, false);
		EnviromentMap->SetWrapModeT(GL_CLAMP_TO_EDGE, false);
		EnviromentMap->SetFilterMin(GL_LINEAR, false);
		EnviromentMap->Generate(EnviromentPath, true);
    }

    if (!IrradiancePath.empty())
    {
        IrradianceMap = new Texture();
        IrradianceMap->SetWrapModeS(GL_CLAMP_TO_EDGE, false);
        IrradianceMap->SetWrapModeT(GL_CLAMP_TO_EDGE, false);
        IrradianceMap->SetFilterMin(GL_LINEAR, false);
        IrradianceMap->Generate(IrradiancePath, true);
    }

    if (ReflectionPaths.IsNotEmpty())
    {

        ReflectionMap = new Texture();
        ReflectionMap->SetWrapModeS(GL_REPEAT, false);
        ReflectionMap->SetWrapModeT(GL_MIRRORED_REPEAT, false);
        ReflectionMap->GenerateMipMaps(ReflectionPaths,true);
    }
}
