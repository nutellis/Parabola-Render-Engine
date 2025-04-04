#pragma once

#include <Core/UDTypes.h>

#include <ParabolaMath.h>

class PChannel
{
public:
	const char* Name;

	Vector4f Colours;

	bool HasTexture;
	
	Texture *ChannelTexture;

public:
	PChannel();
	~PChannel();

	//void Serialize(Archive &Ar);
	//void Deserialize(Archive &Ar);
};

class PMaterial
{
public:
	PMaterial();
	~PMaterial();
	//PMaterial *Initialize(const FbxSurfaceMaterial * pMaterial);

	// Set material colors and binding diffuse texture if exists.
	//void SetCurrentMaterial() const;
	//Channel *GetMaterialProperty(const FbxSurfaceMaterial * pMaterial,
	//	const char * pPropertyName,
		//const char * pFactorPropertyName);

	//Texture *LoadTexture(const char* filename);
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
	Texture m_color_texture;
	Texture m_shininess_texture;
	Texture m_metalness_texture;
	Texture m_fresnel_texture;
	Texture m_emission_texture;
	
	*/

};