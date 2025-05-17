#pragma once

#include <Core/RenderCore.h>


#include <ParabolaMath.h>

#include <VertexFormat.h>


struct VertexAttributes 
{
	//uint32 
	//uint32 Size;
	//uint32 Stride;
};

struct PVertexComponentCount {
	uint32 Position = 3;
	uint32 Normal = 3;
	uint32 UVs = 2;
};


class VertexBufferObject
{
public:

public:
	VertexBufferObject();
	~VertexBufferObject();

	void Bind();

	void CreateBuffer(uint32 DrawType = GL_STATIC_DRAW, size_t Size = 0, const void* Data = nullptr);

	void WriteBuffer(void * Data);

	void CopyBuffer(uint32 ReadBufferID, ptr ReadOffset = 0);

	uint32 GetID();

	uint32 GetIndex();

private:
	uint32 ID;
	uint32 Index;

	bool bIsCreated;
};

class VertexArrayObject
{

public:
	PVertexComponentCount ComponentCount;

public:
	VertexArrayObject();

	~VertexArrayObject();

	void Bind();

	void CreateArray(PVertexComponentCount ComponentCount = PVertexComponentCount());

	void SetupAttribute(uint32 BufferIndex, uint32 Attribute, uint32 ComponentCount, uint32 RelativeOffset);

	void AttachVertexBuffer(uint32 Buffer, uint32 Index, uint32 Stride);

	void AttachElementBuffer(uint32 ElementBuffer);

	uint32 GetID();

private:
	uint8 bIsCreated : 1;
	uint32 ID;
};


////TODO : move this to another file... probably
//struct VertexFormat {
//	// position
//	Vector3f Position;
//	// normal
//	Vector3f Normal;
//	// texCoords
//	Vector2f UVs;
//	//color
//	Vector3f Color;
//	// bitangent
//	Vector3f Binormal;
//	// tangent
//	Vector3f Tangent;
//
//	bool operator==(const VertexFormat& other) const
//	{
//		return Position == other.Position && Normal == other.Normal && UVs == other.UVs && Color == other.Color  && Binormal == other.Binormal && Tangent == other.Tangent;
//	}
//};
//

// eeerm...
struct Index
{
	int32 VertexIndex;
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