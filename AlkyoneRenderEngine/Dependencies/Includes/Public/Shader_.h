#pragma once
#ifndef SHADER_H
#define SHADER_H


#include <glm/glm.hpp>

#include <FileUtilities.h>
#include <iostream>
#include <Typedefs.h>
class Shader
{
private:
	uint32 ID;
	const char *vertexPath;
	const char *fragmentPath;

public:

	Shader(const char* vertPath, const char* fragPath, const char* geoPath = nullptr);
	Shader();
	~Shader();
	void enable() const;
	void disable() const;

	uint32 getShaderID() const;

	//Utility Uniform Funtions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string & name, int value) const;
	void setFloat(const std::string & name, float value) const;
	void setVec2(const std::string & name, const glm::vec2 & value) const;
	void setVec2(const std::string & name, float x, float y) const;
	void setVec3(const std::string & name, const glm::vec3 & value) const;
	void setVec3(const std::string & name, float x, float y, float z) const;
	void setVec4(const std::string & name, const glm::vec4 & value) const;
	void setVec4(const std::string & name, float x, float y, float z, float w) const;
	void setMat2(const std::string & name, const glm::mat2 & mat) const;
	void setMat3(const std::string & name, const glm::mat3 & mat) const;
	void setMat4(const std::string & name, const glm::mat4 & mat) const;

private:

	//void checkCompileErrors(GLuint &shader, std::string type);

};
#endif
