#pragma once

#ifndef FBXIMPORTER_H
#define FBXIMPORTER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <Components.h>

class FBXImporter {

public:
	//Variables
	

	//Constructors
	FBXImporter();


	//Functions
	bool ReadFromFile(const char* path);
	std::string FetchData(std::stringstream& cleanUp);
	void CleanData(const char* CurrentLine);
	void PrintFile();

private:
	//Variables
	std::stringstream FileStream;
	std::string *keywords;
};


#endif // !FBXIMPORTER_H
