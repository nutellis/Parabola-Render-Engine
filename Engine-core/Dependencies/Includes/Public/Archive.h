#pragma once

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <fstream>
#include <iostream>
#include <Typedefs.h>

typedef uint8 byte;

//
class UArchive {
public:
	UArchive();
	~UArchive();
	void ArchiveWrite(void* data, size_t dataSize);
	void ArchiveRead(void *data, size_t dataSize);
	void Save(const char* path);
	void Load(const char * path);

	inline size_t size() { return ArchiveSize; }

	byte *Data;

	size_t ByteCount;
	size_t DefaultArchiveSize;
	size_t ArchiveSize;

};



#endif // !ARCHIVE_H
