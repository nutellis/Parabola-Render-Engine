#include "Archive.h"


UArchive::UArchive()
{
	DefaultArchiveSize = 4096;

	ByteCount = 0;
	ArchiveSize = 0;

	Data = (byte*)malloc(sizeof(byte)*ArchiveSize);
	
}


UArchive::~UArchive()
{
	std::cout << "Deallocating Raw Data" << std::endl;
	delete[] Data;
}

void UArchive::ArchiveWrite(void * data, size_t  dataSize)
{
	if (ByteCount + dataSize > DefaultArchiveSize) {
		//realloc
		std::cout << "Reallocating..." << std::endl;
		byte *tempData = (byte*)realloc(Data, ByteCount + dataSize);

		if (tempData != NULL) {
			Data = tempData;
			std::cout << "Reallocation Succesfull" << std::endl;
		}
		else {
			std::cout << "Reallocation Failed" << std::endl;
		}
	}
	memcpy(Data + ByteCount, data, dataSize);
	ByteCount += dataSize;

	//	ArchiveReader();
	//std::cout << ByteCount << std::endl;
}

void UArchive::ArchiveRead(void *data, size_t dataSize)
{

	memcpy(data, Data + ByteCount, dataSize);
	ByteCount += dataSize;
}

void UArchive::Save(const char* path)
{
	std::ofstream out;
	out.open(path, std::ios::binary);
	out.write((char*)&this->ByteCount, sizeof(size_t));
	out.write((char*)this->Data, sizeof(byte)*ByteCount);
	out.close();

}

void UArchive::Load(const char * path)
{
	std::ifstream in;
	in.open(path, std::ios::binary);
	in.read((char*)&this->ArchiveSize, sizeof(size_t));

	Data = (byte*)malloc(sizeof(byte)*ArchiveSize);

	in.read((char*)this->Data, sizeof(byte)*ArchiveSize);
	
	in.close();
}
