#include <RenderEngine\Components\Texture.h>

Texture::Texture()
{
	Width = 0;
	Height = 0;
	Format = GL_RGB;

	Target = GL_TEXTURE_2D;
	InternalFormat = GL_RGBA;
	Format = GL_RGBA;
	Type = GL_UNSIGNED_BYTE;
	FilterMin = GL_LINEAR_MIPMAP_LINEAR;
	FilterMax = GL_LINEAR;
	WrapS = GL_REPEAT;
	WrapT = GL_REPEAT;
	WrapR = GL_REPEAT;
	Mipmapping = TRUE;



}

Texture::~Texture()
{
}

void Texture::Generate(uint32 iNwidth, uint32 iNheight, uint32 inInternalFormat, uint32 InFormat, uint32 InType, void * InData)
{
	glGenTextures(1, &textureID);

	Width = iNwidth;
	Height = iNheight;
	//Depth = 0;
	InternalFormat = inInternalFormat;
	Format = InFormat;
	Type = InType;

	TextureImage;


	//assert(Target == GL_TEXTURE_2D);
	Bind();

	glTexImage2D(Target, 0, InternalFormat, Width, Height, 0, Format, Type, InData);
	
	glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, FilterMin);
	glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, FilterMax);

	glTexParameteri(Target, GL_TEXTURE_WRAP_S, WrapS);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, WrapT);
	
	if (Mipmapping)
	{
		glGenerateMipmap(Target);
	}
	Unbind();
}

void Texture::Bind(int32 Unit)
{
	if (Unit >= 0)
	{
		glActiveTexture(GL_TEXTURE0 + Unit);
	}
	glBindTexture(Target, textureID);
}

void Texture::Unbind()
{
	glBindTexture(Target, 0);
}

