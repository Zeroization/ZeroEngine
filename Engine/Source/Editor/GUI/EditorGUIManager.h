#pragma once
#include "pch.h"

namespace ZeroEngine
{
    class EditorGUIManager
    {
    public:
        static void Create();
        static void Destroy();
        static EditorGUIManager& GetInstance();

        EditorGUIManager();
        virtual ~EditorGUIManager() = default;
        EditorGUIManager(const EditorGUIManager&) = delete;
        EditorGUIManager(EditorGUIManager&&) = delete;
        EditorGUIManager& operator=(const EditorGUIManager&) = delete;
        EditorGUIManager& operator=(EditorGUIManager&&) = delete;

        /// 逻辑相关===================================
        void Update();
        void CheckGUIShortcuts();


        void Render();
        void GUIRender();

    protected:
        virtual void BeforeGUIRender() = 0;
        virtual void AfterGUIRender() = 0;
    };
} // ZeroEngine
