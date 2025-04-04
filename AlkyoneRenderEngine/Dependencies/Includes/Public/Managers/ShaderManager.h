#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <GL\glew.h>


#include <Core/SingletonBase.h>
#include <Managers/ManagerBase.h>

#include <Utilities/Containers/Array.h>

#include <Utilities/FileUtilities.h>

class Shader;


// typedef std::map<const char*, Shader*> ShaderMap;

class GShaderManager : public SingletonBase<GShaderManager>, ManagerBase
{
private:
	// ShaderMap s_Shaders;
	TArray<Shader *> Shaders;
public:

	static GShaderManager& getInstance();
	static GShaderManager * getInstancePtr();

	GShaderManager();
	~GShaderManager();

	Shader* CompileProgram(std::string Name, std::string VertexPath, std::string FragmentPath, std::string GeometryPath);

	//void AddShader(Shader* shader);

	Shader* GetShader(std::string Name);

	Shader* GetShader(const uint32 Id);
	
	
	void Clean();

	void ReloadShader(std::string name);
	void ReloadShader(const Shader* shader);





	virtual void Init() override;
	virtual void Terminate() override;

private:
	

	// utility functions for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------

	void CheckShaderCompileErrors(GLuint shader, const char* type, const char* ShaderName);
	void CheckProgramCompileErrors(GLuint shader, const char* type, const char* ShaderName);

	
	
};

#endif // !SHADERMANAGER_H
