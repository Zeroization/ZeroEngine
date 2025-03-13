#pragma once
#include "pch.h"
#include "EditorGUIBasePanel.h"

namespace ZeroEngine
{
    struct PanelData
    {
        const char *UUID = "";
        const char *Name = "";
        std::shared_ptr<EditorGUIBasePanel> Panel = nullptr;
        bool bIsOpen = false;
    };

    class EditorGUIPanelManager
    {
    public:
        static void Create();
        static std::shared_ptr<EditorGUIPanelManager> GetInstance();

        EditorGUIPanelManager() = default;
        ~EditorGUIPanelManager();

        void EditorPanelRender();

    private:
        static std::shared_ptr<EditorGUIPanelManager> Instance;
        std::vector<PanelData> Panels;
    };
} // ZeroEngine
