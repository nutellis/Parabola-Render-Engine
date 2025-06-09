#include <Components/Shader.h>

Shader::Shader()
{
}

Shader::Shader(
	uint32 InId, 
	std::string InName,
	std::string InVertexPath,
	std::string InFragmentPath)
	: 
	ID(InId), 
	Name(InName), 
	VertexPath(InVertexPath),
	FragmentPath(InFragmentPath) {}

void Shader::Enable()
{
	glUseProgram(ID);

	Uniforms.ModelLocation = glGetUniformLocation(ID, "model");
	Uniforms.ViewLocation = glGetUniformLocation(ID, "view");
	Uniforms.ProjectionLocation = glGetUniformLocation(ID, "projection");
}

void Shader::Disable() const
{
	glUseProgram(0);
}

void Shader::SetBool(const char* Name, bool Value) const
{
	glUniform1i(glGetUniformLocation(ID, Name), Value ? 1 : 0);
}
// ------------------------------------------------------------------------
void Shader::SetInt(const char* Name, int Value) const
{
	glUniform1i(glGetUniformLocation(ID, Name), Value);
}
// ------------------------------------------------------------------------
void Shader::SetFloat(const char* Name, float Value) const
{
	glUniform1f(glGetUniformLocation(ID, Name), Value);
}
// ------------------------------------------------------------------------
void Shader::SetVec2(const char* Name, const Vector2f &Value) const
{
	glUniform2fv(glGetUniformLocation(ID, Name), 1, &Value[0]);
}
void Shader::SetVec2(const char* Name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, Name), x, y);
}
// ------------------------------------------------------------------------
void Shader::SetVec3(const char* Name, const Vector3f &Value) const
{
	glUniform3fv(glGetUniformLocation(ID, Name), 1, &Value[0]);
}
void Shader::SetVec3(const char* Name, const uint32 Size, const Vector3f* Value) const
{
	glUniform3fv(glGetUniformLocation(ID, Name), Size, (float*)Value);
}
void Shader::SetVec3(const char* Name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, Name), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::SetVec4(const char* Name, const Vector4f &Value) const
{
	glUniform4fv(glGetUniformLocation(ID, Name), 1, &Value[0]);
}
void Shader::SetVec4(const char* Name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, Name), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::SetMat2(const char* Name, const glm::mat2 &Matrix) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, Name), 1, GL_FALSE, &Matrix[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat3(const char* Name, const glm::mat3 &Matrix) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, Name), 1, GL_FALSE, &Matrix[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat4(const char* Name, bool ShouldTranspose, const Matrix4f &Matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, Name), 1, ShouldTranspose, &Matrix[0][0]);
}

void Shader::SetMat4(uint32 Uniform, bool ShouldTranspose, const Matrix4f &Matrix) const
{
	glUniformMatrix4fv(Uniform, 1, ShouldTranspose, &Matrix[0][0]);
}

void Shader::SetMat4(const char* Name, bool ShouldTranspose, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, Name), 1, ShouldTranspose, &mat[0][0]);
}


std::string Shader::GetVertexPath() const
{
	return VertexPath;
}

std::string Shader::GetFragmentPath() const
{
	return FragmentPath;
}

