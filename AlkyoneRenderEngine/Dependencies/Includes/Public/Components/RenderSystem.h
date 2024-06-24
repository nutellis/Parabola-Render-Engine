#pragma once

#include <Core/RenderCore.h>
#include <GL/glew.h>

#include <string>

#include <Managers/LogManager.h>

class RenderSystem
{
public:
	int MajorVersion, MinorVersion;
	uint8 CoreProfile : 1;
	const char * Version;
	const char * glslVersion;
	
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

		glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	}

	~RenderSystem(){}

	void Info() 
	{

		const uint8* renderer = glGetString(GL_RENDERER);
		const uint8* vendor = glGetString(GL_VENDOR);
		const uint8* version = glGetString(GL_VERSION);
		const uint8 *glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);

		glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

		LOG(INFO, "************************              ************************");
		LOG(INFO, "\t\t\t      GPU INFO START");
		LOG(INFO, "Vendor : %s", vendor);
		LOG(INFO, "Renderer : %s", renderer);
		LOG(INFO, "OpenGl version: %s", version);
		LOG(INFO, "GLSL version: %s", glsl);
		LOG(INFO, "\t\t\t      GPU  INFO  END");
		LOG(INFO, "************************              ************************\n");
	}

	void SetDepthTest(bool boolHasDepth)
	{
		if (boolHasDepth == true)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else glDisable(GL_DEPTH_TEST);

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

			glEnable(GL_DEPTH_TEST);
			LOG(INFO, "Depth Test = ENABLED");

			//glEnable(GL_CULL_FACE);
			//LOG(INFO, "Face Culling ENABLED");

			//glEnable(GL_STENCIL_TEST);
			//LOG(INFO, "Stencil Test ENABLED");

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			LOG(INFO, "OPENGL MESSAGE SYSTEM = ENABLED");
			glDebugMessageCallback(DebugCallback, nullptr);

			Info();
		}
	}

	void Terminate() 
	{}


	static void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{
		// filter too verbose messages
		switch (id)
		{
			// NVidia
		case 131185: // Buffer detailed info 'will use VIDEO memory as the source for buffer object'
		case 131218: // Program/shader state performance warning: Fragment Shader is going to be recompiled because the
					 // shader key based on GL state mismatches.
		case 131186: // Buffer performance warning: Buffer object (bound to GL_SHADER_STORAGE_BUFFER, and
					 // GL_SHADER_STORAGE_BUFFER (0), usage hint is GL_STATIC_DRAW) is being copied/moved from VIDEO memory
					 // to HOST memory.
			return;
		}

		// ignore push/pop
		if (source == GL_DEBUG_SOURCE_APPLICATION && type == GL_DEBUG_TYPE_PUSH_GROUP)
			return;
		if (source == GL_DEBUG_SOURCE_APPLICATION && type == GL_DEBUG_TYPE_POP_GROUP)
			return;

		const char* sSource;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			sSource = "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			sSource = "Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			sSource = "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			sSource = "3rd Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			sSource = "Application";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			sSource = "Other";
			break;
		default:
			sSource = "Unknown";
			break;
		}

		const char* sSeverity;
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			sSeverity = "High";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			sSeverity = "Medium";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			sSeverity = "Low";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			sSeverity = "Notification";
			break;
		default:
			sSeverity = "Unknown";
			break;
		}

		const char* sType;
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			sType = "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			sType = "Deprecated";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			sType = "Undefined";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			sType = "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			sType = "Performance";
			break;
		case GL_DEBUG_TYPE_MARKER:
			sType = "Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			sType = "Push";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			sType = "Pop";
			break;
		case GL_DEBUG_TYPE_OTHER:
			sType = "Other";
			break;
		default:
			sType = "Unknown";
			break;
		}

		if (std::strcmp(sSeverity, "Notification") == 0)
		{
			//LOG(INFO, "%s, %s, %d: %s\n", src_str, type_str, id, message);
		}
		else if (std::strcmp(sSeverity, "Low") == 0)
		{
			LOG(NOTICE, "%s, %s, %d: %s\n", sSource, sType, id, message);
		}
		else if (std::strcmp(sSeverity, "Medium") == 0)
		{
			LOG(WARNING, "%s:: %s, %s, %d: %s\n", sSeverity, sSource, sType, id, message);
		}
		else if (std::strcmp(sSeverity, "High") == 0)
		{
			LOG(WARNING, "%s:: %s, %s, %d: %s\n", sSeverity, sSource, sType, id, message);

			//std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
		}
	}

};


