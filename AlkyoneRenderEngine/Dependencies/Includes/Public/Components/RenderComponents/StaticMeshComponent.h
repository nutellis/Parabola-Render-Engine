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
	PStaticMeshComponent(const PStaticMeshComponent& Other);
	PStaticMeshComponent(PStaticMeshComponent&& Other) noexcept;
	~PStaticMeshComponent();

	PStaticMeshComponent& operator=(const PStaticMeshComponent& Other);

	PStaticMeshComponent& operator=(PStaticMeshComponent&& Other) noexcept;

	void SetupBuffers(uint32 DrawType = GL_STATIC_DRAW, PVertexComponentCount ComponentCount = PVertexComponentCount());

	void SetShaderMaterial(Shader* ActiveShader, PMaterial* Material) const;

	void SetupVertexBuffers(VertexArray Vertices);

	void DrawComponent(Shader* ActiveShader);

	void BindVAO();

	inline PStaticMesh* GetMesh(uint32 Index) const { return Meshes[Index]; }
	inline PMaterial* GetMaterial(uint32 Index) const { return Materials[Index]; }


	/*void Deserialize(UArchive& Ar);
	void BindTextures(Shader shader);*/

	Matrix4f ModelMatrix;

	float angle;

	VertexArray Vertices;
public:
	TArray<PStaticMesh*> Meshes;
	TArray<PMaterial*> Materials;

	VertexArrayObject VAO;

	VertexBufferObject VBO, EBO;

	
	IndexArray Indices;
};

#endif
