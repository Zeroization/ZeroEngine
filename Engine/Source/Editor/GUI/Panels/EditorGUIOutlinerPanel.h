#pragma once
#include "EditorGUIBasePanel.h"

namespace ZeroEngine
{
    class EditorGUIOutlinerPanel : public EditorGUIBasePanel
    {
    public:
        EditorGUIOutlinerPanel();

        void ImGuiRender() override;
        void OnEvent(Event& e) override;
    };
} // ZeroEngine
