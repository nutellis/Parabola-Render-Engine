#pragma once
#ifndef _MATERIAL_
#define _MATERIAL_

#include <GL\glew.h>

#include <Object.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Archive.h>
#include <RenderEngine/Shader.h>
#include <vector>


struct Texture {
public:
	uint32 textureID = 0;
	uint32 width;
	uint32 height;
	uint32 format = GL_RGB;
	std::vector<byte> data;
	void SetupTextures();
	void Deserialize(UArchive & Ar);

	Texture() :textureID(0), data(0), width(0), height(0) {}
};


class Channel
{
public:
	const char* ChName;
	bool HasTexture;
	Texture *ch_Texture;
	glm::vec4 ch_Colour;
	
	Channel() :HasTexture(false), ch_Texture(), ch_Colour(glm::vec4()){}

	void SetupChannel();
	
	void SetShader(Shader s, uint32 &TextureSlot);

	void Serialize(UArchive &Ar);
	void Deserialize(UArchive &Ar);
};

class Material
{
public:
	Material();
	Material(UArchive & Ar);
	~Material();

	void SetupMaterial();

	// Set material colors and binding diffuse texture if exists.
	void SetActiveMaterial(Shader s);

	void Serialize(UArchive &Ar);
	void Deserialize(UArchive &Ar);

	bool isActive;

	uint32 TextureSlot;
	//private:
	
	Channel *mEmissive;
	Channel *mAmbient;
	Channel *mDiffuse;
	Channel *mSpecular;
	float mShinness;
};

#endif // !_MATERIAL_
