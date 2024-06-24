#ifndef STATICMESH_H
#define STATICMESH_H

#include <GL\glew.h> // holds all OpenGL type declarations


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <RenderEngine/shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <Object.h>
#include <Archive.h>


struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};


using VertexArray = std::vector<Vertex>;

using IndexArray = std::vector<uint32>;



struct VertexBufferObject {

	uint32 VBO;
	VertexArray vertices;

	int CreateVertexBuffer();
};

struct IndexBufferObject {

	IndexArray indices;
	uint32 EBO;

	int CreateIndexBuffer();
};



class StaticMesh : public PObject{
public:

	const char* path;
	IndexArray indices;
	VertexArray vertices;

	/*  StaticMesh Data  */
	VertexBufferObject VBuffer;

	IndexBufferObject IBuffer;
	
	//Material *materials;
	//vector<Texture> textures;
	uint32 VAO;

	/*  Functions  */
	// constructor
	StaticMesh(UArchive & Ar);
	~StaticMesh();
	//StaticMesh(vector<Vertex> vertices, vector<uint32> indices, vector<Texture> textures);
	// render the StaticMesh
	void Draw(Shader shader);

	void Deserialize(const char *path);

	void Deserialize(UArchive & Ar);

private:
	/*  Render data  */
	uint32 VBO, EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupStaticMesh();
};
#endif

