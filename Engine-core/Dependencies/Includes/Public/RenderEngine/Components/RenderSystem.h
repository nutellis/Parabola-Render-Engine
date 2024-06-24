#pragma once

#include <Core/Core.h>
#include <GL/glew.h>

#include <string>

#include <Core/LogManager.h>

class RenderSystem
{
public:
	int MajorVersion, MinorVersion;
	uint8 CoreProfile : 1;
	const char * Version;
	
private:
	std::string InternalVersion;

public:

	RenderSystem()
	{
		MajorVersion = 4;
		MinorVersion = 5;
		CoreProfile = 1;

		InternalVersion = std::string("GL_VERSION_" + std::to_string(MajorVersion) + "_" + std::to_string(MinorVersion));
		
		Version = InternalVersion.c_str();
	}

	~RenderSystem(){}

	void Info() 
	{

		const uint8* renderer = glGetString(GL_RENDERER);
		const uint8* vendor = glGetString(GL_VENDOR);
		const uint8* version = glGetString(GL_VERSION);
		const uint8 *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

		LOG(INFO, "************************              ************************");
		LOG(INFO, "\t\t\t      GPU INFO START");
		LOG(INFO, "Vendor : %s", vendor);
		LOG(INFO, "Renderer : %s", renderer);
		LOG(INFO, "OpenGl version: %s", version);
		LOG(INFO, "GLSL version: %s", glslVersion);
		LOG(INFO, "\t\t\t      GPU  INFO  END");
		LOG(INFO, "************************              ************************\n");
	}

	void Init()
	{
		LOG(INFO, "INITIATING OPENGL");

		
		/*glewExperimental = GL_TRUE;
		LOG(INFO, "Experimental == TRUE");*/
		GLenum bInit = glewInit();
		if (bInit == GLEW_OK)
		{
			if (GLEW_ARB_direct_state_access)
			{
				LOG(INFO, "ARB Extension is supported");
			}

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			LOG(INFO, "OPENGL MESSAGE SYSTEM ENABLED");
			glDebugMessageCallback(DebugCallback, nullptr);

			Info();
		}
	}

	void Terminate() 
	{
		
	}
	
	static void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{
		auto const src_str = [source]() {
			switch (source)
			{
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			}
		}();

		auto const type_str = [type]() {
			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "MARKER";
			case GL_DEBUG_TYPE_OTHER: return "OTHER";
			}
		}();

		auto const severity_str = [severity]() {
			switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			}
		}();

		if (std::strcmp(severity_str, "NOTIFICATION") == 0)
		{
			//LOG(INFO, "%s, %s, %d: %s\n", src_str, type_str, id, message);
		}
		else if (std::strcmp(severity_str, "LOW") == 0)
		{
			LOG(NOTICE, "%s, %s, %d: %s\n", src_str, type_str, id, message);
		}
		
		if (std::strcmp(severity_str, "MEDIUM") == 0)
		{
			LOG(WARNING, "%s:: %s, %s, %d: %s\n", severity_str, src_str, type_str, id, message);
		}
		else if (std::strcmp(severity_str, "HIGH") == 0)
		{
			LOG(WARNING, "%s:: %s, %s, %d: %s\n", severity_str, src_str, type_str, id, message);
		}
		//std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
	}

};

struct FramebufferInfo {

	uint32 Flags;
	uint8 boolMsaa : 1;

	FramebufferInfo()
	{
		//default
		Flags = GL_RGBA | GL_DOUBLE | GL_DEPTH;
		boolMsaa = 0;
	}
};