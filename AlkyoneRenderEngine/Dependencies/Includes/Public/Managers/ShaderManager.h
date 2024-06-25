#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <GL\glew.h>


#include <Core/SingletonBase.h>
#include <Managers/ManagerBase.h>

#include <Utilities/Containers/Array.h>

#include <FileUtilities.h>

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

	uint32 CreateShader(const char* Name, const char* VertexPath, const char* FragmentPath, const char* GeometryPath);


	//void AddShader(Shader* shader);


	Shader* GetShader(const char* Name);

	Shader* GetShader(const uint32 Id);
	
	
	void Clean();

	void ReloadShader(const char* name);
	void ReloadShader(const Shader* shader);



	virtual void Init() override;
	virtual void Terminate() override;

private:
	

	// utility functions for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------

	void CheckShaderCompileErrors(GLuint shader, const char* type);
	void CheckProgramCompileErrors(GLuint shader, const char* type);

	
	
};

#endif // !SHADERMANAGER_H
