#include <Managers/ShaderManager.h>


#include <Components/Shader.h>


#include <Managers/LogManager.h>

template<> GShaderManager* SingletonBase<GShaderManager>::instance = 0;
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

uint32 GShaderManager::CreateShader(const char* Name, const char* VertexPath, const char* FragmentPath, const char* GeometryPath = nullptr)
{
	const char* vShaderCode = FileUtilities::read_file(VertexPath);
	const char * fShaderCode = FileUtilities::read_file(FragmentPath);

	uint32 ID, VertexShader, FragmentShader, GeometryShader, bCompileGeometry = 0;

	// vertex shader
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vShaderCode, NULL);
	glCompileShader(VertexShader);
	CheckShaderCompileErrors(VertexShader, "VERTEX");
	// fragment Shader
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(FragmentShader);
	CheckShaderCompileErrors(FragmentShader, "FRAGMENT");

	// if geometry shader is given, compile geometry shader
	if (GeometryPath != nullptr)
	{
		bCompileGeometry = 1;
		const char * gShaderCode = FileUtilities::read_file(GeometryPath);
		GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(GeometryShader, 1, &gShaderCode, NULL);
		glCompileShader(GeometryShader);
		CheckShaderCompileErrors(GeometryShader, "GEOMETRY");
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
	CheckProgramCompileErrors(ID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessery

	glDetachShader(ID, VertexShader);
	glDeleteShader(VertexShader);

	glDetachShader(ID, FragmentShader);
	glDeleteShader(FragmentShader);

	if (bCompileGeometry) {
		glDetachShader(ID, GeometryShader);
		glDeleteShader(GeometryShader);
	}



	//Temporary solution. Need to find a better one!
	Shaders.PushBack(new Shader(ID, Name));

	return ID;
}

//void GShaderManager::AddShader(Shader* shader)
//{
//	s_Shaders.push_back(shader);
//}

Shader* GShaderManager::GetShader(const char* Name)
{
	Shader * FoundShader = Shaders.FindFirst([Name](const Shader* At) {
		return strcmp(At->Name, Name) == 0;
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

void GShaderManager::ReloadShader(const char* name)
{
	//for (int i = 0; i < s_Shaders.size(); i++)
	//{
	//	//if (s_Shaders[i]->GetName() == name)
	//	//{
	//		//String path = s_Shaders[i]->GetFilePath();
	//	//	String error;
	//	//	if (!Shader::TryCompileFromFile(path, error))
	//	{
	//		//SP_ERROR(error);
	//	}
	//	//else
	//	{
	//		s_Shaders[i]->~Shader();
	//		//s_Shaders[i] = API::Shader::CreateFromFile(name, path, s_Shaders[i]);
	//		//SP_INFO("Reloaded shader: " + name);
	//	}
	//	return;
	//	//}
	//}
	//SP_WARN("Could not find '", name, "' shader to reload.");
}

void GShaderManager::ReloadShader(const Shader* shader)
{
	//for (int i = 0; i < s_Shaders.size(); i++)
	//{
	//	if (s_Shaders[i] == shader)
	//	{
	//		//String name = shader->GetName();
	//	//	String path = shader->GetFilePath();
	//		s_Shaders[i]->~Shader();
	//		//	s_Shaders[i] = API::Shader::CreateFromFile(name, path, s_Shaders[i]);
	//		return;
	//	}
	//}
	//	SP_WARN("Could not find specified shader to reload.");
}

void GShaderManager::CheckShaderCompileErrors(GLuint shader, const char* type)
{
	int32 success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(shader, 1024, NULL, infoLog);
	if (!success)
	{
		LOG(ERROR, "SHADER COMPILATION ERROR OF TYPE: %s \n%s \n-- --------------------------------------------------- --", type, infoLog);
	}
	else
	{
		LOG(DEBUG, " %s SHADER COMPILED SUCCESSFULLY\n%s \n-- --------------------------------------------------- --", type, infoLog);
	}
}
void GShaderManager::CheckProgramCompileErrors(GLuint shader, const char* type)
{
	int32 success;
	char infoLog[1024];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	glGetProgramInfoLog(shader, 1024, NULL, infoLog);
	if (!success)
	{
		LOG(ERROR, "PROGRAM LINKING ERROR OF TYPE: %s \n%s \n-- --------------------------------------------------- --", type, infoLog);
	}
	else
	{
		LOG(DEBUG, " %s PROGRAM LINKED SUCCESSFULLY\n%s \n-- --------------------------------------------------- --", type, infoLog);
	}
}

void GShaderManager::Init()
{

	LOG(DEBUG, "Initiating SHADER_MANAGER\n");

	CreateShader("Light", "Q:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/lamp.vs", "Q:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/lamp.fs");
	
	//TEXT
	CreateShader("Text", "Q:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/TextShader.vs", "Q:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/TextFragShader.fs");


	//probably read some standard shaders that need to load. For now do nothing. hoho
	LOG(DEBUG, "SHADER_MANAGER INITIATED\n"); 
}

void GShaderManager::Terminate()
{

	LOG(DEBUG, "SHADER_MANAGER Terminated");
}
