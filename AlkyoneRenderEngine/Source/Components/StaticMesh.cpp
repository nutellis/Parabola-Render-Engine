#include <Components\StaticMesh.h>
#include <Utilities/Containers/Array.h>


PStaticMesh::PStaticMesh() : Vertices(),
Indices(), VAO(), VBO()// , EBO()
{

	//Deserialize("ball.asset");

}

PStaticMesh::PStaticMesh(Asset* Mesh)
	: Vertices(), Indices(), VAO(), VBO()// , EBO()
{
	if (!Mesh->isEmpty())
	{
		Vertices = Mesh->Vertex;
		Indices = Mesh->indices;
	}
}

PStaticMesh::PStaticMesh(const char * filepath) : Vertices(),
Indices(), VAO(), VBO()// , EBO()
{
	//ObjLoader load = ObjLoader();
	//FbxLoader load = FbxLoader();
	//Data = load.Read(filepath);
	
	//if (Data)
	//{
	//	Vertices = Data->Vertex;
	//	Indices = Data->indices;

		

		//Textures = TextureArray();
		//Textures.PushBack(Texture());
		//Textures[0].Generate();
	//}

	//ObjLoader::Read("Assets/cube.obj");

	//else
	//{
	//	//oug!
	//}

}

PStaticMesh::PStaticMesh(const VertexArray & vertex, IndexArray & index) :
	Vertices(vertex),
	Indices(index),
	VAO(),
	VBO()
	// EBO()
{
//	Textures = TextureArray();
	//Textures.PushBack(Texture());
 
	//Textures[0].Generate();

	//Textures.PushBack(Texture());


}

PStaticMesh::~PStaticMesh()
{
	//Vertices.~TArray();
	//Indices.~TArray();
}

void PStaticMesh::SetupBuffers()
{
	VAO.CreateArray();
	VBO.CreateBuffer(Vertices.SizeOf(), Vertices.Begin());

	//if(MaterialIndexMapping.size() != 0) {
	//	for (auto& Material : MaterialIndexMapping) {
	//		VertexBufferObject* EBO = new VertexBufferObject();
	//		EBO->CreateBuffer(Material.second.SizeOf(), Material.second.Begin());
	//		//EBOs.PushBack(EBO);
	//	}
	//}
	//else {
		VertexBufferObject* EBO = new VertexBufferObject();
		EBO->CreateBuffer(Indices.SizeOf(), Indices.Begin());
		//EBOs.PushBack(EBO);
		VAO.AttachElementBuffer(EBO->GetID());
	//}
	
	

	uint32 VBOIndex = VBO.GetIndex();

	VAO.SetupAttribute(VBOIndex, 0, offsetof(VertexFormat, VertexFormat::Position));
	VAO.SetupAttribute(VBOIndex, 1, offsetof(VertexFormat, VertexFormat::Normal));
	VAO.SetupAttribute(VBOIndex, 2, offsetof(VertexFormat, VertexFormat::UVs));
	

	VAO.AttachVertexBuffer(VBOIndex, VBOIndex, sizeof(VertexFormat));

	//VAO.AttachElementBuffer(EBO.GetID());

	//VAO.SetupAttribute(VBO.GetIndex(), 0, offsetof(VertexFormat, VertexFormat::Position));

}

RenderActor * PStaticMesh::GetParent()
{
	return Parent;
}

void PStaticMesh::Deserialize(const char * path)
{
	int i = 0;

	size_t vertexSize = 0, indexSize = 0;

	std::ifstream in;
	in.open(path, std::ios::binary);
	if (in)
	{
		//Vertex
		in.read((char*)&vertexSize, sizeof(uint8));
		std::cout << vertexSize<< "\n\n\n\n\n";
		Vertices = VertexArray(vertexSize);

		for (i = 0; i < vertexSize; i++) {
			in.read((char*)&Vertices[i], sizeof(VertexFormat));
		}

		//Index
		in.read((char*)&indexSize, sizeof(size_t));
		Indices = IndexArray(indexSize);

		for (i = 0; i < indexSize; i++) {
			in.read((char*)&Indices[i], sizeof(uint32));
		}

	}

	in.close();

}

//PStaticMesh::PStaticMesh(UArchive &Ar) {
//	//vertices = VertexArray(0);
//	//indices = IndexArray(0);
//
//
//	Deserialize(Ar);
//
//	setupStaticMesh();
//}


//PStaticMesh::PStaticMesh(vector<Vertex> vertices, vector<uint32> indices, vector<Texture> textures)
//{
//	this->vertices = vertices;
//	this->indices = indices;
//	//this->textures = textures;
//
//	// now that we have all the required data, set the vertex buffers and its attribute pointers.
//	setupStaticMesh();
//
//}

