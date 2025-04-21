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
        static EditorGUIPanelManager& GetInstance();

        EditorGUIPanelManager() = default;
        ~EditorGUIPanelManager() = default;
        EditorGUIPanelManager(const EditorGUIPanelManager&) = delete;
        EditorGUIPanelManager(EditorGUIPanelManager&&) = delete;
        EditorGUIPanelManager& operator=(const EditorGUIPanelManager&) = delete;
        EditorGUIPanelManager& operator=(EditorGUIPanelManager&&) = delete;

        void EditorPanelRender();

    private:
        std::vector<std::unique_ptr<EditorGUIBasePanel>> mPanels;
    };
} // ZeroEngine
