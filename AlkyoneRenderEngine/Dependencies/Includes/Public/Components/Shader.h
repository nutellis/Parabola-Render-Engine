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
private:
	std::string VertexPath;
	std::string FragmentPath;

public:
	uint32 ID;
	
	std::string Name;

	ShaderUniforms Uniforms;

	Shader();

	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(uint32 InId, std::string InName, std::string VertexPath, std::string FragmentPath);

	// activate the shader
	// ------------------------------------------------------------------------
	void Enable();
	void Disable() const;

	//void setMaterial(PMaterial *material);

	// utility uniform functions
	// ------------------------------------------------------------------------
	void SetBool(const char* Name, bool Value) const;
	// ------------------------------------------------------------------------
	void SetInt(const char* Name, int Value) const;
	// ------------------------------------------------------------------------
	void SetFloat(const char*Name, float Value) const;
	// ------------------------------------------------------------------------
	void SetVec2(const char*Name, const Vector2f &Value) const;
	void SetVec2(const char*Name, float x, float y) const;
	// ------------------------------------------------------------------------
	void SetVec3(const char*Name, const Vector3f &Value) const;
	void SetVec3(const char* Name, const uint32 Size, const Vector3f* Value) const;
	void SetVec3(const char*Name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void SetVec4(const char*Name, const Vector4f &Value) const;
	void SetVec4(const char*Name, float x, float y, float z, float w);
	// ------------------------------------------------------------------------
	void SetMat2(const char*Name, const glm::mat2 &Matrix) const;
	// ------------------------------------------------------------------------
	void SetMat3(const char*Name, const glm::mat3 &Matrix) const;

	
	// ------------------------------------------------------------------------
	void SetMat4(const char* Name, bool ShouldTranspose, const glm::mat4& mat) const;
	void SetMat4(const char* Name, bool ShouldTranspose, const Matrix4f& Matrix) const;
	void SetMat4(uint32 Uniform, bool ShouldTranspose, const Matrix4f& Matrix) const;


	std::string GetVertexPath() const;
	std::string GetFragmentPath() const;
};

#endif
