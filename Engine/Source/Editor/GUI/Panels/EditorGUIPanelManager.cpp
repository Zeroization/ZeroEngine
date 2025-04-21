#include "EditorGUIPanelManager.h"

#include "EditorGUIContentBrowserPanel.h"
#include "EditorGUIOutlinerPanel.h"
#include "EditorGUIScenePanel.h"
#include "EditorGUIViewportPanel.h"

namespace ZeroEngine
{

    void EditorGUIPanelManager::Create()
    {
        GetInstance().mPanels.emplace_back(std::make_unique<EditorGUIViewportPanel>());
        GetInstance().mPanels.emplace_back(std::make_unique<EditorGUIScenePanel>());
        GetInstance().mPanels.emplace_back(std::make_unique<EditorGUIContentBrowserPanel>());
        GetInstance().mPanels.emplace_back(std::make_unique<EditorGUIOutlinerPanel>());
    }

    void EditorGUIPanelManager::Destroy()
    {
        for (auto& panel: GetInstance().mPanels)
        {
            panel.reset();
        }
    }

    EditorGUIPanelManager& EditorGUIPanelManager::GetInstance()
    {
        static EditorGUIPanelManager sInstance;
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
