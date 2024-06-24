#pragma once

#include <RenderEngine/Core/RenderCore.h>
#include <ParabolaMath.h>


struct VertexAttributes 
{
	//uint32 
	//uint32 Size;
	//uint32 Stride;

};


class VertexBufferObject
{
public:

public:
	VertexBufferObject();
	~VertexBufferObject();

	void Bind();

	void CreateBuffer(int32 Size, const void * Data);

	void WriteBuffer(void * Data);

	void CopyBuffer(uint32 ReadBufferID, intptr ReadOffset = 0);

	uint32 ID;
	uint32 Index;


private:

};

class VertexArrayObject
{
public:
	VertexArrayObject();

	~VertexArrayObject();

	void Bind();

	void CreateArray();

	void BuffertoArray(void * data1, void * data2);

	void SetupArray(uint32 BufferIndex, uint32 Attribute, uint32 RelativeOffset);

	void BindBuffer(uint32 Buffer, uint32 Index, uint32 Stride);

	VertexBufferObject pos, col;
	
	uint32 ID;
private:
	
};


//TODO : move this to another file.


struct VertexFormat
{
public:
	// position
	Vector3f Position;
	// normal
	//Vector3f Normal;
	// texCoords
//	Vector2f TexCoords;

	//color
	Vector3f Color;

	// tangent
//	Vector3f Tangent;
	// bitangent
//	Vector3f Bitangent;
};

struct Index
{
	float VertexIndex;
};

//TEMP
struct Cube
{

	VertexFormat Vertices[4];
	Index Indices[36];

	Cube() {}



		//for (uint32 i = 0; i < 36; i)
		//{
		//	Indices[i].VertexIndex = vertex_indices[i]; 
		//}
	
	GLfloat vertex_positions[24] =
	{
	-0.25f, -0.25f, -0.25f,
	-0.25f, 0.25f, -0.25f,	
	0.25f, -0.25f, -0.25f,	
	0.25f, 0.25f, -0.25f,	
	0.25f, -0.25f, 0.25f,	
	0.25f, 0.25f, 0.25f,	
	-0.25f, -0.25f, 0.25f,	
	-0.25f, 0.25f, 0.25f
	};
	GLfloat vertex_color[24] =
	{
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 0.0f,
	};


	GLushort vertex_indices[36] =
	{
	0, 1, 2,
	2, 1, 3,
	2, 3, 4,
	4, 3, 5,
	4, 5, 6,
	6, 5, 7,
	6, 7, 0,
	0, 7, 1,
	6, 0, 2,
	2, 4, 6,
	7, 5, 3,
	7, 3, 1
	};

};




/*
an idea...

typedef ArrayBuffer<Vertex> VertexArrayBuffer;


template <typename BufferType>
class ArrayBuffer
{
public:
	BufferType  * Vertices;
	//more to come
public:
	ArrayBuffer();
	~ArrayBuffer();

	uint32 ID;
private:

};

*/