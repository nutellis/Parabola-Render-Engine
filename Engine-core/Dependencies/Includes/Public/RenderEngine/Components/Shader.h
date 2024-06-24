#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <glm/glm.hpp>


#include <ParabolaMath.h>

class Shader
{
public:
	uint32 ID;
	//const char* Name;

	Shader();
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(uint32 Id);

	// activate the shader
	// ------------------------------------------------------------------------
	void enable() const;
	void disable() const;

	//void setMaterial(Material *material);

	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const char* name, bool value) const;
	// ------------------------------------------------------------------------
	void setInt(const char* name, int value) const;
	// ------------------------------------------------------------------------
	void setFloat(const char*name, float value) const;
	// ------------------------------------------------------------------------
	void setVec2(const char*name, const Vector2f &value) const;
	void setVec2(const char*name, float x, float y) const;
	// ------------------------------------------------------------------------
	void setVec3(const char*name, const Vector3f &value) const;
	void setVec3(const char*name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void setVec4(const char*name, const Vector4f &value) const;
	void setVec4(const char*name, float x, float y, float z, float w);
	// ------------------------------------------------------------------------
	void setMat2(const char*name, const glm::mat2 &mat) const;
	// ------------------------------------------------------------------------
	void setMat3(const char*name, const glm::mat3 &mat) const;
	// ------------------------------------------------------------------------
	void setMat4(const char*name, const glm::mat4 &mat) const;

};

#endif
