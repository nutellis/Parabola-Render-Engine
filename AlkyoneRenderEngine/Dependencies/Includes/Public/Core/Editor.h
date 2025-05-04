#pragma once

//here we can create, edit, delete SCENES that 
//can be loaded to the rendermanager 
//and later, on the state machine.

#include <Core/RenderCore.h>
#include <Core/SingletonBase.h>

class Scene;

class GEditor: SingletonManagerBase<GEditor> {
public:

	static GEditor& getInstance();
	static GEditor* getInstancePtr();


	GEditor();
	~GEditor();

	bool LoadScene();
	bool CreateScene();
	bool DeleteScene();

	Scene* GetActiveScene();

	virtual void Init() override;

	virtual void Terminate() override;

private:

public:
	GSceneManager * SceneManager;

};

