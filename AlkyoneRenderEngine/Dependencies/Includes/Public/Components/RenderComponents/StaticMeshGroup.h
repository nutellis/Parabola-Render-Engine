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



class RStaticMesh;
class RMaterial;

//Component that has premade geometry
class RStaticMeshGroup : public PPrimitiveComponent {

public:

	RStaticMeshGroup(RRenderActor* Parent);
	RStaticMeshGroup(RRenderActor* Parent, const char* path);
	RStaticMeshGroup(const RStaticMeshGroup& Other);
	RStaticMeshGroup(RStaticMeshGroup&& Other) noexcept;
	~RStaticMeshGroup();

	RStaticMeshGroup& operator=(const RStaticMeshGroup& Other);

	RStaticMeshGroup& operator=(RStaticMeshGroup&& Other) noexcept;

	void SetupBuffers(uint32 DrawType = GL_STATIC_DRAW, PVertexComponentCount ComponentCount = PVertexComponentCount());

	void SetShaderMaterial(Shader* ActiveShader, RMaterial* Material) const;

	void SetupVertexBuffers(VertexArray Vertices);

	void DrawComponent(Shader* ActiveShader);

	void BindVAO();

	inline RStaticMesh* GetMesh(uint32 Index) const { return Meshes[Index]; }
	inline RMaterial* GetMaterial(uint32 Index) const { return Materials[Index]; }


	/*void Deserialize(UArchive& Ar);
	void BindTextures(Shader shader);*/

	Matrix4f ModelMatrix;

	float angle;

	VertexArray Vertices;
public:
	TArray<RStaticMesh*> Meshes;
	TArray<RMaterial*> Materials;

	VertexArrayObject VAO;

	VertexBufferObject VBO, EBO;

	
	IndexArray Indices;
};

#endif
