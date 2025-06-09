#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <Components/RenderActor.h>

class Image;


class RSkyBoxActor: public PRenderActor
{
public:
    RSkyBoxActor();
    RSkyBoxActor(const char* Name);
    ~RSkyBoxActor();

    void Draw();
    void PrepareForDraw(Shader* ActiveShader, PCameraComponent* Camera);
    void CleanUp();
    void CreateSkyBox(std::string EnviromentPath, std::string IrradiancePath, TArray<std::string> ReflectionPaths);

public:
    RTexture* EnviromentMap;
    RTexture* IrradianceMap;
    RTexture* ReflectionMap;
};

#endif // !SKYBOX_H