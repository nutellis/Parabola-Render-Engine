#pragma once

#ifndef ASSETBUILDER_H
#define ASSETBUILDER_H

#include <Core/RenderCore.h>
#include <VertexFormat.h>

#include <ParabolaMath.h>

#include <fbxsdk.h>

#include <unordered_map>

#include <Core/SingletonBase.h>
#include <Managers/ManagerBase.h>


#include <Utilities/Containers/Array.h>

#include <fstream>
#include <string>

//#include <Utilities/Archive.h>



class PChannel;
class PMaterial;
class PStaticMesh;

struct Asset {

	Asset();
	~Asset();

	const char* name;
	TArray<VertexFormat> Vertices;
	IndexArray Indices;
	TArray<PMaterial *> Materials;
	TArray<PStaticMesh *> Meshes;
	IndexArray MaterialIndexMapping;

	//AxisAlignedBoundingBox BoundingBox;

	bool isEmpty();

	//std::vector<int64> indices;
	//std::vector<VertexFormat2> Vertex;

	//PMaterial *material;
};

//TArray<Vector3f> RawPositions;
//TArray<VertexFormat> s_Vertices;
//IndexArray s_Indices;

//  std::vector<glm::vec3> RawPositions;
//  std::vector<VertexFormat2> s_Vertices;
//  std::vector<int64> s_Indices;

//  PMaterial *mat;

//std::unordered_map<VertexFormat, uint32> s_IndexMapping;

class GAssetLoader : ManagerBase, SingletonBase<GAssetLoader>
{
public:
	GAssetLoader();
	~GAssetLoader();


	static GAssetLoader & getInstance();
	static GAssetLoader * getInstancePtr();

public:

	Asset * LoadAsset(const char * Filepath);
	
	// Inherited via ManagerBase
	virtual void Init() override;

	virtual void Terminate() override;

};


class BaseLoader {
public:

	//virtual void InsertVertex(const Vector3f& position, const Vector3f& normal, const Vector2f& uv);
	virtual void InsertVertex(bool hasUVs, bool hasNormals);

protected:
	TArray<Vector3f> Positions;
	TArray<Vector3f> Normals;
	TArray<Vector2f> UVs;
	IndexArray MaterialPerPolygonIndices;

	TArray<Vector3f> RawPositions;
	TArray<VertexFormat> Vertices;
	IndexArray Indices;


	Vector3f Max, Min;

	std::unordered_map<VertexFormat, uint32> IndexMapping;


	bool hasNormals;
	bool hasUVs;
	bool hasMaterials;



};

class FbxLoader : public BaseLoader
{
public:

	FbxLoader();
	~FbxLoader();

public:
	//Asset format;
	Asset * Read(const char* Filepath);

	void Print();
	FbxIOSettings* Settings(FbxManager *manager);

private:


	//FbxScene *aScene;
	//Archive *AssetArchive;

	
	const char* splitPath(const char *path);
	//void Serialize(Archive &Ar);
	//Asset * Deserialize(Archive &Ar);

	Vector3f Convert(FbxVector4 & ToConvert);

	Asset * Build(FbxNode *node);
	void FetchPosition(FbxMesh* mesh);
	Vector3f FetchNormal(FbxMesh* mesh, int gVertexIndex, int gVertexCounter);
	Vector2f FetchUVs(FbxMesh * mesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer);
	//Vector3f FetchBinormal(FbxMesh* mesh, int controlPointIndex, int gVertexCounter);
	//Vector3f FetchTangent(FbxMesh* mesh, int controlPointIndex, int gVertexCounter);
	
	TArray<PMaterial> FetchMaterial(FbxNode * node);

	PChannel* GetMaterialProperty(const FbxSurfaceMaterial* pMaterial, const char* pPropertyName, const char* pFactorPropertyName);

	const char *path;

};

class ObjLoader : BaseLoader
{
public:
	ObjLoader();
	~ObjLoader();

	Asset * Read(const char* Filepath);

	
	TArray<Vector3f> RawNormals;
	TArray<Vector2f> RawUVs;

private:

	const uint32 Split(const std::string & in, const char * deli, TArray<uint32> & Index);


};


#endif // !ASSETBUILDER_H

//EXEI PROBLHMA H KLASH ME TA TEXTURES !

