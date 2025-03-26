#pragma once
#ifndef STATICMESH_H
#define STATICMESH_H

#include <Core/RenderCore.h>

#include <Components/VertexBuffers.h>

#include <Components/Texture.h>

#include <Utilities/Containers/Array.h>

#include <Utilities/AssetLoader.h>

class RenderActor;

class DrawableMesh 
{
	//empty for now
};

class PStaticMesh : DrawableMesh
{
public:

	const char * Name;

	VertexArray Vertices;
	IndexArray Indices;
	std::unordered_map<uint32, IndexArray> MaterialIndexMapping;

	TextureArray Textures;

	VertexArrayObject VAO;

	VertexBufferObject VBO, EBO;

private:
	// Parent provides all the info about transformation if needed
	RenderActor* Parent;


public:
	
	PStaticMesh();

	PStaticMesh(Asset* Mesh);

	PStaticMesh(const char * filepath);

	PStaticMesh(const VertexArray & vertex, IndexArray & index);

	~PStaticMesh();



	void SetupBuffers();


	RenderActor* GetParent();


	void Deserialize(const char* path);

	/*  StaticMesh Data  */
	//VertexBufferObject VBuffer;

	//IndexBufferObject IBuffer;
	
	//PMaterial *materials;
	//vector<Texture> textures;
	

	/*  Functions  */
	// constructor
	//PStaticMesh(UArchive & Ar);
	
	//PStaticMesh(vector<Vertex> vertices, vector<uint32> indices, vector<Texture> textures);
	// render the StaticMesh
	//void Draw(Shader shader);

	//void Deserialize(const char *path);

	//void Deserialize(UArchive & Ar);

private:

	//Asset * Data;
	/*  Render data  */
	

	/*  Functions    */
	// initializes all the buffer objects/arrays
	
};
#endif

