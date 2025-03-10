#include "EditorImGUIManager.h"
#ifdef ZERO_GRAPHIC_OPENGL
#include "EditorImGUIManager_OpenGLImpl.h"
#endif

namespace ZeroEngine
{
    std::shared_ptr<EditorImGUIManager> EditorImGUIManager::Instance = nullptr;

    void EditorImGUIManager::Create()
    {
#ifdef ZERO_GRAPHIC_OPENGL
        Instance = std::make_shared<EditorImGUIManager_OpenGLImpl>();
#endif
    }

    void EditorImGUIManager::Destroy()
    {
        Instance.reset();
    }

    std::shared_ptr<EditorImGUIManager> EditorImGUIManager::GetInstance()
    {
        return Instance;
    }

    void EditorImGUIManager::Render()
    {
        BeforeGUIRender();
        GUIRender();
        AfterGUIRender();
    }
} // ZeroEngine
