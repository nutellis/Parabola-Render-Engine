#pragma once

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H


#include <Core/RenderCore.h>


#include <GLFW/glfw3.h>



#include <Core/SingletonBase.h>
#include <Managers/ManagerBase.h>


#include <Components/VertexBuffers.h>

#include <Components/Camera.h>

#include <Components/StaticMesh.h>


#include <HoneycombUIFramework/Atoms/ButtonWidget.h>


#include <Text.h>



class RenderSystem;

//TEMP
class Cube
{
public:
	VertexArray Vertices;
	IndexArray Indices;

	Cube(){

		Vertices = VertexArray(8);
		Indices = IndexArray(36);

		for (uint32 i = 0, j = 0; j < 8; i = i + 3, j++)
		{
			Vertices[j].Position = Vector3f(position[i], position[i + 1], position[i + 2]);
			//Vertices[j].Color = Vector3f(color[i], color[i + 1], color[i + 2]);
			Vertices[j].Normal = Vector3f(color[i], color[i + 1], color[i + 2]);
		}
		for (uint32 i = 0; i < 36; i++)
		{
			Indices[i] = indices[i];
		}
		Vertices[0].UVs = Vector2f(0, 0);
		Vertices[1].UVs = Vector2f(1, 0);
		Vertices[2].UVs = Vector2f(1, 1);
		Vertices[3].UVs = Vector2f(0, 1);

		Vertices[4].UVs = Vector2f(0, 0);
		Vertices[5].UVs = Vector2f(1, 0);
		Vertices[6].UVs = Vector2f(1, 1);
		Vertices[7].UVs = Vector2f(0, 1);

	}
private:
	GLfloat position[24] = {
		// front
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0
	};

	GLfloat color[24] = {
		// front colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		// back colors
		1.0, 0.5, 0.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 0.0
	};
	GLuint indices[36] =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLuint indices2[36] =
	{
		0, 6, 4,
		0, 2, 6,
		0, 3, 2,
		0, 1, 3,
		2, 7, 6,
		2, 3, 7,
		4, 6, 7,
		4, 7, 5,
		0, 4, 5,
		0, 5, 1,
		1, 5, 7,
		1, 7, 3

	};

};

class Pyramid {

public:
	
	unsigned int indices[18] = { 0,3,1, 1,3,2, 2,3,4 , 4,3,0,  0,1,2, 2,4,0 };

	VertexArray Vertices;
	IndexArray Indices;
	Pyramid() {

		Vertices = VertexArray(5);
		Indices = IndexArray(18);

		Vertices[0].Position = Vector3f(-1.0f, -1.0f, 0.0f); //bottom left //orange
		Vertices[1].Position = Vector3f(0.0f, -1.0f, 1.0f); //front  //cyan
		Vertices[2].Position = Vector3f(1.0f, -1.0f, 0.0f); //bottom right //blue
		Vertices[3].Position = Vector3f(0.0f, 1.0f, 0.0f);//tip //magenta
		Vertices[4].Position = Vector3f(0.0f, -1.0f, -1.0f); //back //red 

		//Vertices[0].Color = Vector3f(1.0f, 0.50f, 0.0f);
		//Vertices[1].Color = Vector3f(0.0f, 1.0f, 1.0f);
	//	Vertices[2].Color = Vector3f(0.0f, 0.0f, 1.0f);
	//	Vertices[3].Color = Vector3f(1.0f, 0.0f, 1.0f);
	//	Vertices[4].Color = Vector3f(1.0f, 0.0f, 0.0f);

		for (uint32 i = 0; i < 18; i++)
		{
			Indices[i] = indices[i];
		}
	}
};


struct Triangle {

	float vertices[9] = {
		// positions        
		 0.5f, -0.5f, 0.0f,    // bottom right
		-0.5f, -0.5f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f    // top 
	};

	float color[9] = {
      // colors
		  1.0f, 0.0f, 0.0f,  // bottom right
		  0.0f, 0.0f, 1.0f,  // bottom left
		  1.0f, 1.0f, 0.0f   // top 
	};

/*
		float vertices[18] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

	};*/
};

struct Plane
{
	VertexArray Vertices;
	IndexArray Indices;
	Plane() {
		Vertices = VertexArray(4);
		Indices = IndexArray(6);

			Vertices[0].Position = Vector3f(0.5, 0.5, 0);
			Vertices[1].Position = Vector3f(0.5, -0.5, 0);
			Vertices[2].Position = Vector3f(-0.5, -0.5, 0);
			Vertices[3].Position = Vector3f(-0.5, 0.5, 0);

			Vertices[0].Normal = Vector3f(0, 1,0);
			Vertices[1].Normal = Vector3f(0, 1,0);
			Vertices[2].Normal = Vector3f(0, 1,0);
			Vertices[3].Normal = Vector3f(0, 1,0);


			Indices[0] = 0;
			Indices[1] = 1;
			Indices[2] = 2;
			Indices[3] = 0;
			Indices[4] =2;
			Indices[5] = 3;
	}
	
};

class GGUIManager;

class GRenderManager : SingletonBase<GRenderManager>, ManagerBase
{
public:

	static GRenderManager & getInstance();
	static GRenderManager * getInstancePtr();


	GRenderManager();
	~GRenderManager();

	GWindowManager * WindowManager;

	GShaderManager * ShaderManager;

	GAssetLoader * AssetLoader;
	
private:
	RenderSystem * OpenGLSystem;

	VertexArrayObject vao, vio;

	VertexBufferObject vbo, ebo, col, pos;

	GLuint shaderProgram;

	ShaderUniforms uniforms;

public:

	void GetContextInfo();

	RenderSystem * GetContext();

	//void SetCommonUniforms(const PCameraComponent& Camera);

	void Render(double currentTime);

	void Update(double DeltaTime);


	Vector3f lightPos;


	void tempFunction();

	// Inherited via ManagerBase
	virtual void Init() override;

	virtual void Terminate() override;

private:
	int test;


	//temp
public:
	PStaticMesh cubemesh;
	PStaticMesh pyramidmesh;
	PStaticMesh planemesh;

	PStaticMesh light;

	HCButtonWidget testButton;

	Pyramid * pyramid;
	Plane plane;
	Cube * cube;
	Triangle model;
	uint32 VBO, VAO;

	int32 simpletimer = 0;

	Text *text;
	Camera * cmr ;
	//ModelManager test;
};


#endif // !RENDERER_H
