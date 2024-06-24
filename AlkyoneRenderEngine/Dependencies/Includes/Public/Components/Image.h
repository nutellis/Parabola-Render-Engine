#pragma once

#include <Core/RenderCore.h>


#include <Utilities/ImageLoader.h>

class Image
{
public:
	Image(const char * path, uint8 Flip)//uint32 newWidth, uint32 newHeight, uint32 newNrChannels
	{
		stbi_set_flip_vertically_on_load(Flip);
		Data = stbi_load(path, &Width, &Height, &NrChannels, 0);

	}
	Image(void * inData, uint8 Flip)//uint32 newWidth, uint32 newHeight, uint32 newNrChannels
	{
		stbi_set_flip_vertically_on_load(Flip);
		Data = static_cast<uint8 *>(inData);
	}


	~Image()
	{
		stbi_image_free(Data);
	}

	inline uint8 * GetData() { return Data; }

	int32 Width;
	int32 Height;
	int32 NrChannels;
private:
	uint8 * Data;
	
};

