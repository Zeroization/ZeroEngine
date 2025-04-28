#include "EditorGUITexManager_OpenGLImpl.h"

#include "Function/Render/RHI/RHI.h"

namespace ZeroEngine
{
    bool EditorGUITexManager_OpenGLImpl::CheckExistenceByEngineID(uint32_t id)
    {
        return mAllTextures.contains(id);
    }

    ImTextureID EditorGUITexManager_OpenGLImpl::GetImTextureIDByEngineID(uint32_t id)
    {
        return mAllTextures[id].ImGuiID;
    }

    ImGuiTextureIdx EditorGUITexManager_OpenGLImpl::LoadTexture(const std::string& path)
    {
        int width = 0, height = 0;
        ImGuiTextureIdx newImGuiTex;
        newImGuiTex.engineID = RHI::GetInstance().LoadTexture(path, width, height, false);
        newImGuiTex.ImGuiID = newImGuiTex.engineID;
        mAllTextures[newImGuiTex.engineID] = newImGuiTex;
        return newImGuiTex;
    }

    void EditorGUITexManager_OpenGLImpl::DeleteTexture(const ImGuiTextureIdx& textureIdx)
    {
        RHI::GetInstance().DeleteTexture(textureIdx.engineID);
        mAllTextures.erase(textureIdx.engineID);
    }

    ImGuiTextureIdx EditorGUITexManager_OpenGLImpl::CreateTextureFromEngineID(uint32_t id)
    {
        ImGuiTextureIdx newImGuiTex;
        newImGuiTex.engineID = id;
        newImGuiTex.ImGuiID = id;
        mAllTextures[newImGuiTex.engineID] = newImGuiTex;
        return newImGuiTex;
    }

    void EditorGUITexManager_OpenGLImpl::DeleteTextureFromEngineID(uint32_t id)
    {
        mAllTextures.erase(id);
    }
} // ZeroEngine
