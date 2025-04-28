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

    protected:
        void BeforePanelSizeChange() override;
        void OnPanelSizeChange(const glm::vec2& size, uint8_t flags) override;
        void AfterPanelSizeChange(const glm::vec2& size) override;

    private:
        bool mbFirstDraw = true;
        uint32_t mLastWidth = 0;
        uint32_t mLastHeight = 0;
    };
} // ZeroEngine
