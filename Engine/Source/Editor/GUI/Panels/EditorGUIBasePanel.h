#pragma once

namespace ZeroEngine
{
    class EditorGUIBasePanel
    {
    public:
        virtual ~EditorGUIBasePanel() = default;

        /// 执行面板控件的渲染
        virtual void ImGuiRender() = 0;
    };
} // ZeroEngine
