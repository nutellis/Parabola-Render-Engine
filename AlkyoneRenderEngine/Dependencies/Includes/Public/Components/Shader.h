#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <glm/glm.hpp>


#include <ParabolaMath.h>

struct ShaderUniforms {
	uint32 ModelLocation;
	uint32 ViewLocation;
	uint32 ProjectionLocation;
};

class Shader
{
public:
	uint32 ID;
	
	const char* Name;

	ShaderUniforms Uniforms;

	Shader();
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(uint32 Id, const char* Name);

	// activate the shader
	// ------------------------------------------------------------------------
	void Enable();
	void disable() const;

	//void setMaterial(PMaterial *material);

	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const char* Name, bool Value) const;
	// ------------------------------------------------------------------------
	void setInt(const char* Name, int Value) const;
	// ------------------------------------------------------------------------
	void setFloat(const char*Name, float Value) const;
	// ------------------------------------------------------------------------
	void setVec2(const char*Name, const Vector2f &Value) const;
	void setVec2(const char*Name, float x, float y) const;
	// ------------------------------------------------------------------------
	void setVec3(const char*Name, const Vector3f &Value) const;
	void setVec3(const char*Name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void setVec4(const char*Name, const Vector4f &Value) const;
	void setVec4(const char*Name, float x, float y, float z, float w);
	// ------------------------------------------------------------------------
	void setMat2(const char*Name, const glm::mat2 &Matrix) const;
	// ------------------------------------------------------------------------
	void setMat3(const char*Name, const glm::mat3 &Matrix) const;

	
	// ------------------------------------------------------------------------
	void SetMat4(const char* Name, bool ShouldTranspose, const glm::mat4& mat) const;
	void SetMat4(const char* Name, bool ShouldTranspose, const Matrix4f& Matrix) const;
	void SetMat4(uint32 Uniform, bool ShouldTranspose, const Matrix4f& Matrix) const;

};

#endif
