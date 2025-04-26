#pragma once

#include "Function/Render/RHI/RHI.h"
#include "GLRenderDefines.h"

namespace ZeroEngine
{
    using namespace ZeroEngine::RHI_GL;

    class RHI_OpenGLImpl final : public RHI
    {
    public:
        RHI_OpenGLImpl();

        void BeginFrame() override;
        void EndFrame() override;

        void OnEvent(Event& e) override;

        void SetRenderState(const RenderState& state) override;
        void SetViewPort(uint32_t width, uint32_t height, uint32_t xOffset, uint32_t yOffset) override;

        void SwitchFrameBuffer(uint32_t id) override;
        void ClearFrameBuffer(FrameBufferClearFlag flags) override;
        void BlitFrameBuffer(uint32_t cmd, const std::string& srcName, const std::string& dstName,
                             FrameBufferPieceFlag flags) override;
        std::shared_ptr<FrameBufferObject> CreateFBO(FrameBufferType type, uint32_t width, uint32_t height) override;
        std::shared_ptr<FrameBufferObject> CreateFBO(FrameBufferType type, uint32_t width, uint32_t height,
                                                     const ClearInfo& clearInfo) override;
        void DeleteFBO(const std::shared_ptr<FrameBufferObject>& pFBO) override;
        uint32_t GetRenderBufferTexture(uint32_t id) override;

        uint32_t CreateStaticInstanceBuffer(uint32_t size, const void* data) override;
        uint32_t CreateDynamicInstanceBuffer(uint32_t size) override;
        void UpdateDynamicInstanceBuffer(uint32_t id, uint32_t size, const void* data) override;
        void SetInstanceBufferAttributesByVAO(uint32_t VAO, uint32_t instanceBufferID,
                                              const std::initializer_list<std::pair<InstanceBufferLayoutType, uint32_t>>
                                              &
                                              RHITypeAndElemCntList) override;
        void DeleteInstanceBuffer(uint32_t id) override;

        uint32_t AllocateDrawCommand(RenderCommandType cmdType, FrameBufferClearFlag clearFlags) override;
        void DeleteDrawCommand(uint32_t id) override;
        void GenerateDrawCommand(uint32_t id) override;
        void Draw(uint32_t VAO) override;
        void DrawInstanced(uint32_t VAO, uint32_t instanceCnt, uint32_t instanceBuffer) override;

        uint32_t LoadTexture(const std::u8string& path, int& width, int& height, bool needFlip) override;
        uint32_t CreateTexture(const std::shared_ptr<TextureData>& pData) override;
        uint32_t LoadCubeMap(const std::vector<std::u8string>& facePaths) override;
        uint32_t CreateCubeMap(const std::shared_ptr<CubeMapData>& pData) override;
        void DeleteTexture(uint32_t id) override;

        uint32_t CreateMaterialData() override;
        void SetupMaterial(const std::shared_ptr<Material>& material) override;
        void UseMaterialData(uint32_t ID) override;
        void DeleteMaterialData(uint32_t ID) override;

        void DeleteMesh(unsigned VAO) override;
        void SetupStaticMesh(unsigned& VAO, const std::vector<MeshVertex>& vertices,
                             const std::vector<uint32_t>& indices) override;
        void SetupDynamaticMesh(unsigned& VAO, unsigned vertexSize, unsigned indexSize) override;
        void UpdateDynamaticMesh(unsigned VAO, const std::vector<MeshVertex>& vertices,
                                 const std::vector<uint32_t>& indices) override;

