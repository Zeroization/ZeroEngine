#pragma once

#include "EditorGUIBasePanel.h"

namespace ZeroEngine
{
    class EditorGUIViewportPanel : public EditorGUIBasePanel
    {
    public:
        EditorGUIViewportPanel();

        void ImGuiRender() override;
        void OnEvent(Event& e) override;
    };
} // ZeroEngine
