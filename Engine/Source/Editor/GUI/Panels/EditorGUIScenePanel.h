#pragma once

#include "EditorGUIBasePanel.h"

namespace ZeroEngine
{
    class EditorGUIScenePanel : public EditorGUIBasePanel
    {
    public:
        EditorGUIScenePanel();

        void ImGuiRender() override;
        void OnEvent(Event& e) override;
    };
} // ZeroEngine
