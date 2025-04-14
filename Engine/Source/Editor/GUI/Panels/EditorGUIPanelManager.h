#pragma once
#include "pch.h"
#include "EditorGUIBasePanel.h"

namespace ZeroEngine
{
    class EditorGUIPanelManager
    {
    public:
        static void Create();
        static void Destroy();
        static std::shared_ptr<EditorGUIPanelManager> GetInstance();

        EditorGUIPanelManager() = default;
        ~EditorGUIPanelManager() = default;

        void EditorPanelRender();

    private:
        static std::shared_ptr<EditorGUIPanelManager> sInstance;
        std::vector<std::unique_ptr<EditorGUIBasePanel>> mPanels;
    };
} // ZeroEngine
