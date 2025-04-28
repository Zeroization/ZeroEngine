#include "EditorGUIOutlinerPanel.h"

#include "imgui.h"

namespace ZeroEngine
{
    EditorGUIOutlinerPanel::EditorGUIOutlinerPanel()
    {
        mPanelTagName = "细节查看器##EditorGUIOutliner";
        mpIdentifier = std::make_unique<Identifier>(mPanelTagName);
    }

    void EditorGUIOutlinerPanel::ImGuiRender()
    {
        ImGui::Begin(mPanelTagName.c_str());
        mbIsWindowFocused = ImGui::IsWindowFocused();


        ImGui::End();
    }

    void EditorGUIOutlinerPanel::OnEvent(Event& e)
    {
    }
} // ZeroEngine