        std::shared_ptr<ShaderReference> LoadAndSetupShader(const std::string& path, FrameBufferType type) override;
        std::shared_ptr<ShaderReference> SetupShader(const std::string& path, const std::string& shaderCode,
                                                     FrameBufferType type) override;
        void DeleteShader(uint32_t id) override;
        void UseShader(uint32_t id) override;
        void SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName, bool value) override;
        void SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           float value) override;
        void SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           int32_t value) override;
        void SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           uint32_t value) override;
        void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::vec2& value) override;
        void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::vec3& value) override;
        void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::vec4& value) override;
        void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::ivec2& value) override;
        void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::ivec3& value) override;
        void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::ivec4& value) override;
        void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::uvec2& value) override;
        void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::uvec3& value) override;
        void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::uvec4& value) override;
        void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::mat2& value) override;
        void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::mat2x3& value) override;
        void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::mat2x4& value) override;
        void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::mat3& value) override;
        void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::mat3x2& value) override;
        void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::mat3x4& value) override;
        void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::mat4& value) override;
        void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::mat4x2& value) override;
        void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                           const glm::mat4x3& value) override;
        void SetTextureProp(const std::shared_ptr<Material>& material, const std::string& propName, uint32_t texID,
                            uint32_t texIdx,
                            bool isBuffer) override;
        void SetCubemapProp(const std::shared_ptr<Material>& material, const std::string& propName, uint32_t texID,
                            uint32_t texIdx,
                            bool isBuffer) override;

        uint32_t CreateSSBO(const void* data, size_t size, GPUBufferType type) override;
        void BindSSBO(uint32_t id, uint32_t binding) override;
        void UpdateSSBO(uint32_t id, const void* data, size_t size) override;
        void DeleteSSBO(uint32_t id) override;
        void BindWithVAO(uint32_t VAO, uint32_t binding) override;

        std::shared_ptr<ComputeShaderReference> LoadAndSetupComputeShader(const std::string& path) override;
        void DeleteComputeShader(uint32_t id) override;
        void DispatchComputeCommand(uint32_t commandID, uint32_t shaderID, uint32_t groupX, uint32_t groupY,
                                    uint32_t groupZ) override;

    private:
        void DoWindowSizeChange();
        void UpdateRenderState();
        void UpdateMaterialData();
        void ClearColorBuffer(const glm::vec4& color);
        void ClearDepthBuffer(float depth);
        void ClearStencilBuffer(uint32_t stencil);

        void DoSetShaderTexture(const std::string& name, uint32_t ID, uint32_t idx);
        void DOSetShaderCubemap(const std::string& name, uint32_t ID, uint32_t idx);

        uint32_t GetNextVAOIdx();
        std::shared_ptr<GLVertexArray> GetVAOByIdx(uint32_t idx);
        uint32_t GetNextMaterialDataIdx();
        std::shared_ptr<GLMaterialData> GetMaterialDataByIdx(uint32_t idx);

    private:
        bool mIsStateDirty = false;
        bool mIsWindowSizeChanged = false;

        uint32_t mCurFBOID = 0;
        uint32_t mCurShaderID = 0;
        uint32_t mCurMaterialDataID = 0;
        uint32_t mNewWindowWidth = 0;
        uint32_t mNewWindowHeight = 0;

        std::vector<std::shared_ptr<GLVertexArray>> mGLVAOArray;
        std::vector<std::shared_ptr<GLMaterialData>> mGLMaterialDataArray;

        std::unordered_map<uint32_t, std::shared_ptr<GLMaterialData>> mShaderToMaterialData;
        std::unordered_map<uint32_t, ClearInfo> mFBOClearInfoMp;

        std::unique_ptr<RenderState> mTargetState = nullptr;
        std::unique_ptr<RenderState> mCurRealState = nullptr;

        // 模板部分
    private:
        template <typename T>
        void SetUniform(const std::string& name, const T& value) const
        {
            ZERO_CORE_ASSERT(false, std::format("Unknown type for uniform var: {}", name));
        }

        template <>
        void SetUniform(const std::string& name, const uint32_t& value) const;
        template <>
        void SetUniform(const std::string& name, const int& value) const;
        template <>
        void SetUniform(const std::string& name, const bool& value) const;
        template <>
        void SetUniform(const std::string& name, const float& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::vec2& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::vec3& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::vec4& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::ivec2& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::ivec3& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::ivec4& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::uvec2& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::uvec3& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::uvec4& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::mat2& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::mat2x3& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::mat2x4& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::mat3& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::mat3x2& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::mat3x4& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::mat4x2& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::mat4x3& value) const;
        template <>
        void SetUniform(const std::string& name, const glm::mat4& value) const;
    };

    ZERO_FORCE_INLINE void GLAPIENTRY GLDebugCallback(unsigned source, unsigned type, unsigned id, unsigned severity,
                                                      int length, const char* message, const void* userParam)
    {
        std::stringstream ss;

        ss << "[";
        switch (source)
        {
            case GL_DEBUG_SOURCE_API: ss << "API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: ss << "Window System";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY: ss << "Third Party";
                break;
            case GL_DEBUG_SOURCE_APPLICATION: ss << "Application";
                break;
            case GL_DEBUG_SOURCE_OTHER: ss << "Other";
                break;
            default: ss << "Unknown";
                break;
        }
        ss << "]";

        ss << "[";
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR: ss << "Error";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "Deprecated";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: ss << "UB";
                break;
            case GL_DEBUG_TYPE_PORTABILITY: ss << "Portability";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE: ss << "Performance";
                break;
            case GL_DEBUG_TYPE_MARKER: ss << "Marker";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP: ss << "Push Group";
                break;
            case GL_DEBUG_TYPE_POP_GROUP: ss << "Pop Group";
                break;
            case GL_DEBUG_TYPE_OTHER: ss << "Other";
                break;
            default: ss << "Unknown";
                break;
        }
        ss << "]" << id << ":";

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                LOG_ERROR(std::format("[{}]{} {}", __FUNCTION__, ss.str(), message));
                ZERO_CORE_ASSERT(false, "OpenGL error occurred!");
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                LOG_WARN(std::format("[{}]{} {}", __FUNCTION__, ss.str(), message));
                break;
            case GL_DEBUG_SEVERITY_LOW:
                LOG_INFO(std::format("[{}]{} {}", __FUNCTION__, ss.str(), message));
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                LOG_DEBUG(std::format("[{}]{} {}", __FUNCTION__, ss.str(), message));
                break;
        }
    }
} // ZeroEngine
