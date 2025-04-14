#include "EditorGUIPanelManager.h"

#include "EditorGUIContentBrowserPanel.h"
#include "EditorGUIOutlinerPanel.h"
#include "EditorGUIScenePanel.h"
#include "EditorGUIViewportPanel.h"

namespace ZeroEngine
{
    std::shared_ptr<EditorGUIPanelManager> EditorGUIPanelManager::sInstance = nullptr;

    void EditorGUIPanelManager::Create()
    {
        sInstance = std::make_shared<EditorGUIPanelManager>();

        sInstance->mPanels.emplace_back(std::make_unique<EditorGUIViewportPanel>());
        sInstance->mPanels.emplace_back(std::make_unique<EditorGUIScenePanel>());
        sInstance->mPanels.emplace_back(std::make_unique<EditorGUIContentBrowserPanel>());
        sInstance->mPanels.emplace_back(std::make_unique<EditorGUIOutlinerPanel>());
    }

    void EditorGUIPanelManager::Destroy()
    {
        for (auto& panel: sInstance->mPanels)
        {
            panel.reset();
        }
        sInstance.reset();
    }

    std::shared_ptr<EditorGUIPanelManager> EditorGUIPanelManager::GetInstance()
    {
        return sInstance;
    }

    void EditorGUIPanelManager::EditorPanelRender()
    {
        for (auto& panel: mPanels)
        {
            panel->ImGuiRender();
        }
    }
} // ZeroEngine