//void PStaticMesh::Draw(Shader shader)
//{
///*
//	// bind appropriate textures
//	uint32 diffuseNr = 1;
//	uint32 specularNr = 1;
//	uint32 normalNr = 1;
//	uint32 heightNr = 1;
//	for (uint32 i = 0; i < textures.size(); i++)
//	{
//		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
//										  // retrieve texture number (the N in diffuse_textureN)
//		stringstream ss;
//		string number;
//		string name = textures[i].type;
//		if (name == "texture_diffuse")
//			ss << diffuseNr++; // transfer uint32 to stream
//		else if (name == "texture_specular")
//			ss << specularNr++; // transfer uint32 to stream
//		else if (name == "texture_normal")
//			ss << normalNr++; // transfer uint32 to stream
//		else if (name == "texture_height")
//			ss << heightNr++; // transfer uint32 to stream
//		number = ss.str();
//		// now set the sampler to the correct texture unit
//		glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
//		// and finally bind the texture
//		glBindTexture(GL_TEXTURE_2D, textures[i].id);
//	}
//	*/
//	// draw StaticMesh
//	//glBindVertexArray(VAO);
//	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//	//glBindVertexArray(0);
//
//	// always good practice to set everything back to defaults once configured.
//	//glActiveTexture(GL_TEXTURE0);
//}
//
//void PStaticMesh::setupStaticMesh()
//{
//	// create buffers/arrays
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//
//	glBindVertexArray(VAO);
//	// load data into vertex buffers
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//	// A great thing about structs is that their memory layout is sequential for all its items.
//	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
//	// again translates to 3/2 floats which translates to a byte array.
//	
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), &indices[0], GL_STATIC_DRAW);
//
//	//// set the vertex attribute pointers
//	//// vertex Positions
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//	glVertexAttribBinding(0, 0);
//	// vertex normals
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//	glVertexAttribBinding(1, 0);
//	// vertex texture coords
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//	glVertexAttribBinding(2, 0);
//	// vertex tangent
//	glEnableVertexAttribArray(3);
//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
//	glVertexAttribBinding(3, 0);
//	// vertex bitangent
//	glEnableVertexAttribArray(4);
//	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
//	glVertexAttribBinding(4, 0);
//	glBindVertexArray(0);
//}

//
//void PStaticMesh::Deserialize(const char* path) {
//
//	int i = 0;
//
//	size_t vertexSize = 0, indexSize = 0;
//
//	std::ifstream in;
//	in.open(path, std::ios::binary);
//
//	//Vertex
//	in.read((char*)&vertexSize, sizeof(size_t));
//	vertices = VertexArray(vertexSize);
//
//	for (i = 0; i < vertexSize; i++) {
//		in.read((char*)&vertices[i], sizeof(Vertex));
//	}
//
//	//Index
//	in.read((char*)&indexSize, sizeof(size_t));
//	indices = std::vector<uint32>(indexSize);
//
//	for (i = 0; i < indexSize; i++) {
//		in.read((char*)&indices[i], sizeof(uint32));
//	}
//
//	in.close();
//
//}
//
//
//void PStaticMesh::Deserialize(UArchive &Ar) {
//
//	uint32 i = 0;
//
//	size_t vertexSize = 0, indexSize = 0;
//
//	//Vertex
//	Ar.ArchiveRead(&vertexSize, sizeof(size_t));
//	std::cout << vertexSize << std::endl;
//	vertices = VertexArray(vertexSize);
//
//	Ar.ArchiveRead(&vertices[0], sizeof(Vertex)*vertexSize);
//	//std::cout << vertices[0].Position.x << std::endl;
//	
//	//Index
//	Ar.ArchiveRead(&indexSize, sizeof(size_t));
//	indices = IndexArray(indexSize);
//
//	Ar.ArchiveRead(&indices[0], sizeof(uint32)*indexSize);
//	//std::cout << indices[2] << std::endl;
//
//}
//
//
//int VertexBufferObject::CreateVertexBuffer()
//{
//	glGenBuffers(1, &VBO);
//	// load data into vertex buffers
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	// A great thing about structs is that their memory layout is sequential for all its items.
//	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
//	// again translates to 3/2 floats which translates to a byte array.
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexArray), &vertices[0], GL_STATIC_DRAW);
//
//	// set the vertex attribute pointers
//	// vertex Positions
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//	glVertexAttribBinding(0, 0);
//	// vertex normals
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//	glVertexAttribBinding(1, 0);
//	// vertex texture coords
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//	glVertexAttribBinding(2, 0);
///*	// vertex tangent
//	glEnableVertexAttribArray(3);
//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
//	// vertex bitangent
//	glEnableVertexAttribArray(4);
//	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
//	*/
//	return 0;
//}
//
//int IndexBufferObject::CreateIndexBuffer()
//{
//	glGenBuffers(1, &EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), &indices[0], GL_STATIC_DRAW);
//
//	return 0;
//}



