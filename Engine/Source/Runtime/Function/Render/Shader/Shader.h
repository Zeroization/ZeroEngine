#pragma once

#include "pch.h"

namespace ZeroEngine
{
    class Shader
    {
    public:
        Shader(const std::string& path, FrameBufferType type);
        Shader(const std::string& path, const std::string& shaderCode, FrameBufferType type);
        virtual ~Shader();

        void Use() const;
        uint32_t GetID() const;
        LightType GetLightType() const;
        ShadowType GetShadowType() const;

    public:
        std::string name;
        std::shared_ptr<ShaderReference> reference = nullptr;

    private:
        static std::vector<std::shared_ptr<ShaderReference>> mLoadedShaders;
    };
} // ZeroEngine
