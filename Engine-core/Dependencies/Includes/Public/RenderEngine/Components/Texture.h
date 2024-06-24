#pragma once


#include <RenderEngine/Core/RenderCore.h>

class Image;

class Texture {
public:
	Texture();
	~Texture();

	// 2D texture generation
	void Generate(uint32 iNwidth, uint32 iNheight, uint32 inInternalFormat, uint32 InFormat, uint32 InType, void* InData);

	void Bind(int32 Unit = -1);
	void Unbind();

	// update relevant texture state
	void SetWrapMode(uint32 WrapMode, uint8 Bind = FALSE);
	void SetFilterMin(uint32 Filter, uint8 Bind = FALSE);
	void SetFilterMax(uint32 Filter, uint8 Bind = FALSE);

public:


	uint32 textureID;
	uint32 Width;
	uint32 Height;

	Image * TextureImage;
	
	void SetupTextures();

private:

	uint32 Target;				// what type of texture we're dealing with
	uint32 InternalFormat;      // number of color components
	uint32 Format;				// the format each texel is stored in
	uint32 Type;
	uint32 FilterMin;			// what filter method to use during minification
	uint32 FilterMax;           // what filter method to use during magnification
	uint32 WrapS;				// wrapping method of the S coordinate
	uint32 WrapT;;				// wrapping method of the T coordinate
	uint32 WrapR;				// wrapping method of the R coordinate
	uint8 Mipmapping : 1;




};

