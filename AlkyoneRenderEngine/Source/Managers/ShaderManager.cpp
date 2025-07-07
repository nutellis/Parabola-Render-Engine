#include <Managers/ShaderManager.h>


#include <Components/Shader.h>


#include <Managers/LogManager.h>

template<> GShaderManager* SingletonManagerBase<GShaderManager>::instance = 0;
GShaderManager & GShaderManager::getInstance()
{
	//assert?
	return (*instance);
}

GShaderManager * GShaderManager::getInstancePtr()
{
	if (instance == nullptr)
	{
		instance = new GShaderManager();
	}
	return instance;
}

GShaderManager::GShaderManager()
{}

GShaderManager::~GShaderManager()
{}

Shader * GShaderManager::CompileProgram(std::string Name, std::string VertexPath, std::string FragmentPath, std::string GeometryPath = "")
{
    std::string vShaderCodeStr = FileUtilities::ReadFilePath(VertexPath);
    const char* vShaderCode = vShaderCodeStr.c_str();
    std::string fShaderCodeStr = FileUtilities::ReadFilePath(FragmentPath);
    const char* fShaderCode = fShaderCodeStr.c_str();
	uint32 ID, VertexShader, FragmentShader, GeometryShader, bCompileGeometry = 0;

	// vertex shader
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vShaderCode, NULL);
	glCompileShader(VertexShader);
	CheckShaderCompileErrors(VertexShader, "VERTEX", Name.c_str());
	// fragment Shader
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(FragmentShader);
	CheckShaderCompileErrors(FragmentShader, "FRAGMENT", Name.c_str());

	// if geometry shader is given, compile geometry shader
	if (GeometryPath.empty() == false)
	{
		bCompileGeometry = 1;
		const char * gShaderCode = FileUtilities::ReadFilePath(GeometryPath).c_str();
		GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(GeometryShader, 1, &gShaderCode, NULL);
		glCompileShader(GeometryShader);
		CheckShaderCompileErrors(GeometryShader, "GEOMETRY", Name.c_str());
	}
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, VertexShader);
	glAttachShader(ID, FragmentShader);
	if (bCompileGeometry)
	{
		glAttachShader(ID, GeometryShader);
	}

	glLinkProgram(ID);
	CheckProgramCompileErrors(ID, "PROGRAM", Name.c_str());

	// delete the shaders as they're linked into our program now and no longer necessery

	glDetachShader(ID, VertexShader);
	glDeleteShader(VertexShader);

	glDetachShader(ID, FragmentShader);
	glDeleteShader(FragmentShader);

	if (bCompileGeometry) {
		glDetachShader(ID, GeometryShader);
		glDeleteShader(GeometryShader);
	}

	Shader* shader = new Shader(ID, Name, VertexPath, FragmentPath);

	return shader;
}


Shader* GShaderManager::GetShader(std::string Name)
{
	Shader * FoundShader = Shaders.FindFirst([Name](const Shader* At) {
		return At->Name == Name;
		});

	return FoundShader;
}

Shader* GShaderManager::GetShader(const uint32 Id)
{

	Shader* FoundShader = Shaders.FindFirst([Id](const Shader* At) {
		return At->ID == Id;
		});

	return FoundShader;
}

void GShaderManager::Clean()
{
	for (int i = 0; i < Shaders.Size(); i++) {}
	//spdel s_Shaders[i];
}

void GShaderManager::ReloadShader(std::string name)
{

	for (Shader * shader : Shaders) {
		if (shader->Name == name) {
			Shader* ReloadedShader = CompileProgram(shader->Name, shader->GetVertexPath(), shader->GetFragmentPath());
			if (ReloadedShader->ID != -1) {
				shader = Utilities::Move(ReloadedShader);
			}
			else {
				LOG(ERROR, "Failed to reload shader: %s", name.c_str());
			}
		}
	}

}

void GShaderManager::ReloadShaders()
{
	for (uint32 i = 0; i < Shaders.Size(); ++i) {
		Shader* shader = Shaders[i];
		Shader* ReloadedShader = CompileProgram(shader->Name, shader->GetVertexPath(), shader->GetFragmentPath());
		if (ReloadedShader->ID != -1) {
			delete shader; // Clean up the old shader
			Shaders[i] = ReloadedShader; // Replace with new one
		}
		else {
			LOG(ERROR, "Failed to reload shader: %s", shader->Name.c_str());
		}
	}
}

