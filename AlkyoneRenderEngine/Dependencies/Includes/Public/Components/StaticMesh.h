#pragma once
#ifndef STATICMESH_H
#define STATICMESH_H

#include <Core/RenderCore.h>

#include <Components/VertexBuffers.h>

#include <Components/Texture.h>

#include <Utilities/Containers/Array.h>

#include <Utilities/AssetLoader.h>

class RStaticMeshGroup;
class PAxisAlignedBoundingBox;

class RDrawableMesh 
{
public:
	std::string Name;

};

class RStaticMesh : public RDrawableMesh
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
	bool HasTransparency = true;

public:
	
	RStaticMesh();

	RStaticMesh(const RStaticMesh& other);

	RStaticMesh(RStaticMesh&& other) noexcept;

	RStaticMesh(Asset* Mesh);

	RStaticMesh(const char * filepath);

	RStaticMesh(const VertexArray & vertex, IndexArray & index);

	~RStaticMesh();

	RStaticMesh& operator=(const RStaticMesh& Other);
	RStaticMesh& operator=(RStaticMesh&& Other) noexcept;

	void SetupBuffers();

	void Deserialize(const char* path);

	/*  StaticMesh Data  */
	//VertexBufferObject VBuffer;

	//IndexBufferObject IBuffer;
	
	//RMaterial *materials;
	//vector<RTexture> textures;
	

	/*  Functions  */
	// constructor
	//RStaticMesh(UArchive & Ar);
	
	//RStaticMesh(vector<Vertex> vertices, vector<uint32> indices, vector<RTexture> textures);
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

