#include "EditorGUIPanelManager.h"

namespace ZeroEngine
{
    std::shared_ptr<EditorGUIPanelManager> EditorGUIPanelManager::Instance = nullptr;

    void EditorGUIPanelManager::Create()
    {
        Instance = std::make_shared<EditorGUIPanelManager>();
    }

    std::shared_ptr<EditorGUIPanelManager> EditorGUIPanelManager::GetInstance()
    {
        return Instance;
    }

    EditorGUIPanelManager::~EditorGUIPanelManager()
    {
    }

    void EditorGUIPanelManager::EditorPanelRender()
    {
        for (const auto& Data : Panels)
        {
            Data.Panel->ImGuiRender();
        }
    }
} // ZeroEngine
