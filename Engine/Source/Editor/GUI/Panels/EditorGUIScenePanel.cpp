#include "EditorGUIScenePanel.h"

#include "imgui.h"

namespace ZeroEngine
{
    EditorGUIScenePanel::EditorGUIScenePanel()
    {
        mPanelTagName = "场景层级##EditorGUIScenePanel";
        mpIdentifier = std::make_unique<Identifier>(mPanelTagName);
    }

    void EditorGUIScenePanel::ImGuiRender()
    {
        ImGui::Begin(mPanelTagName.c_str());
        mbIsWindowFocused = ImGui::IsWindowFocused();


        ImGui::End();
    }

    void EditorGUIScenePanel::OnEvent(Event& e)
    {
        // TODO
    }
} // ZeroEngine
