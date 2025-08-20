#pragma once

#include <Core/UDTypes.h>

#include <ParabolaMath.h>

class RChannel
{
public:
	const char* Name;

	Vector4f Colours;

	bool HasTexture;
	
	RTexture *ChannelTexture;

public:
	RChannel();
	RChannel(const RChannel& other);
	RChannel(RChannel&& other) noexcept;
	~RChannel();

	RChannel& operator=(const RChannel& other);

	RChannel& operator=(RChannel&& other) noexcept;

	//void Serialize(Archive &Ar);
	//void Deserialize(Archive &Ar);
};

class RMaterial
{
public:
	RMaterial();
	RMaterial(const RMaterial& Other);
	RMaterial(RMaterial&& Other) noexcept;
	~RMaterial();
	RMaterial& operator=(const RMaterial& Other);
	RMaterial& operator=(RMaterial&& Other) noexcept;
	//RMaterial *Initialize(const FbxSurfaceMaterial * pMaterial);

	// Set material colors and binding diffuse texture if exists.
	//void SetCurrentMaterial() const;
	//Channel *GetMaterialProperty(const FbxSurfaceMaterial * pMaterial,
	//	const char * pPropertyName,
		//const char * pFactorPropertyName);

	//RTexture *LoadTexture(const char* filename);
	//void Serialize(Archive &Ar);
	//void Deserialize(Archive &Ar);
	std::string Name;
	int32 MaterialID;

	std::string ShaderName;

//private:

	//TODO: Add roughness and more later
	RChannel Diffuse;
	RChannel Metalness;
	RChannel Roughness;
	RChannel Fresnel;
	RChannel Emissive;
	RChannel Specular;
	RChannel Normal;
	float Transparency;
	float IOR;
	bool HasTransparency;

	/*
	glm::vec3 m_color;
	float m_shininess;
	float m_metalness;
	float m_fresnel;
	glm::vec3 m_emission;
	float m_transparency;
	float m_ior;
	RTexture m_color_texture;
	RTexture m_shininess_texture;
	RTexture m_metalness_texture;
	RTexture m_fresnel_texture;
	RTexture m_emission_texture;
	
	*/

};