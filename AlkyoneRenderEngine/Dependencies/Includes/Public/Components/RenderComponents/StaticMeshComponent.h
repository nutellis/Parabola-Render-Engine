#pragma once
#ifndef STATIC_MESH_COMPONENTS_H
#define STATIC_MESH_COMPONENTS_H

#include <Core/RenderCore.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Components/Lights.h>
#include <Utilities/Containers/Array.h>
#include <Components/Components.h>
#include <Components/VertexBuffers.h>



class PStaticMesh;
class PMaterial;

//Component that has premade geometry
class PStaticMeshComponent : public PPrimitiveComponent {

public:

	PStaticMeshComponent(PRenderActor* Parent);
	PStaticMeshComponent(PRenderActor* Parent, const char* path);

	void SetupBuffers(PVertexComponentCount ComponentCount = PVertexComponentCount());

	void SetShaderMaterial(Shader* ActiveShader, PMaterial* Material) const;

	void SetupModelMatrix(Shader* ActiveShader);

	void SetupVertexBuffers(VertexArray Vertices);

	void DrawComponent(Shader* ActiveShader);

	void BindVAO();

	inline PStaticMesh* GetMesh(uint32 Index) const { return Meshes[Index]; }
	inline PMaterial* GetMaterial(uint32 Index) const { return Materials[Index]; }


	/*void Deserialize(UArchive& Ar);
	void BindTextures(Shader shader);*/

	Matrix4f ModelMatrix;

	float angle;

private:
	TArray<PStaticMesh*> Meshes;
	TArray<PMaterial*> Materials;

	VertexArrayObject VAO;

	VertexBufferObject VBO, EBO;

	VertexArray Vertices;
	IndexArray Indices;
};

#endif
