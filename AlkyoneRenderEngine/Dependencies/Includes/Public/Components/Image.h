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

	// Copy constructor
	Image(const Image& Other)
		: Width(Other.Width),
		Height(Other.Height),
		NrChannels(Other.NrChannels),
		IsHDR(Other.IsHDR)
	{
		if (IsHDR) {
			size_t dataSize = size_t(Width) * Height * NrChannels;
			DataFloat = nullptr;
			if (Other.DataFloat) {
				DataFloat = static_cast<float*>(stbi__malloc(dataSize * sizeof(float)));
				std::memcpy(DataFloat, Other.DataFloat, dataSize * sizeof(float));
			}
		}
		else {
			size_t dataSize = size_t(Width) * Height * NrChannels;
			DataUint8 = nullptr;
			if (Other.DataUint8) {
				DataUint8 = static_cast<uint8*>(stbi__malloc(dataSize * sizeof(uint8)));
				std::memcpy(DataUint8, Other.DataUint8, dataSize * sizeof(uint8));
			}
		}
	}

	// Move constructor
	Image(Image&& Other) noexcept
		: Width(Other.Width),
		Height(Other.Height),
		NrChannels(Other.NrChannels),
		IsHDR(Other.IsHDR)
	{
		if (IsHDR) {
			DataFloat = Other.DataFloat;
			Other.DataFloat = nullptr;
		}
		else {
			DataUint8 = Other.DataUint8;
			Other.DataUint8 = nullptr;
		}
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


	// Copy assignment
	Image& operator=(const Image& Other)
	{
		if (this == &Other) return *this;
		// Free existing data
		if (IsHDR && DataFloat) stbi_image_free(DataFloat);
		if (!IsHDR && DataUint8) stbi_image_free(DataUint8);

		Width = Other.Width;
		Height = Other.Height;
		NrChannels = Other.NrChannels;
		IsHDR = Other.IsHDR;

		if (IsHDR) {
			size_t dataSize = size_t(Width) * Height * NrChannels;
			DataFloat = nullptr;
			if (Other.DataFloat) {
				DataFloat = static_cast<float*>(stbi__malloc(dataSize * sizeof(float)));
				std::memcpy(DataFloat, Other.DataFloat, dataSize * sizeof(float));
			}
		}
		else {
			size_t dataSize = size_t(Width) * Height * NrChannels;
			DataUint8 = nullptr;
			if (Other.DataUint8) {
				DataUint8 = static_cast<uint8*>(stbi__malloc(dataSize * sizeof(uint8)));
				std::memcpy(DataUint8, Other.DataUint8, dataSize * sizeof(uint8));
			}
		}
		return *this;
	}

	// Move assignment
	Image& operator=(Image&& Other) noexcept
	{
		if (this == &Other) return *this;
		// Free existing data
		if (IsHDR && DataFloat) stbi_image_free(DataFloat);
		if (!IsHDR && DataUint8) stbi_image_free(DataUint8);

		Width = Other.Width;
		Height = Other.Height;
		NrChannels = Other.NrChannels;
		IsHDR = Other.IsHDR;

		if (IsHDR) {
			DataFloat = Other.DataFloat;
			Other.DataFloat = nullptr;
		}
		else {
			DataUint8 = Other.DataUint8;
			Other.DataUint8 = nullptr;
		}
		return *this;
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