struct FramebufferInfo {

	uint32 Flags;
	bool boolMsaa : 1;

	FramebufferInfo()
	{
		//default
		Flags = GL_RGBA | GL_DOUBLE | GL_DEPTH;
		boolMsaa = 0;
	}
};

//TODO: check this debugging tool
/*
namespace debugging
{
bool DebugOverlay::mInitialized = false;
bool DebugOverlay::mVisible = false;
bool DebugOverlay::mSuppressed = false;

ImGuiTextBuffer DebugOverlay::mImguiBuffer;
ImGuiTextFilter DebugOverlay::mImguiFilter;
ImVector<int> DebugOverlay::mImguiLineOffsets;
bool DebugOverlay::mScrollToBottom = false;

void DebugOverlay::PrintErrorLine(const char* content)
{
	if (!mVisible && !mSuppressed)
		ToggleVisibility();

	auto oldSize = mImguiBuffer.size();
	mImguiBuffer.appendf("%s", content);

	for (auto newSize = mImguiBuffer.size(); oldSize < newSize; ++oldSize)
		if (mImguiBuffer[oldSize] == '\n')
			mImguiLineOffsets.push_back(oldSize + 1);

	mScrollToBottom = true;
}

void DebugOverlay::ToggleVisibility()
{
	mVisible = !mVisible;

	if (mVisible)
		mSuppressed = false;
}

void DebugOverlay::OnGui()
{
	assert(mInitialized);

	if (!mVisible)
		return;

	ImGui::SetNextWindowSize(ImVec2(850, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("OpenGL Debug Log");

	{
		if (ImGui::Button("Clear"))
			ClearErrorLog();

		ImGui::SameLine();

		if (ImGui::Button("Hide"))
			mVisible = false;

		ImGui::SameLine();

		if (ImGui::Button("Suppress"))
		{
			mVisible = false;
			mSuppressed = true;
		}

		ImGui::SameLine();
		mImguiFilter.Draw("Search", -100.f);
	}


	ImGui::Separator();

	{
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		auto bufBegin = mImguiBuffer.begin();
		auto bufEnd = mImguiBuffer.end();

		if (mImguiFilter.IsActive())
		{
			for (auto i = 0; i < mImguiLineOffsets.Size; ++i)
			{
				const char* line_start = bufBegin + mImguiLineOffsets[i];
				const char* line_end = (i + 1 < mImguiLineOffsets.Size) ? (bufBegin + mImguiLineOffsets[i + 1] - 1) : bufEnd;
				if (mImguiFilter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			ImGuiListClipper clipper;
			clipper.Begin(mImguiLineOffsets.Size);
			while (clipper.Step())
			{
				for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
				{
					const char* line_start = bufBegin + mImguiLineOffsets[i];
					const char* line_end = (i + 1 < mImguiLineOffsets.Size) ? (bufBegin + mImguiLineOffsets[i + 1] - 1) : bufEnd;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (mScrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		mScrollToBottom = false;

		ImGui::EndChild();
	}

	ImGui::End();
}

void DebugOverlay::ClearErrorLog()
{
	mImguiBuffer.clear();
	mImguiLineOffsets.clear();
	mImguiLineOffsets.push_back(0);
}

void DebugOverlay::Init()
{
	// Override OpenGL Debug Callback
	glDebugMessageCallback(OverrideDebugMessageCallback, nullptr);
	mInitialized = true;
}
}

*/