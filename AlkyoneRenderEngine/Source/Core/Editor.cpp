#include "Core\Editor.h"


#include <Managers/SceneManager.h>


#include <Managers/GUIManager.h>

template<> GEditor* SingletonManagerBase<GEditor>::instance = 0;
GEditor& GEditor::getInstance()
{
	return (*instance);
}

GEditor* GEditor::getInstancePtr()
{
	if (instance == nullptr)
	{
		instance = new GEditor();
	}
	return instance;
}

GEditor::GEditor()
{
	SceneManager = GSceneManager::getInstancePtr();
}

GEditor::~GEditor()
{
	SceneManager = nullptr;
}

bool GEditor::CreateScene()
{
	bool Created = false;

	return Created;
}

Scene* GEditor::GetActiveScene()
{
	return SceneManager->GetActiveScene();
}

void GEditor::Init()
{
	LOG(DEBUG, "Initiating EDITOR\n");

	GSceneManager::getInstance().Init();
	
	LOG(INFO, "EDITOR INITIATED\n");
	
}

void GEditor::Terminate()
{
	GSceneManager::getInstance().Terminate();
}
