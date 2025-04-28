#pragma once

#include "pch.h"
#include "Function/Render/Texture/Texture.h"

namespace ZeroEngine
{
    struct TextureStruct
    {
        std::string path;
        std::string propName;
        std::shared_ptr<TextureData> data = nullptr;
    };

    struct CubeMapStruct
    {
        std::string path;
        std::string propName;
        std::shared_ptr<CubeMapData> data = nullptr;
    };

    struct MaterialStruct
    {
        std::string path;
        std::string propName;

        std::string shaderPath;
        std::string shaderCode;

        std::map<std::string, std::pair<ShaderPropertyType, entt::meta_any>> properties;
        std::vector<std::shared_ptr<TextureStruct>> textures;
        std::vector<std::shared_ptr<CubeMapStruct>> cubeMaps;

        MaterialType type = MaterialType::Forward;
    };

    // TODO: Scene, Prefab的序列化Struct

    class ResourceSystem
    {
    public:

    private:
    };
} // ZeroEngine
