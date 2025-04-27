#pragma once

#include "pch.h"

namespace ZeroEngine
{
    class Shader;
    class MaterialData;
    struct MaterialStruct;

    class Material
    {
    public:
        explicit Material(MaterialStruct* matStruct);
        virtual ~Material();

        void Use();
        void SetProperties();
        int GetRenderQueue() const;
        RenderQueueType GetRenderQueueType() const;

        void SetScalarProp(const std::string& propName, bool value);
        void SetScalarProp(const std::string& propName, float value);
        void SetScalarProp(const std::string& propName, int32_t value);
        void SetScalarProp(const std::string& propName, uint32_t value);
        void SetVectorProp(const std::string& propName, const glm::vec2& value);
        void SetVectorProp(const std::string& propName, const glm::vec3& value);
        void SetVectorProp(const std::string& propName, const glm::vec4& value);
        void SetVectorProp(const std::string& propName, const glm::ivec2& value);
        void SetVectorProp(const std::string& propName, const glm::ivec3& value);
        void SetVectorProp(const std::string& propName, const glm::ivec4& value);
        void SetVectorProp(const std::string& propName, const glm::uvec2& value);
        void SetVectorProp(const std::string& propName, const glm::uvec3& value);
        void SetVectorProp(const std::string& propName, const glm::uvec4& value);
        void SetMatrixProp(const std::string& propName, const glm::mat2& value);
        void SetMatrixProp(const std::string& propName, const glm::mat2x3& value);
        void SetMatrixProp(const std::string& propName, const glm::mat2x4& value);
        void SetMatrixProp(const std::string& propName, const glm::mat3& value);
        void SetMatrixProp(const std::string& propName, const glm::mat3x2& value);
        void SetMatrixProp(const std::string& propName, const glm::mat3x4& value);
        void SetMatrixProp(const std::string& propName, const glm::mat4& value);
        void SetMatrixProp(const std::string& propName, const glm::mat4x2& value);
        void SetMatrixProp(const std::string& propName, const glm::mat4x3& value);
        void SetTextureProp(const std::string& propName, uint32_t texID, uint32_t texIdx, bool isBuffer);
        void SetCubemapProp(const std::string& propName, uint32_t texID, uint32_t texIdx, bool isBuffer);

    public:
        std::string mName;
        std::string mPath;
        MaterialType mType = MaterialType::Forward;
        std::shared_ptr<MaterialData> mData = nullptr;
        std::shared_ptr<Shader> mShader = nullptr;

    private:
        void CopyMaterialStructTOMaterialData(MaterialStruct* matStruct, const std::shared_ptr<MaterialData>& data);

    private:
        uint32_t mTextureIdx;
        int32_t mRenderQueue;
        RenderQueueType mRenderQueueTy = RenderQueueType::Opaque;
    };
} // ZeroEngine
