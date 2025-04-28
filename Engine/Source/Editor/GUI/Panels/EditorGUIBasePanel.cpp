#include "EditorGUIBasePanel.h"

#include "imgui.h"

namespace ZeroEngine
{
    void EditorGUIBasePanel::CheckManualResize()
    {
        bool isPressing = ImGui::IsMouseDown(0);
        if (isPressing && !mbPressing)
        {
            // 按下: 当前按下, 上一帧没按下
            ImVec2 curWindowPos = ImGui::GetWindowPos();
            mPos.x = curWindowPos.x;
            mPos.y = curWindowPos.y;
            mDragPos = mPos;

            ImVec2 curWindowSize = ImGui::GetWindowSize();
            mSize.x = curWindowSize.x;
            mSize.y = curWindowSize.y;
            mDragSize = mSize;

            BeforePanelSizeChange();
        }
        else if (!isPressing && mbPressing)
        {
            // 松开: 当前松开, 上一帧按下
            if (mSize != mDragSize)
            {
                AfterPanelSizeChange(mDragSize);
            }
        }
        else if (isPressing)
        {
            // 按住
            ImVec2 curWindowPos = ImGui::GetWindowPos();
            ImVec2 curWindowSize = ImGui::GetWindowSize();

            uint8_t flags = static_cast<uint8_t>(EditorPanelEdgeFlag::None);
            if (mDragSize.x != curWindowSize.x)
            {
                if (mDragPos.x == curWindowPos.x)
                    flags |= EditorPanelEdgeFlag::Right;
                else
                    flags |= EditorPanelEdgeFlag::Left;
            }
            if (mDragSize.y != curWindowSize.y)
            {
                if (mDragPos.y == curWindowPos.y)
                    flags |= EditorPanelEdgeFlag::Bottom;
                else
                    flags |= EditorPanelEdgeFlag::Top;
            }

            mDragPos.x = curWindowPos.x;
            mDragPos.y = curWindowPos.y;

            mDragSize.x = curWindowSize.x;
            mDragSize.y = curWindowSize.y;

            OnPanelSizeChange(mDragSize, flags);
        }

        mbPressing = isPressing;
    }
}

