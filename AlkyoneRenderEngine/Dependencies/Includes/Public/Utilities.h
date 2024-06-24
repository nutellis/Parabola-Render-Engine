#pragma once

#include <Core/PODTypes.h>
#include <string>
#include <fstream>

static int32 ToInt(std::string String) {
	return std::stoi(String);
}

static float ToFloat(std::string String) {
	return std::stof(String);
}


