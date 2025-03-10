#pragma once

#include "EditorImGUIManager.h"

namespace ZeroEngine
{
    class EditorImGUIManager_OpenGLImpl final : public EditorImGUIManager
    {
    public:
        EditorImGUIManager_OpenGLImpl();
        ~EditorImGUIManager_OpenGLImpl() override;

    protected:
        void BeforeGUIRender() override;
        void GUIRender() override;
        void AfterGUIRender() override;
    };
} // ZeroEngine
