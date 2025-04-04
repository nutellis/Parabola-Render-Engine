#pragma once
#ifndef STATICMESH_H
#define STATICMESH_H

#include <Core/RenderCore.h>

#include <Components/VertexBuffers.h>

#include <Components/Texture.h>

#include <Utilities/Containers/Array.h>

#include <Utilities/AssetLoader.h>

class PStaticMeshComponent;

class PDrawableMesh 
{
	//empty for now
};

class PStaticMesh : PDrawableMesh
{
public:

	std::string Name;

	VertexArray Vertices;
	IndexArray Indices;
	uint32 MaterialIndex;

	uint32 IndexStart;
	uint32 NumVertices;

public:
	
	PStaticMesh();

	PStaticMesh(Asset* Mesh);

	PStaticMesh(const char * filepath);

	PStaticMesh(const VertexArray & vertex, IndexArray & index);

	~PStaticMesh();



	void SetupBuffers();

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

