#pragma once
#include "pch.h"

namespace ZeroEngine
{
    class EditorGUIManager
    {
    public:
        static void Create();
        static void Destroy();
        static std::shared_ptr<EditorGUIManager> GetInstance();

        EditorGUIManager();
        virtual ~EditorGUIManager() = default;

        /// 逻辑相关===================================
        void Update();
        void CheckGUIShortcuts();


        void Render();
        void GUIRender();

    protected:
        virtual void BeforeGUIRender() = 0;
        virtual void AfterGUIRender() = 0;

    private:
        static std::shared_ptr<EditorGUIManager> Instance;
    };
} // ZeroEngine
