#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <Components/RenderActor.h>

class Image;


class PSkyBox: public PRenderActor
{
public:
    PSkyBox();
    PSkyBox(const char* Name);
    ~PSkyBox();

    void Draw();

    void CreateSkyBox(std::string EnviromentPath, std::string IrradiancePath, TArray<std::string> ReflectionPaths);

public:
    Texture* EnviromentMap;
    Texture* IrradianceMap;
    Texture* ReflectionMap;
};

#endif // !SKYBOX_H