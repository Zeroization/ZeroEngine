#pragma once

#include "EditorGUIBasePanel.h"

namespace ZeroEngine
{
    class EditorGUIContentBrowserPanel : public EditorGUIBasePanel
    {
    public:
        EditorGUIContentBrowserPanel();

        void ImGuiRender() override;
        void OnEvent(Event& e) override;
    };
} // ZeroEngine
