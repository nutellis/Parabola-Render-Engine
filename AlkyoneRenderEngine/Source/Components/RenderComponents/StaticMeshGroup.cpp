#include <Components/RenderComponents/StaticMeshGroup.h>
#include <Utilities/AssetLoader.h>

#include <ParabolaMath.h>

#include <Components/RenderActor.h>
#include <Managers/ShaderManager.h>
#include <Components/Shader.h>

#include <Components/StaticMesh.h>
#include <Components/Material.h>


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//								STATIC MESH COMPONENT
//----------------------------------------------------------------------------------------
RStaticMeshGroup::RStaticMeshGroup(RRenderActor* Parent) : Vertices(), Meshes(), Materials(), Indices()
{
	this->Parent = Parent;
}


RStaticMeshGroup::RStaticMeshGroup(RRenderActor* Parent, const char* path) : Vertices(), Meshes(), Materials(), Indices()  //const char* path,PSceneComponent *Default,bool isAbsolute)
{
	std::cout << "This is a StaticMeshComponent\n";
	//	ComponentArchive->Load(path);
	//	Deserialize(*ComponentArchive);

	this->Parent = Parent;

	std::cout << "Parent is " + Parent->ObjectName + "\n";

	Asset* AssetToLoad = gAssetLoader.LoadAsset(path);
	

	Meshes = Utilities::Move(AssetToLoad->Meshes);
	Materials = Utilities::Move(AssetToLoad->Materials);

	if (Materials.IsEmpty()) {
		RMaterial* Material = new RMaterial();
		if (AssetToLoad->Materials.Size() <= 1) {
			Material->Diffuse.Colours = Vector4f(Vector3f((float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100), 1);
			Material->Specular.Colours = Vector4f(Vector3f(0.5f, 0.5f, 0.5f), 1);
			Material->Roughness.Colours = 32.0;
			Materials.PushBack(Material);
		}
	}

	Vertices = Utilities::Move(AssetToLoad->Vertices);
	Indices = Utilities::Move(AssetToLoad->Indices);

	SetupBuffers();

}

RStaticMeshGroup::RStaticMeshGroup(const RStaticMeshGroup& Other)
	: PPrimitiveComponent(Other),
	ModelMatrix(Other.ModelMatrix),
	angle(Other.angle),
	Vertices(Other.Vertices),
	Indices(Other.Indices),
	VAO(Other.VAO),
	VBO(Other.VBO),
	EBO(Other.EBO)
{
	// Deep copy meshes
	Meshes = TArray<RStaticMesh*>(Other.Meshes.Size());
	for (RStaticMesh* Mesh : Other.Meshes) {
		if (Mesh){
			Meshes.PushBack(new RStaticMesh(*Mesh));
		}
	}

	// Deep copy materials
	Materials = TArray<RMaterial*>(Other.Materials.Size());
	for (RMaterial* Mat : Other.Materials) {
		if (Mat) {
			Materials.PushBack(new RMaterial(*Mat));
		}
	}
}

// Move constructor
RStaticMeshGroup::RStaticMeshGroup(RStaticMeshGroup&& Other) noexcept
	: PPrimitiveComponent(Utilities::Move(Other)),
	ModelMatrix(Utilities::Move(Other.ModelMatrix)),
	angle(Other.angle),
	Vertices(Utilities::Move(Other.Vertices)),
	Meshes(Utilities::Move(Other.Meshes)),
	Materials(Utilities::Move(Other.Materials)),
	VAO(Utilities::Move(Other.VAO)),
	VBO(Utilities::Move(Other.VBO)),
	EBO(Utilities::Move(Other.EBO)),
	Indices(Utilities::Move(Other.Indices))
{
	Other.Meshes.Clear();
	Other.Materials.Clear();
	Other.Vertices.Clear();
	Other.Indices.Clear();
}

RStaticMeshGroup::~RStaticMeshGroup()
{
	Vertices.Clear();
	Meshes.Clear();
	Materials.Clear();
	Indices.Clear();
}


// Copy Assignment
RStaticMeshGroup& RStaticMeshGroup::operator=(const RStaticMeshGroup& Other)
{
	if (this == &Other)
		return *this;

	PPrimitiveComponent::operator=(Other);

	// Clean up current
	for (auto Mesh : Meshes)
		delete Mesh;
	Meshes.Clear();

	for (auto Mat : Materials)
		delete Mat;
	Materials.Clear();

	// Copy
	ModelMatrix = Other.ModelMatrix;
	angle = Other.angle;
	Vertices = Other.Vertices;
	Indices = Other.Indices;

	VAO = Other.VAO;
	VBO = Other.VBO;
	EBO = Other.EBO;

	//// Deep copy again
	//for (RStaticMesh* mesh : other.Meshes)
	//	Meshes.Add(mesh ? new RStaticMesh(*mesh) : nullptr);

	//for (RMaterial* mat : other.Materials)
	//	Materials.Add(mat ? new RMaterial(*mat) : nullptr);

	return *this;
}

// Move assignment operator
RStaticMeshGroup& RStaticMeshGroup::operator=(RStaticMeshGroup&& Other) noexcept
{
	if (this == &Other)
		return *this;

	PPrimitiveComponent::operator=(Utilities::Move(Other));
	ModelMatrix = Utilities::Move(Other.ModelMatrix);
	angle = Other.angle;
	Vertices = Utilities::Move(Other.Vertices);
	Meshes = Utilities::Move(Other.Meshes);
	Materials = Utilities::Move(Other.Materials);
	VAO = Utilities::Move(Other.VAO);
	VBO = Utilities::Move(Other.VBO);
	EBO = Utilities::Move(Other.EBO);
	Indices = Utilities::Move(Other.Indices);

	return *this;
}

void RStaticMeshGroup::SetupBuffers(uint32 DrawType, PVertexComponentCount ComponentCount)
{
	VAO.CreateArray(ComponentCount);

	VBO.CreateBuffer(DrawType, Vertices.SizeOf(), Vertices.Begin());

	VAO.SetupAttribute(VBO.GetIndex(), 0, VAO.ComponentCount.Position, offsetof(VertexFormat, VertexFormat::Position));

	VAO.SetupAttribute(VBO.GetIndex(), 1, VAO.ComponentCount.Normal, offsetof(VertexFormat, VertexFormat::Normal));

	VAO.SetupAttribute(VBO.GetIndex(), 2, VAO.ComponentCount.UVs, offsetof(VertexFormat, VertexFormat::UVs));

	VAO.AttachVertexBuffer(VBO.GetID(), VBO.GetIndex(), sizeof(VertexFormat));

}


void RStaticMeshGroup::SetShaderMaterial(Shader* ActiveShader, RMaterial* Material) const
{
	bool has_color_texture = Material->Diffuse.HasTexture;
	bool has_metalness_texture = Material->Metalness.HasTexture;
	bool has_fresnel_texture = Material->Fresnel.HasTexture;
	bool has_shininess_texture = Material->Roughness.HasTexture;
	bool has_emission_texture = Material->Emissive.HasTexture;
	if (has_color_texture)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Material->Diffuse.ChannelTexture->TextureID);
	}
	if ( has_metalness_texture )
	{
		glActiveTexture( GL_TEXTURE2 );
		glBindTexture( GL_TEXTURE_2D, Material->Metalness.ChannelTexture->TextureID);
	}
	if ( has_fresnel_texture )
	{
		glActiveTexture( GL_TEXTURE3 );
		glBindTexture( GL_TEXTURE_2D, Material->Fresnel.ChannelTexture->TextureID);
	}
	if ( has_shininess_texture )
	{
		glActiveTexture( GL_TEXTURE4 );
		glBindTexture( GL_TEXTURE_2D, Material->Roughness.ChannelTexture->TextureID);
	}
	if (has_emission_texture)
	{
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, Material->Emissive.ChannelTexture->TextureID);
	}
	//glActiveTexture(GL_TEXTURE0);

	
	ActiveShader->SetBool("has_color_texture", has_color_texture);
	ActiveShader->SetBool("has_metalness_texture", has_metalness_texture);
	ActiveShader->SetBool("has_fresnel_texture", has_fresnel_texture);
	ActiveShader->SetBool("has_shininess_texture", has_shininess_texture);
	ActiveShader->SetBool("has_emission_texture", has_emission_texture);

	// Set material properties
	ActiveShader->SetVec3("material_color", Material->Diffuse.Colours);
	ActiveShader->SetFloat("material_metalness", Material->Metalness.Colours[0]);
	ActiveShader->SetFloat("material_fresnel", Material->Fresnel.Colours[0]);
	ActiveShader->SetFloat("material_shininess", Material->Roughness.Colours[0]);
	ActiveShader->SetVec3("material_emission", Material->Emissive.Colours);

}

void RStaticMeshGroup::DrawComponent(Shader* ActiveShader) {

	VAO.Bind();

	// for each material
	for (auto& Mesh : Meshes)
	{
		SetShaderMaterial(ActiveShader, Materials[Mesh->MaterialIndex]);
		glDrawArrays(GL_TRIANGLES, Mesh->IndexStart, (GLsizei)Mesh->NumVertices);

	}
}

void RStaticMeshGroup::BindVAO()
{
	VAO.Bind();
}

void RStaticMeshGroup::SetupVertexBuffers(VertexArray Vertices)
{
	this->Vertices = Vertices;
}

//void RStaticMeshGroup::Deserialize(UArchive & Ar)
//{
//	std::cout << "LOL" << std::endl;
//	Mesh = new RStaticMesh(Ar);
//	cMaterial = new RMaterial(Ar);
//	cMaterial->SetupMaterial();
//
//}
//void RStaticMeshGroup::BindTextures(Shader shader)
//{
//	cMaterial;
//	glActiveTexture(GL_TEXTURE0);
//	//glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
//	// and finally bind the texture
//	//glBindTexture(GL_TEXTURE_2D, textures[i].id);
//}