#include <Core/RenderCore.h>

#include <Components/SkyBox.h>
#include <string>
#include <Components/Texture.h>
#include <Utilities/Containers/Array.h>
#include <Components/RenderComponents/StaticMeshComponent.h>
#include <Components/CameraComponents/Camera.h>
#include <Components/Shader.h>
#include "RenderHelper.h"

RSkyBoxActor::RSkyBoxActor() : PRenderActor()
{
    ActorType = EntityType::SKYBOX;
    EnviromentMap = nullptr;
    IrradianceMap = nullptr;
    ReflectionMap = nullptr;
}

RSkyBoxActor::RSkyBoxActor(const char* Name) : PRenderActor(Name)
{
    ActorType = EntityType::SKYBOX;

    EnviromentMap = nullptr;
    IrradianceMap = nullptr;
    ReflectionMap = nullptr;

	StaticMesh = new PStaticMeshComponent(this);

    //VertexArray SkyVertices = RenderHelper::CreateFullScreenQuad();

    // StaticMesh->SetupVertexBuffers(SkyVertices);
    // StaticMesh->SetupBuffers(GL_STATIC_DRAW,PVertexComponentCount(2,3,2));

}

RSkyBoxActor::~RSkyBoxActor()
{
    delete EnviromentMap;
    delete IrradianceMap;
    delete ReflectionMap;
}

void RSkyBoxActor::Draw()
{
    RenderHelper::GetQuadActor().DrawFullScreenQuad();
}

void RSkyBoxActor::PrepareForDraw(Shader * ActiveShader, PCameraComponent * Camera)
{
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, EnviromentMap->TextureID);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, IrradianceMap->TextureID);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, ReflectionMap->TextureID);

    float environment_multiplier = 1.3f;
    ActiveShader->SetFloat("environment_multiplier", environment_multiplier);
    ActiveShader->SetMat4("inv_PV", false, Inverse(Camera->Projection * Camera->View));
    ActiveShader->SetVec3("camera_pos", Camera->GetPosition());
}

void RSkyBoxActor::CleanUp() {
    //glActiveTexture(GL_TEXTURE6);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glActiveTexture(GL_TEXTURE7);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glActiveTexture(GL_TEXTURE8);
    //glBindTexture(GL_TEXTURE_2D, 0);
}

void RSkyBoxActor::CreateSkyBox(std::string EnviromentPath, std::string IrradiancePath, TArray<std::string> ReflectionPaths)
{

    if (!EnviromentPath.empty())
    {
		EnviromentMap = new RTexture();
		EnviromentMap->SetWrapModeS(GL_CLAMP_TO_EDGE, false);

		EnviromentMap->SetWrapModeT(GL_CLAMP_TO_EDGE, false);
		EnviromentMap->SetFilterMin(GL_LINEAR, false);
		EnviromentMap->Generate(EnviromentPath, true);
    }

    if (!IrradiancePath.empty())
    {
        IrradianceMap = new RTexture();
        IrradianceMap->SetWrapModeS(GL_CLAMP_TO_EDGE, false);
        IrradianceMap->SetWrapModeT(GL_CLAMP_TO_EDGE, false);
        IrradianceMap->SetFilterMin(GL_LINEAR, false);
        IrradianceMap->Generate(IrradiancePath, true);
    }

    if (ReflectionPaths.IsNotEmpty())
    {

        ReflectionMap = new RTexture();
        ReflectionMap->SetWrapModeS(GL_REPEAT, false);
        ReflectionMap->SetWrapModeT(GL_MIRRORED_REPEAT, false);
        ReflectionMap->GenerateTextureLayers(ReflectionPaths,true);
    }
}
