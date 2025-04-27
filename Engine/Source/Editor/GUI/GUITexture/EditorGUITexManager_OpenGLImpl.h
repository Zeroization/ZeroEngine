#pragma once
#include "EditorGUITexManager.h"

namespace ZeroEngine
{
    class EditorGUITexManager_OpenGLImpl : public EditorGUITexManager
    {
    public:
        bool CheckExistenceByEngineID(uint32_t id) override;
        ImTextureID GetImTextureIDByEngineID(uint32_t id) override;
        ImGuiTextureIdx LoadTexture(const std::string& path) override;
        void DeleteTexture(const ImGuiTextureIdx& textureIdx) override;
        ImGuiTextureIdx CreateTextureFromEngineID(uint32_t id) override;
        void DeleteTextureFromEngineID(uint32_t id) override;

    private:
        std::unordered_map<uint32_t, ImGuiTextureIdx> mAllTextures;
    };
} // ZeroEngine
