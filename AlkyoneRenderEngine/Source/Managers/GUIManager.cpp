#include <Managers\GUIManager.h>

#include <Managers/WindowManager.h>
#include <Components/FBORenderTarget.h>
#include <Components/Scene.h>
#include <Components/NodeSystem/SceneNode.h>


#include <Managers/SceneManager.h>

#include <Core/Editor.h>

template<> GGUIManager* SingletonBase<GGUIManager>::instance = 0;
GGUIManager & GGUIManager::getInstance()
{
	//assert?
	return (*instance);
}

GGUIManager * GGUIManager::getInstancePtr()
{
	if (instance == nullptr)
	{
		instance = new GGUIManager();
	}
	return instance;
}


GGUIManager::GGUIManager()
{
	RenderTarget = new FBORenderTarget(1280,720);
}

GGUIManager::~GGUIManager()
{
	RenderTarget = nullptr;
}

void GGUIManager::Draw()
{

	bool open = true;


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();


	DrawLog("Editor Log", &open);

	if (GEditor::getInstancePtr() != nullptr) {
		DrawEditor();
		gSceneManager.DrawSceneGraph();
	}

	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GGUIManager::BindRenderTarget() {
	RenderTarget->Bind();
}
FBORenderTarget * GGUIManager::GetRenderTarget() {
	return RenderTarget;
}


void GGUIManager::Init()
{

	//Init imGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO & io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(gWindowManager.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 450");
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::StyleColorsDark();

	RenderTarget->Init();
}

void GGUIManager::Terminate()
{

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}


void GGUIManager::DrawLog(const char* title, bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1000, 250));
	ImGui::SetNextWindowPos(ImVec2(0, 830));
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;

	ImGuiTextFilter     Filter;
	bool AutoScroll = true;  // Keep scrolling if already at the bottom.

	if (!ImGui::Begin(title, p_open, window_flags))
	{
		ImGui::End();
		return;
	}


	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
	ImGui::Separator();
	ImGui::Spacing();
	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &AutoScroll);
		ImGui::EndPopup();
	}

	// Main window
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", 180);

	ImGui::Separator();
	
	// Reserve enough left-over height for 1 separator + 1 input text
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	if (clear)
		gLogManager.EditorLog.Clear();
	if (copy)
		ImGui::LogToClipboard();

	//ImGuiListClipper clipper;
	//clipper.Begin(LogGUI.Items.Size);
	//while (clipper.Step())
	//{
		//for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		for (auto& logItem: gLogManager.EditorLog.Items)
	
		{
			if (!Filter.PassFilter(logItem.c_str()))
				continue;

			// Normally you would store more information in your item than just a string.
			// (e.g. make Items[] an array of structure, store color/type etc.)
			ImVec4 color;
			bool has_color = false;
			if (strstr(logItem.c_str(), "[ERROR]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
			else if (strstr(logItem.c_str(), "[WARNING]")) { color = ImVec4(1.0f, 1.0f, 0.19f, 1.0f); has_color = true; }
			else if (strstr(logItem.c_str(), "[DEBUG]")) { color = ImVec4(0.1f, 1.0f, 0.1f, 1.0f); has_color = true; }
			if (has_color)
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(logItem.c_str());
			if (has_color)
				ImGui::PopStyleColor();
		}
	//}
	//clipper.End();

	/*if (Filter.IsActive())
	{
		// In this example we don't use the clipper when Filter is enabled.
		// This is because we don't have a random access on the result on our filter.
		// A real application processing logs with ten of thousands of entries may want to store the result of
		// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
		for (int line_no = 0; line_no < LogGUI.LineOffsets.Size; line_no++)
		{
			const char* line_start = buf + LogGUI.LineOffsets[line_no];
			const char* line_end = (line_no + 1 < LogGUI.LineOffsets.Size) ? (buf + LogGUI.LineOffsets[line_no + 1] - 1) : buf_end;
			if (Filter.PassFilter(line_start, line_end))
				ImGui::TextUnformatted(line_start, line_end);
		}
	}
	else
	{
		// The simplest and easy way to display the entire buffer:
		//   ImGui::TextUnformatted(buf_begin, buf_end);
		// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
		// to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
		// within the visible area.
		// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
		// on your side is recommended. Using ImGuiListClipper requires
		// - A) random access into your data
		// - B) items all being the  same height,
		// both of which we can handle since we an array pointing to the beginning of each line of text.
		// When using the filter (in the block of code above) we don't have random access into the data to display
		// anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
		// it possible (and would be recommended if you want to search through tens of thousands of entries).
		ImGuiListClipper clipper;
		clipper.Begin(LogGUI.LineOffsets.Size);
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_start = buf + LogGUI.LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LogGUI.LineOffsets.Size) ? (buf + LogGUI.LineOffsets[line_no + 1] - 1) : buf_end;
				ImGui::TextUnformatted(line_start, line_end);
			}
		}
		clipper.End();
	}*/
	
	if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::PopStyleVar();

	

	ImGui::EndChild();
	ImGui::End();
}

void GGUIManager::DrawEditor()
{
	if (RenderTarget != nullptr) {
		ImGui::SetNextWindowSize(ImVec2(RenderTarget->Width, RenderTarget->Height));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("Editor", 0, window_flags);
		{
			ImGui::Image(
				(ImTextureID)RenderTarget->GetTexture(),
				ImGui::GetContentRegionAvail(),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
		}
		ImGui::End();
	}
}