#pragma once


#include <RenderEngine/ImageLoader.h>
#include <RenderEngine/Core/RenderCore.h>

class Image
{
public:
	Image(const char * path, uint8 Flip)//uint32 newWidth, uint32 newHeight, uint32 newNrChannels
	{
		stbi_set_flip_vertically_on_load(Flip);
		Data = stbi_load(path, &Width, &Height, &NrChannels, 0);

	}
	Image(uint8 * inData, uint8 Flip)//uint32 newWidth, uint32 newHeight, uint32 newNrChannels
	{
		stbi_set_flip_vertically_on_load(Flip);
		Data = inData;
	}


	~Image()
	{
		stbi_image_free(Data);
	}

private:
	int32 Width;
	int32 Height;
	int32 NrChannels;

	uint8 * Data;
};

