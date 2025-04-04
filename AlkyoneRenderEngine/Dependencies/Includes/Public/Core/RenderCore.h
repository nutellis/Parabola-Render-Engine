#pragma once

#include <GL/glew.h>

#include <ImGUI\imgui.h>
#include <ImGUI\imgui_impl_glfw.h>
#include <ImGUI\imgui_impl_opengl3.h>

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


#define gLogManager GLogManager::getInstance()
#define gRenderManager GRenderManager::getInstance()
#define gWindowManager GWindowManager::getInstance()
#define gAssetLoader GAssetLoader::getInstance()
#define gShaderManager GShaderManager::getInstance()

#define gSceneManager GSceneManager::getInstance()

#define gGuiManager GGUIManager::getInstance()

#define EDITOR GEditor::getInstance()

//TODO : i dont like this here... will have to move!!
#include <memory>
