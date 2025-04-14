#include "EditorGUIViewportPanel.h"

#include "imgui.h"

namespace ZeroEngine
{
    EditorGUIViewportPanel::EditorGUIViewportPanel()
    {
        mPanelTagName = "视口##EditorGUIViewportPanel";
        mpIdentifier = std::make_unique<Identifier>(mPanelTagName);
    }

    void EditorGUIViewportPanel::ImGuiRender()
    {
        ImGui::Begin(mPanelTagName.c_str());
        mbIsWindowFocused = ImGui::IsWindowFocused();


        ImGui::End();
    }

    void EditorGUIViewportPanel::OnEvent(Event& e)
    {
    }
} // ZeroEngine
