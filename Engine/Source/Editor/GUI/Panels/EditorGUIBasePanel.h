#pragma once

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
        // TODO: 封装项目Project类, 封装渲染场景Scene类
        // virtual void OnProjectChanged() = 0;
        // virtual void OnSceneChanged() = 0;

        uint32_t GetHashedID() const
        {
            ZERO_CORE_ASSERT(mpIdentifier != nullptr, "Get ID in uninitialized panel.");
            return mpIdentifier->GetHashVal();
        }

    protected:
        bool mbIsWindowFocused = false;
        std::string mPanelTagName;
        std::unique_ptr<Identifier> mpIdentifier = nullptr;
    };
} // ZeroEngine
