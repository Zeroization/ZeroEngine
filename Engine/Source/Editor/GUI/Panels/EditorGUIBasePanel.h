#pragma once

#include "EditorDefines.h"
#include "Core/Hash/Identifier.h"
#include "Core/Event/Event.h"

namespace ZeroEngine
{
    class EditorGUIBasePanel
    {
    public:
        virtual ~EditorGUIBasePanel() = default;

        virtual void ImGuiRender() = 0;
        virtual void OnEvent(Event& e) = 0;

        virtual void ResetPanel()
        {
        }

        uint32_t GetHashedID() const
        {
            ZERO_CORE_ASSERT(mpIdentifier != nullptr, "Get ID in uninitialized panel.");
            return mpIdentifier->GetHashVal();
        }

    protected:
        void CheckManualResize();

        virtual void BeforePanelSizeChange()
        {
        }

        virtual void OnPanelSizeChange(const glm::vec2& size, uint8_t flags)
        {
        }

        virtual void AfterPanelSizeChange(const glm::vec2& size)
        {
        }

    protected:
        bool mbIsWindowFocused = false;
        bool mbPressing = false;
        bool mbResizing = false;
        std::string mPanelTagName;
        std::unique_ptr<Identifier> mpIdentifier = nullptr;

        glm::vec2 mPos;
        glm::vec2 mDragPos;
        glm::vec2 mSize;
        glm::vec2 mDragSize;
    };
} // ZeroEngine
