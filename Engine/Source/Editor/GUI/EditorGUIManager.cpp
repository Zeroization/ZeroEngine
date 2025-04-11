#include "EditorGUIManager.h"

#ifdef ZERO_GRAPHIC_OPENGL
#include "EditorGUIManager_OpenGLImpl.h"
#endif

#include "Function/Render/Window/WindowManager.h"
#include "Panels/EditorGUIPanelManager.h"
#include "Widgets/EditorGUIWidgets.hpp"
#include "Core/FileSystem/FileSystem.h"
#include "imgui_internal.h"

#define CHECK_GUI_SHORTCUT(name, shortcut, doAction) \
ZERO_EXPLICIT_STATIC ImGuiKeyChord chordFor##name = shortcut; \
ZERO_EXPLICIT_STATIC bool isActiveFor##name = ImGui::GetShortcutRoutingData(chordFor##name)->RoutingCurr != ImGuiKeyOwner_NoOwner; \
if (!isActiveFor##name && ImGui::IsKeyChordPressed(chordFor##name)) \
{ \
	doAction; \
}

namespace ZeroEngine
{
	std::shared_ptr<EditorGUIManager> EditorGUIManager::Instance = nullptr;

	void EditorGUIManager::Create()
	{
#ifdef ZERO_GRAPHIC_OPENGL
		Instance = std::make_shared<EditorGUIManager_OpenGLImpl>();
#endif

		EditorGUIPanelManager::Create();
	}

	void EditorGUIManager::Destroy()
	{
		Instance.reset();
	}

	std::shared_ptr<EditorGUIManager> EditorGUIManager::GetInstance()
	{
		return Instance;
	}

	EditorGUIManager::EditorGUIManager()
	{
		ZERO_CORE_ASSERT(WindowManager::GetInstance(), "WindowPtr is nullptr, can't init ImGUI!")

		// 初始化ImGUI ====================================
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		// 设置输入相关
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 允许键盘输入
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // 允许控件Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // 允许控件出去到glfw窗口外
		// 设置控件受控制相关
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.ConfigWindowsResizeFromEdges = true;
		// 设置字体
		const std::string assetDir = (FileSystem::GetWorkingDir() / "BuiltinAssets").string();
		io.Fonts->AddFontFromFileTTF((assetDir + "/Fonts/OPPOSansM.ttf").c_str(), 18.0f, nullptr,
		                             io.Fonts->GetGlyphRangesChineseFull());
		// 设置主题
		// Ref: https://github.com/qiutang98/chord/blob/master/source/ui/ui.cpp
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.AntiAliasedLines = true;
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.WindowPadding = ImVec2(1, 1);
		style.FramePadding = ImVec2(6, 4);
		style.ItemSpacing = ImVec2(6, 2);
		style.ItemInnerSpacing = ImVec2(2.0f, 3.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.ScrollbarSize = 18;
		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = 1;
		style.TabBorderSize = 1;
		style.WindowRounding = 0;
		style.ChildRounding = 0;
		style.FrameRounding = 1;
		style.PopupRounding = 0;
		style.ScrollbarRounding = 0;
		style.GrabRounding = 0.0f;
		style.GrabMinSize = 8;
		style.LogSliderDeadzone = 0;
		style.TabRounding = 12;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.1f, 0.1f, 0.0f, 0.39f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 0.40f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.25f, 0.25f, 0.25f, 0.1f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
		style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	}

	void EditorGUIManager::Update()
	{
		CheckGUIShortcuts();
	}

	void EditorGUIManager::CheckGUIShortcuts()
	{
		CHECK_GUI_SHORTCUT(Undo, ImGuiKey_ModCtrl | ImGuiKey_Z, LOG_DEBUG("UNDO TODO"));
		CHECK_GUI_SHORTCUT(Redo, ImGuiKey_ModCtrl | ImGuiKey_Y, LOG_DEBUG("REDO TODO"));
	}

	void EditorGUIManager::Render()
	{
		BeforeGUIRender();

		GUIRender();

		AfterGUIRender();
	}

	void EditorGUIManager::GUIRender()
	{
		// TODO: 渲染各种控件和面板
		MainDockingWidget();

		ImGui::Render();
	}
} // ZeroEngine
