#pragma once
#ifndef STATICMESH_H
#define STATICMESH_H

#include <Core/RenderCore.h>

#include <Components/VertexBuffers.h>

#include <Components/Texture.h>

#include <Utilities/Containers/Array.h>

#include <Utilities/AssetLoader.h>

class PStaticMeshComponent;
class PAxisAlignedBoundingBox;

class PDrawableMesh 
{
public:
	std::string Name;

};

class PStaticMesh : public PDrawableMesh
{
public:
	VertexArray Vertices;
	IndexArray Indices;
	uint32 MaterialIndex;

	uint32 IndexStart;
	uint32 NumVertices;

	PAxisAlignedBoundingBox * LocalBoundingBox;
	PAxisAlignedBoundingBox * WorldBoundingBox;

	bool IsCastingShadows = true;

public:
	
	PStaticMesh();

	PStaticMesh(const PStaticMesh& other);

	PStaticMesh(PStaticMesh&& other) noexcept;

	PStaticMesh(Asset* Mesh);

	PStaticMesh(const char * filepath);

	PStaticMesh(const VertexArray & vertex, IndexArray & index);

	~PStaticMesh();

	PStaticMesh& operator=(const PStaticMesh& Other);
	PStaticMesh& operator=(PStaticMesh&& Other) noexcept;

	void SetupBuffers();

	void Deserialize(const char* path);

	/*  StaticMesh Data  */
	//VertexBufferObject VBuffer;

	//IndexBufferObject IBuffer;
	
	//PMaterial *materials;
	//vector<RTexture> textures;
	

	/*  Functions  */
	// constructor
	//PStaticMesh(UArchive & Ar);
	
	//PStaticMesh(vector<Vertex> vertices, vector<uint32> indices, vector<RTexture> textures);
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