void GShaderManager::CheckShaderCompileErrors(GLuint shader, const char* type, const char * ShaderName)
{
	int32 success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(shader, 1024, NULL, infoLog);
	if (!success)
	{
		LOG(ERROR, "SHADER COMPILATION ERROR OF %s ON: %s \n%s \n-- --------------------------------------------------- --", type, ShaderName, infoLog);
	}
	else
	{
		LOG(DEBUG, " %s SHADER COMPILED SUCCESSFULLY\n%s \n-- --------------------------------------------------- --", type, infoLog);
	}
}
void GShaderManager::CheckProgramCompileErrors(GLuint shader, const char* type, const char* ShaderName)
{
	int32 success;
	char infoLog[1024];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	glGetProgramInfoLog(shader, 1024, NULL, infoLog);
	if (!success)
	{
		LOG(ERROR, "PROGRAM LINKING ERROR OF TYPE: %s on Program %s \n %s-- --------------------------------------------------- --", type, ShaderName, infoLog);
		__debugbreak();
	}
	else
	{
		LOG(DEBUG, " %s PROGRAM LINKED SUCCESSFULLY\n%s \n-- --------------------------------------------------- --", type, infoLog);
	}
}

void GShaderManager::Init()
{

	LOG(DEBUG, "Initiating SHADER_MANAGER\n");


	Shader * BrdfShader = CompileProgram("BRDF_Default", "Shaders/brdf_shading.vert", "Shaders/brdf_shading.frag");
	if (BrdfShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile BRDF_Default Shader\n");
	}
	else
	{
		Shaders.PushBack(BrdfShader);
	}

	//Sky
	Shader* SkyBoxShader = CompileProgram("SkyBoxShader", "Shaders/skybox.vert", "Shaders/skybox.frag");
	if (SkyBoxShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile SkyBox Shader\n");
	}
	else
	{
		Shaders.PushBack(SkyBoxShader);
	}

	//Depth
	Shader* DepthShader = CompileProgram("DepthShader", "Shaders/position.vert", "Shaders/depth.frag");
	if (DepthShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile Depth Shader \n");
	}
	else
	{
		Shaders.PushBack(DepthShader);
	}

	// Cascade Shadow Map Debug
	Shader* CSMDebugShader = CompileProgram("CSM_Debug", "Shaders/cascade_debug.vert", "Shaders/cascade_debug.frag");
	if (CSMDebugShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile CSM_Debug Shader\n");
	}
	else
	{
		Shaders.PushBack(CSMDebugShader);
	}

	// SSAO
	Shader* SSAOInputShader = CompileProgram("SSAOInputShader", "Shaders/position.vert", "Shaders/ssaoInput.frag");
	if (SSAOInputShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile SSAO Input Shader\n");
	}
	else
	{
		Shaders.PushBack(SSAOInputShader);
	}

	Shader* SSAOOutputShader = CompileProgram("SSAOOutputShader", "Shaders/ssaoOutput.vert", "Shaders/ssaoOutput.frag");
	if (SSAOOutputShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile SSAO Output Shader\n");
	}
	else
	{
		Shaders.PushBack(SSAOOutputShader);
	}

	Shader* SSAOBlurShader = CompileProgram("SSAOBlurShader", "Shaders/ssaoOutput.vert", "Shaders/ssaoBlur.frag");
	if (SSAOBlurShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile SSAO Blur Shader\n");
	}
	else
	{
		Shaders.PushBack(SSAOBlurShader);
	}

	// Linear Depth
	Shader* LinearDepthShader = CompileProgram("LinearDepthShader", "Shaders/position.vert", "Shaders/linearDepth.frag");
	if (LinearDepthShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile Linear Depth Shader\n");
	}
	else
	{
		Shaders.PushBack(LinearDepthShader);
	}
	// Normal Reconstruction
	Shader* ReconstructNormalShader = CompileProgram("ReconstructNormalShader", "Shaders/ssaoOutput.vert", "Shaders/reconstructNormal.frag");
	if (ReconstructNormalShader->ID == 0)
	{
		LOG(ERROR, "Failed to compileReconstruct Normal Shader\n");
	}
	else
	{
		Shaders.PushBack(ReconstructNormalShader);
	}
	// HBAO
	Shader* HBAOOutputShader = CompileProgram("HBAOOutputShader", "Shaders/ssaoOutput.vert", "Shaders/hbaoOutput.frag");
	if (HBAOOutputShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile HBAO+ Output Shader\n");
	}
	else
	{
		Shaders.PushBack(HBAOOutputShader);
	}

	Shader* LightShader = CompileProgram("SimpleShader", "Shaders/position.vert", "Shaders/colour.frag");
	if (LightShader->ID == 0)
	{
		LOG(ERROR, "Failed to compile Simple Shader\n");
	}
	else
	{
		Shaders.PushBack(LightShader);
	}

	//probably read some standard shaders that need to load. For now do nothing. hoho
	LOG(DEBUG, "SHADER_MANAGER INITIATED\n"); 
}

void GShaderManager::Terminate()
{
	Shaders.~TArray();

	LOG(DEBUG, "SHADER_MANAGER Terminated");
}
