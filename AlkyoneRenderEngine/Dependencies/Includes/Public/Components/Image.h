#pragma once

#include <Core/RenderCore.h>


#include <Utilities/ImageLoader.h>
#include <Managers/LogManager.h>


class Image
{
public:
	Image(const char * path, uint8 Flip, bool isHDRImage)//uint32 newWidth, uint32 newHeight, uint32 newNrChannels
	{
		IsHDR = isHDRImage;
		stbi_set_flip_vertically_on_load(Flip);
		if (IsHDR) {
			DataFloat = stbi_loadf(path, &Width, &Height, &NrChannels, 0);
			if (DataFloat == nullptr)
			{
				LOG(DEBUG, "Failed to load image : %s.\n", path);
			//	exit(1);
			}
		}
		else {
			DataUint8 = stbi_load(path, &Width, &Height, &NrChannels, 0);
			if (DataUint8 == nullptr)
			{
				LOG(DEBUG, "Failed to load image : %s.\n", path);
			//	exit(1);
			}
		}
	}
	Image(void * inData, uint8 Flip)//uint32 newWidth, uint32 newHeight, uint32 newNrChannels
	{
		IsHDR = false;
		stbi_set_flip_vertically_on_load(Flip);
		DataUint8 = static_cast<uint8 *>(inData);
	}


	~Image()
	{
		if (IsHDR) {
			stbi_image_free(DataFloat);
		}
		else {
			stbi_image_free(DataUint8);
		}
		
	}

	inline uint8 * GetDataUint8() { return DataUint8; }
	inline float* GetDataFloat() { return DataFloat; }

	int32 Width;
	int32 Height;
	int32 NrChannels;

	bool IsHDR;
private:
	union {
		uint8* DataUint8;
		float* DataFloat;
	};	
};

