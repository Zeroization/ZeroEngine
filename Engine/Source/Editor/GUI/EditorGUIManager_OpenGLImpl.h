#pragma once

#include "EditorGUIManager.h"

namespace ZeroEngine
{
    class EditorGUIManager_OpenGLImpl final : public EditorGUIManager
    {
    public:
        EditorGUIManager_OpenGLImpl();
        ~EditorGUIManager_OpenGLImpl() override;

    protected:
        void BeforeGUIRender() override;
        void AfterGUIRender() override;
    };
} // ZeroEngine
