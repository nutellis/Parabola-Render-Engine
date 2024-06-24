#include "Archive.h"


Archive::Archive()
{
	DefaultArchiveSize = 4096;
	Data = (byte*)malloc(sizeof(byte)*DefaultArchiveSize);
	ByteCount = 0;
}


Archive::~Archive()
{
	std::cout << "Deallocating Raw Data"<<std::endl;
	delete[] Data;
}

//polu kakos tropos man mou. Xreiazetai ftiaksimo!!
void Archive::ArchiveWrite(void * data, size_t  dataSize)
{
	if (ByteCount+dataSize > DefaultArchiveSize) {
		//realloc
		std::cout << "Reallocating..." << std::endl;
		byte *tempData = (byte*) realloc(Data,ByteCount+dataSize);

		if (tempData != NULL) {
			Data = tempData;
			std::cout << "Reallocation Succesfull" << std::endl;
		}
		else {
			std::cout << "Reallocation Failed" << std::endl;
		}
	}
	std::cout << "DATA SIZE = " << dataSize<<std::endl;
	memcpy(Data + ByteCount, data, dataSize);
	ByteCount += dataSize;

//	ArchiveReader();
	//std::cout << ByteCount << std::endl;
}

void Archive::ArchiveRead(void *data, size_t dataSize)
{

	memcpy(data, Data + ByteCount, dataSize);
	ByteCount += dataSize;
}

void Archive::Finalize(const char* path)
{
	std::ofstream out;
	out.open(path, std::ios::binary);
	out.write((char*)&this->ByteCount, sizeof(size_t));

	std::cout << " ByteCount: " << ByteCount << std::endl;


	out.write((char*)this->Data, sizeof(byte)*ByteCount);
	out.close();

}

void Archive::LoadToArchive(const char * path)
{
	std::ifstream in;
	in.open(path, std::ios::binary);
	in.read((char*)&this->ByteCount, sizeof(size_t));
	

	Data = (byte*)malloc(sizeof(byte)*ByteCount);

	in.read((char*)this->Data, sizeof(byte)*ByteCount);

	this->ByteCount = 0;
	in.close();

}
