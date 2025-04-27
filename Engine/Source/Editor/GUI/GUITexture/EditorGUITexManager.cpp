#include "EditorGUITexManager.h"
#ifdef ZERO_GRAPHIC_OPENGL
#include "EditorGUITexManager_OpenGLImpl.h"
#else
#endif

namespace ZeroEngine
{
    void EditorGUITexManager::Create()
    {
        GetInstance();
    }

    void EditorGUITexManager::Destroy()
    {
    }

    EditorGUITexManager& EditorGUITexManager::GetInstance()
    {
#ifdef ZERO_GRAPHIC_OPENGL
        static EditorGUITexManager_OpenGLImpl sInstance;
#else
        ZERO_CORE_ASSERT(false, "TODO");
#endif
        return sInstance;
    }
} // ZeroEngine
