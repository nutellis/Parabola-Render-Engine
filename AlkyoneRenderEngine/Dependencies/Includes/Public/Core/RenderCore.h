#pragma once

#include <GL/glew.h>

#include <ImGUI\imgui.h>
#include <ImGUI\imgui_impl_glfw.h>
#include <ImGUI\imgui_impl_opengl3.h>

#include <Core/PODTypes.h>

#include <Core/UDTypes.h>

#define WITH_EDITOR 1

class GLogManager;

class GRenderManager;
class GWindowManager;
class GInputManager;
class GShaderManager;
class GSceneManager;
class GAssetLoader;

class GGUIManager;
class GEditor;


#define LOGMANAGER GLogManager::getInstance()
#define RENDERMANAGER GRenderManager::getInstance()
#define WINDOWMANAGER GWindowManager::getInstance()
#define ASSETLOADER GAssetLoader::getInstance()
#define SHADERMANAGER GShaderManager::getInstance()

#define SCENEMANAGER GSceneManager::getInstance()

//#define ASSETLOADER GAssetLoader::getInstance()

#define GUIMANAGER GGUIManager::getInstance()

#define EDITOR GEditor::getInstance()

//TODO : i dont like this here... will have to move!!
#include <memory>

//#define TRUE  1
//#define FALSE 0