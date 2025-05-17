#pragma once


#include <Vector2.h>	
#include <Vector3.h>	

#include <glm/glm.hpp>
#include <glm\gtx\hash.hpp>

//class VertexFormat2 {
//public:
//	// position
//	glm::vec3 Position;
//	// normal
//	glm::vec3 Normal;
//	// texCoords
//	glm::vec2 UVs;
//
//
//	bool operator==(const VertexFormat2& other) const
//	{
//		return Position == other.Position && Normal == other.Normal && UVs == other.UVs;
//	}
//};
//
//template<>
//struct std::hash<VertexFormat2>
//{
//	const size_t operator()(const VertexFormat2& key) const
//	{
//		return hash<glm::vec3>()(key.Position) ^ hash<glm::vec3>()(key.Normal) ^ hash<glm::vec2>()(key.UVs);
//	}
//};

#pragma pack(push,1)
struct VertexFormat {
public:
	// position
	Vector3f Position;
	// normal
	Vector3f Normal;
	// texCoords
	Vector2f UVs;
	//color
	//Vector3f Color;
	// bitangent
	//Vector3f Binormal;
	// tangent
	//Vector3f Tangent;

	VertexFormat() : Position(), Normal(), UVs() {}
	VertexFormat(Vector3f Pos) : Position(Pos), Normal(Vector3f(0, 1, 0)), UVs(Vector2f(0)) {}
	VertexFormat(Vector3f Pos, Vector3f Norm, Vector2f UV) : Position(Pos), Normal(Norm), UVs(UV) {}
	VertexFormat(const VertexFormat& Other): 
		Position(Other.Position),
		Normal(Other.Normal),
		UVs(Other.UVs) {

	}
	VertexFormat(VertexFormat&& Other) noexcept {
		*this = std::move(Other);
	}

	VertexFormat& operator=(const VertexFormat& Other) {
		return *this;
	}
	VertexFormat& operator=( VertexFormat&& Other) {
		if (this != &Other) {
			Position = Other.Position;
			Normal = Other.Normal;
			UVs = Other.UVs;
		}
		return *this;
	}
	bool operator==(const VertexFormat& other) const
	{
		return Position == other.Position && Normal == other.Normal && UVs == other.UVs /*&& Color == other.Color  && Binormal == other.Binormal && Tangent == other.Tangent*/;
	}
};
#pragma pack(pop)

template<>
struct std::hash<VertexFormat>
{
	const size_t operator()(const VertexFormat & key) const
	{

		return key.Position.Hash() ^ key.Normal.Hash() ^ key.UVs.Hash() /*^ key.Binormal.Hash() ^ key.Tangent.Hash()*/;
	}
};
