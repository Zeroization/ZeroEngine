#include "EditorGUIContentBrowserPanel.h"

#include "imgui.h"

namespace ZeroEngine
{
    EditorGUIContentBrowserPanel::EditorGUIContentBrowserPanel()
    {
        mPanelTagName = "内容浏览器##EditorGUIContentBrowserPanel";
        mpIdentifier = std::make_unique<Identifier>(mPanelTagName);
    }

    void EditorGUIContentBrowserPanel::ImGuiRender()
    {
        ImGui::Begin(mPanelTagName.c_str());
        mbIsWindowFocused = ImGui::IsWindowFocused();


        ImGui::End();
    }

    void EditorGUIContentBrowserPanel::OnEvent(Event& e)
    {
    }
} // ZeroEngine
