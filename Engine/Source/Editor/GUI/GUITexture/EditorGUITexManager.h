#pragma once
#include "pch.h"
#include "imgui.h"

namespace ZeroEngine
{
    struct ImGuiTextureIdx
    {
        uint32_t idx = UINT32_MAX;
        uint32_t engineID = UINT32_MAX;
        ImTextureID ImGuiID = UINT64_MAX;;
    };


    class EditorGUITexManager
    {
    public:
        static void Create();
        static void Destroy();
        static EditorGUITexManager& GetInstance();

        EditorGUITexManager() = default;
        virtual ~EditorGUITexManager() = default;
        EditorGUITexManager(const EditorGUITexManager&) = delete;
        EditorGUITexManager(EditorGUITexManager&&) = delete;
        EditorGUITexManager& operator=(const EditorGUITexManager&) = delete;
        EditorGUITexManager& operator=(EditorGUITexManager&&) = delete;

    public:
        virtual bool CheckExistenceByEngineID(uint32_t id) = 0;
        virtual ImTextureID GetImTextureIDByEngineID(uint32_t id) = 0;

        virtual ImGuiTextureIdx LoadTexture(const std::string& path) = 0;
        virtual void DeleteTexture(const ImGuiTextureIdx& textureIdx) = 0;
        virtual ImGuiTextureIdx CreateTextureFromEngineID(uint32_t id) = 0;
        virtual void DeleteTextureFromEngineID(uint32_t id) = 0;
    };
} // ZeroEngine
