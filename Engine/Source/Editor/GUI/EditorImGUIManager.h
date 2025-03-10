#pragma once
#include "pch.h"

namespace ZeroEngine
{
    class EditorImGUIManager
    {
    public:
        static void Create();
        static void Destroy();
        static std::shared_ptr<EditorImGUIManager> GetInstance();

        EditorImGUIManager() = default;
        virtual ~EditorImGUIManager() = default;

        void Render();

    protected:
        virtual void BeforeGUIRender() = 0;
        virtual void GUIRender() = 0;
        virtual void AfterGUIRender() = 0;

    private:
        static std::shared_ptr<EditorImGUIManager> Instance;
    };
} // ZeroEngine
