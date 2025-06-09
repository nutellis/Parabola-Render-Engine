#pragma once

#include <Core/UDTypes.h>

#include <ParabolaMath.h>

class PChannel
{
public:
	const char* Name;

	Vector4f Colours;

	bool HasTexture;
	
	RTexture *ChannelTexture;

public:
	PChannel();
	PChannel(const PChannel& other);
	PChannel(PChannel&& other) noexcept;
	~PChannel();

	PChannel& operator=(const PChannel& other);

	PChannel& operator=(PChannel&& other) noexcept;

	//void Serialize(Archive &Ar);
	//void Deserialize(Archive &Ar);
};

class PMaterial
{
public:
	PMaterial();
	PMaterial(const PMaterial& Other);
	PMaterial(PMaterial&& Other) noexcept;
	~PMaterial();
	PMaterial& operator=(const PMaterial& Other);
	PMaterial& operator=(PMaterial&& Other) noexcept;
	//PMaterial *Initialize(const FbxSurfaceMaterial * pMaterial);

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
	PChannel Diffuse;
	PChannel Metalness;
	PChannel Roughness;
	PChannel Fresnel;
	PChannel Emissive;
	PChannel Specular;
	PChannel Normal;
	float Transparency;
	float IOR;

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