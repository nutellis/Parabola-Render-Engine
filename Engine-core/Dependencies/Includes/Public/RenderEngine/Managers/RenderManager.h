#pragma once

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H


#include <RenderEngine/Core/RenderCore.h>


#include <GLFW/glfw3.h>



#include <Core/SingletonBase.h>
#include <Core/Managers/ManagerBase.h>


#include <RenderEngine/Components/VertexArrayBuffer.h>

class RenderSystem;



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

	VertexFormat Vertices[6];
	Plane() {
		for (uint32 i = 0, j = 0; j < 6; i = i + 3, j++)
		{
			Vertices[j].Position = Vector3f(position[i], position[i + 1], position[i + 2]);
			Vertices[j].Color = Vector3f(color[i], color[i + 1], color[i + 2]);
		}
	}
	float position[18] = {
		// positions        
	  // first triangle
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f,  0.5f, 0.0f,  // top left 
	// second triangle
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};

	float color[18] = {  // colors
		1.0f, 0.50f, 0.0f,  // top right
		0.0f, 1.0f, 1.0f,  // bottom right
		0.0f, 0.0f, 1.0f,   // top left 

		0.0f, 1.0f, 1.0f,  // bottom right
		1.0f, 0.0f, 1.0f,  // bottom left
		0.0f, 0.0f, 1.0f,   // top left
	};
};


class GRenderManager : SingletonBase<GRenderManager>, ManagerBase
{
public:

	static GRenderManager & getInstance();
	static GRenderManager * getInstancePtr();


	GRenderManager();
	~GRenderManager();

	std::unique_ptr<GWindowManager> WindowManager;

	std::unique_ptr<GShaderManager> ShaderManager;

private:
	RenderSystem * OpenGLSystem;


	VertexArrayObject vao, vio;

	VertexBufferObject vbo, IBO, col, pos;

public:

	void GetContextInfo();

	RenderSystem * GetContext();


	void Render(double currentTime);





	void tempFunction();

	// Inherited via ManagerBase
	virtual void Init() override;

	virtual void Terminate() override;




	//temp
public:
	Plane plane;
	Cube cube;
	Triangle model;
	uint32 VBO, VAO;
};


#endif // !RENDERER_H
