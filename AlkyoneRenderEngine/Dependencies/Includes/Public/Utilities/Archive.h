#pragma once

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <fstream>
#include <iostream>


typedef unsigned char byte ;


class Archive{
public:
	Archive();
	~Archive();
	void ArchiveWrite(void* data, size_t dataSize);
	void ArchiveRead(void *data, size_t dataSize);
	void Finalize(const char* path);
	void LoadToArchive(const char * path);



	byte *Data;
	//bool isSaveArchive;
//	bool isLoadArchive;
	size_t ByteCount;
	size_t DefaultArchiveSize;

	
};



#endif // !ARCHIVE_H
