#pragma once

#include "pch.h"
#include "Function/Render/FrameBuffer/FrameBufferObject.h"

namespace ZeroEngine
{
    class UUID32;
    class Event;
    class Material;
    class MaterialData;
    struct RenderState;

    class RHI
    {
    public:
        static void Create();
        static RHI& GetInstance();

        RHI() = default;
        virtual ~RHI() = default;
        RHI(const RHI&) = delete;
        RHI(RHI&&) = delete;
        RHI& operator=(const RHI&) = delete;
        RHI& operator=(RHI&&) = delete;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        /// 一些事件
        virtual void OnEvent(Event& e) = 0;

        /// 光栅化渲染管线的RHI
    public:
        /// Setters
        virtual void SetRenderState(const RenderState& state) = 0;
        virtual void SetViewPort(uint32_t width, uint32_t height, uint32_t xOffset, uint32_t yOffset) = 0;

        /// 帧缓冲 - FrameBuffer
        virtual void SwitchFrameBuffer(uint32_t id) = 0;
        virtual void ClearFrameBuffer(FrameBufferClearFlag flags) = 0;
        virtual void BlitFrameBuffer(uint32_t cmd, const std::string& srcName, const std::string& dstName,
                                     FrameBufferPieceFlag flags) = 0;
        /// 创建一个帧缓冲, 宽度或高度为0表示它的值为屏幕的宽度或高度
        virtual std::shared_ptr<FrameBufferObject> CreateFBO(FrameBufferType type, uint32_t width, uint32_t height) = 0;
        virtual std::shared_ptr<FrameBufferObject> CreateFBO(FrameBufferType type, uint32_t width, uint32_t height,
                                                             const ClearInfo& clearInfo) = 0;
        virtual void DeleteFBO(const std::shared_ptr<FrameBufferObject>& pFBO) = 0;
        virtual uint32_t GetRenderBufferTexture(uint32_t id) = 0;

        /// VBO, VAO相关 - InstanceBuffer
        // 创建指定size的buffer, 数据为data
        virtual uint32_t CreateStaticInstanceBuffer(uint32_t size, const void* data) = 0;
        // 创建指定size的buffer, 稍后指定数据
        virtual uint32_t CreateDynamicInstanceBuffer(uint32_t size) = 0;
        virtual void UpdateDynamicInstanceBuffer(uint32_t id, uint32_t size, const void* data) = 0;
        virtual void SetInstanceBufferAttributesByVAO(uint32_t VAO, uint32_t instanceBufferID,
                                                      const std::initializer_list<std::pair<InstanceBufferLayoutType,
                                                          uint32_t>>&
                                                      RHITypeAndElemCntList) = 0;
        virtual void DeleteInstanceBuffer(uint32_t id) = 0;

        /// Draw Call
        virtual uint32_t AllocateDrawCommand(RenderCommandType cmdType, FrameBufferClearFlag clearFlags) = 0;
        virtual void DeleteDrawCommand(uint32_t id) = 0;
        virtual void GenerateDrawCommand(uint32_t id) = 0;
        virtual void Draw(uint32_t VAO) = 0;
        virtual void DrawInstanced(uint32_t VAO, uint32_t instanceCnt, uint32_t instanceBuffer) = 0;

        /// 纹理 - Texture
        virtual uint32_t LoadTexture(const std::u8string& path, int& width, int& height, bool needFlip) = 0;
        virtual uint32_t CreateTexture(const std::shared_ptr<TextureData>& pData) = 0;
        virtual uint32_t LoadCubeMap(const std::vector<std::u8string>& facePaths) = 0;
        virtual uint32_t CreateCubeMap(const std::shared_ptr<CubeMapData>& pData) = 0;
        virtual void DeleteTexture(uint32_t id) = 0;

        /// 材质 - Material
        virtual uint32_t CreateMaterialData() = 0;
        virtual void SetupMaterial(const std::shared_ptr<Material>& material) = 0;
        virtual void UseMaterialData(uint32_t ID) = 0;
        virtual void DeleteMaterialData(uint32_t ID) = 0;

        /// 网格体 - Mesh
        virtual void DeleteMesh(unsigned int VAO) = 0;
        virtual void SetupStaticMesh(unsigned int& VAO, const std::vector<MeshVertex>& vertices,
                                     const std::vector<uint32_t>& indices) = 0;
        virtual void SetupDynamaticMesh(unsigned int& VAO, unsigned int vertexSize, unsigned int indexSize) = 0;
        virtual void UpdateDynamaticMesh(unsigned int VAO, const std::vector<MeshVertex>& vertices,
                                         const std::vector<uint32_t>& indices) = 0;

        /// 着色器 - Shader TODO: 根据Slang修改接口
        virtual std::shared_ptr<ShaderReference> LoadAndSetupShader(const std::string& path, FrameBufferType type) = 0;
        virtual std::shared_ptr<ShaderReference> SetupShader(const std::string& path, const std::string& shaderCode,
                                                             FrameBufferType type) = 0;
        virtual void DeleteShader(uint32_t id) = 0;
        virtual void UseShader(uint32_t id) = 0;
        virtual void SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   bool value) = 0;
        virtual void SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   float value) = 0;
        virtual void SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   int32_t value) = 0;
        virtual void SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   uint32_t value) = 0;
        virtual void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::vec2& value) = 0;
        virtual void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::vec3& value) = 0;
        virtual void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::vec4& value) = 0;
        virtual void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::ivec2& value) = 0;
        virtual void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::ivec3& value) = 0;
        virtual void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::ivec4& value) = 0;
        virtual void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::uvec2& value) = 0;
        virtual void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::uvec3& value) = 0;
        virtual void SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::uvec4& value) = 0;
        virtual void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::mat2& value) = 0;
        virtual void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::mat2x3& value) = 0;
        virtual void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::mat2x4& value) = 0;
        virtual void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::mat3& value) = 0;
        virtual void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::mat3x2& value) = 0;
        virtual void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::mat3x4& value) = 0;
        virtual void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::mat4& value) = 0;
        virtual void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::mat4x2& value) = 0;
        virtual void SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                   const glm::mat4x3& value) = 0;
        virtual void SetTextureProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                    uint32_t texID, uint32_t texIdx, bool isBuffer) = 0;
        virtual void SetCubemapProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                    uint32_t texID, uint32_t texIdx, bool isBuffer) = 0;

        // 和GPU交互的计算相关API
    public:
        // SSBO
        virtual uint32_t CreateSSBO(const void* data, size_t size, GPUBufferType type) = 0;
        virtual void BindSSBO(uint32_t id, uint32_t binding) = 0;
        virtual void UpdateSSBO(uint32_t id, const void* data, size_t size) = 0;
        virtual void DeleteSSBO(uint32_t id) = 0;
        virtual void BindWithVAO(uint32_t VAO, uint32_t binding) = 0;

        // Compute Shader
        virtual std::shared_ptr<ComputeShaderReference> LoadAndSetupComputeShader(const std::string& path) = 0;
        virtual void DeleteComputeShader(uint32_t id) = 0;
        virtual void DispatchComputeCommand(uint32_t commandID, uint32_t shaderID, uint32_t groupX, uint32_t groupY,
                                            uint32_t groupZ) = 0;

    protected:
        uint32_t mMissingTexID = 0;
    };
} // ZeroEngine
