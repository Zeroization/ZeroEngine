#pragma once
#include "../../Core/Math/Math.h"

#include <atomic>

namespace ZeroEngine
{
#pragma region Enums
    enum class GraphAPI : uint8_t
    {
        OpenGL,
        Vulkan, ///< todo...
        DX12    ///< todo...
    };

    enum class GPUBufferType : uint8_t
    {
        StaticGPURead,
        DynamicGPURead,
        DynamicGPUWrite,
        DynamicGPUReadWrite
    };

    enum class FaceCullingOpt : uint8_t
    {
        None,
        Back,
        Front,
        FrontAndBack
    };

    enum class CompareOpt : uint8_t
    {
        Never,
        Less,
        Greater,
        Equal,
        NotEqual,
        LessOrEqual,
        GreaterOrEqual,

        Always
    };

    enum class BlendOpt : uint8_t
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max
    };

    enum class BlendFactor : uint8_t
    {
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha
    };

    enum class FrameBufferClearFlag : uint8_t
    {
        None    = 0b00000000,
        Color   = 0b00000001,
        Depth   = 0b00000010,
        Stencil = 0b00000100,
    };

    ZERO_FORCE_INLINE bool operator&(FrameBufferClearFlag lhs, FrameBufferClearFlag rhs)
    {
        return static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs);
    }

    enum class FrameBufferPieceFlag : uint8_t
    {
        Color    = 0b00000001,
        Depth    = 0b00000010,
        Position = 0b00000100,
        Normal   = 0b00001000,
    };

    ZERO_FORCE_INLINE bool operator&(FrameBufferPieceFlag lhs, FrameBufferPieceFlag rhs)
    {
        return static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs);
    }

    enum class FrameBufferType : uint8_t
    {
        // 纹理写入方式
        Reserve,   ///< 写入时保留源纹理
        Overwrite, ///< 写入时覆盖源纹理

        // 纹理类型
        Normal,        ///< Color附件 + Depth附件
        Color,         ///< 只有Color附件
        HighPrecision, ///< 高精度(Color + Depth)
        GBuffer,       ///< G-Buffer
        ShadowMap,     ///< 平行光的阴影贴图
        ShadowMapCube, ///< 点光源的阴影贴图
    };

    enum class InstanceBufferLayoutType : uint8_t
    {
        Float,

        Uint32_t,
        UnsignedInt,

        Uint8_t,
        UnsignedChar,

        Int32_t,
        Int,
    };

    enum class ShaderDataBufferType : uint8_t
    {
        Uniform, // UBO
        Storage, // SSBO
    };

    // 着色器参数的类型, 供Slang反射使用
    enum class ShaderPropertyType : uint8_t
    {
        Bool,
        Int,
        UInt,
        Float,
        Vec2,  ///< float vec2
        Vec3,  ///< float vec3
        Vec4,  ///< float vec4
        IVec2, ///< integer vec2
        IVec3, ///< integer vec3
        IVec4, ///< integer vec4
        UVec2, ///< unsigned integer vec2
        UVec3, ///< unsigned integer vec3
        UVec4, ///< unsigned integer vec4
        Mat2x2,
        Mat2x3,
        Mat2x4,
        Mat3x2,
        Mat3x3,
        Mat3x4,
        Mat4x2,
        Mat4x3,
        Mat4x4,

        Sampler,
        Sampler2D,
        SamplerCube,
        TextureIndex,
    };

    /// 着色器所有阶段
    /// Ref: Slang::SlangStage
    enum class ShaderStageFlag : uint16_t
    {
        Unknown       = 0b0000000000000000,
        Vertex        = 0b0000000000000001,
        Hull          = 0b0000000000000010, ///< 经典曲面细分 Hull -> 硬件 -> Domain
        Domain        = 0b0000000000000100, ///< 经典曲面细分 Hull -> 硬件 -> Domain
        Geometry      = 0b0000000000001000,
        Fragment      = 0b0000000000010000,
        Pixel         = 0b0000000000010000,
        Compute       = 0b0000000000100000,
        RayGeneration = 0b0000000001000000,
        Intersection  = 0b0000000010000000,
        AnyHit        = 0b0000000100000000,
        ClosestHit    = 0b0000001000000000,
        Miss          = 0b0000010000000000,
        Callable      = 0b0000100000000000,
        Mesh          = 0b0001000000000000, ///< 现代曲面细分着色器  Mesh
        Amplification = 0b0010000000000000,
    };

    enum class RenderPipelineType : uint8_t
    {
        Forward,
        Deferred,
        RayTracing, // TODO
    };

    enum class RenderQueueType : uint8_t
    {
        Opaque,      // 不透明物体
        Transparent, // (半)透明物体
        UI,          // 游戏内UI
        Deferred
    };

    enum class RenderCommandType : uint8_t
    {
        DontCare,

        GenerateShadow,
        GenerateGBuffer,

        ForwardRendering,
        DeferredRendering,
        PostEffectRendering,
        UIRendering,
        AssetPreviewRendering,

        Compute,
    };

    enum class LightType : uint8_t
    {
        None,        // 无
        Directional, // 平行光
        Point,       // 点光源
        Spot,        // 聚光灯
    };

    enum class ShadowType : uint8_t
    {
        None,        ///< 无
        Directional, ///< 平行光阴影
        Point,       ///< 点光源阴影
        Spot         ///< 聚光灯阴影 TODO
    };

    enum class TextureType : uint8_t
    {
        Single2D,
        CubeMap
    };

    enum class MaterialType : uint8_t
    {
        Forward,
        Deferred,
        RayTracing ///< TODO
    };

    enum class NormalGeometryType : uint8_t
    {
        // TODO: 有关简单Mesh的生成, 参考ZXEngine和人宅教程
        Box,
        NormalSphere,
        TessellatedSphere,
        Cylinder,
        Plane,
        Quad,
        Cone,
        Circle
    };
#pragma endregion

#pragma region Structs
    // Render
    struct ClearInfo
    {
        glm::vec4 ClearColor = glm::vec4(0.f, 0.f, 0.f, 1.f);
        uint32_t ClearStencil = 0;
        float ClearDepth = 1.0f;
    };

    struct RenderState
    {
        bool enableDepthTest = true;
        bool enableDepthWrite = true;

        bool enableFaceCulling = true;
        FaceCullingOpt faceCullingOption = FaceCullingOpt::Back;

        BlendFactor srcFactor = BlendFactor::SrcAlpha;
        BlendFactor dstFactor = BlendFactor::OneMinusSrcAlpha;

        ClearInfo clearInfo;
    };

#ifndef ZERO_MAX_BONE_INFLUENCE
#   define ZERO_MAX_BONE_INFLUENCE 4
#endif
    // Vertex
    struct MeshVertex
    {
        glm::vec3 Position = glm::vec3(0.0f);
        glm::vec3 Normal = glm::vec3(0.0f);
        glm::vec2 TexCoords = glm::vec2(0.0f);
        glm::vec3 Tangent = glm::vec3(0.0f);

        // 影响该顶点的关节们
        std::array<int, ZERO_MAX_BONE_INFLUENCE> BoneIDs;
        // 关节对应的权重
        std::array<float, ZERO_MAX_BONE_INFLUENCE> BoneWeights;
    };

    // Texture
    struct TextureData
    {
        int width = 0;
        int height = 0;
        int numChannels = 0;
        uint32_t _PADDING_;

        std::string path;
        uint8_t* data = nullptr;
    };

    struct CubeMapData
    {
        int width = 0;
        int height = 0;
        int numChannels = 0;
        uint32_t _PADDING_;

        std::string path;
        uint8_t* data[6] = {nullptr};
    };

    // Shader
    struct ShaderStateSet
    {
        BlendOpt blendOpt = BlendOpt::Add;
        BlendFactor srcFactor = BlendFactor::SrcAlpha;
        BlendFactor dstFactor = BlendFactor::OneMinusSrcAlpha;
        FaceCullingOpt faceCullOpt = FaceCullingOpt::Back;
        CompareOpt depthCompareOpt = CompareOpt::Less;
        RenderQueueType renderQueueType = RenderQueueType::Opaque;

        bool enableDepthWrite = true;
    };

    // 着色器参数信息 (TODO: 通过Slang反射得到)
    struct ShaderProperty
    {
        std::string varName = ""; ///< 变量名
        uint32_t size = 0;        ///< 该参数的大小(如果是数组则为整个数组大小)
        uint32_t alignment = 0;   ///< 该参数对齐量
        uint32_t offset = 0;      ///< 参数偏移量
        uint32_t space = 0;       ///< 参数绑定空间(VK: UniformBuffer和纹理的LayoutBinding; DX12: 纹理register(t))

        uint32_t arrayLength = 0; ///< 属性数组长度(如果是数组的话)
        uint32_t arrayOffset = 0; ///< 单个元素的偏移量(如果是数组的话)

        ShaderPropertyType type = ShaderPropertyType::Float;
    };

    // 某阶段着色器中所有的参数信息
    struct ShaderPropertiesInfo
    {
        std::vector<ShaderProperty> paramProps;
        std::vector<ShaderProperty> textureProps;
    };

    using ShaderSemanticType = std::pair<ShaderPropertyType, std::string>;

    struct ShaderInstanceInfo
    {
        uint32_t size = 0;
        std::vector<ShaderSemanticType> attributes;
    };

    struct ShaderInfo
    {
        LightType lightType = LightType::None;
        ShadowType shadowType = ShadowType::None;
        ShaderStageFlag stages = ShaderStageFlag::Unknown;
        ShaderStateSet stateSet;
        ShaderInstanceInfo instanceInfo;
        std::unordered_map<ShaderStageFlag, ShaderPropertiesInfo> props;
    };

    struct ShaderDataBufferInfo
    {
        uint32_t binding = 0;
        bool isReadOnly = true;
        ShaderDataBufferType type = ShaderDataBufferType::Uniform;
    };

    struct ShaderReference
    {
        uint32_t id = 0;
        FrameBufferType targetFrameBufferTy = FrameBufferType::Color;

        std::string path;
        ShaderInfo info;
    };

    struct ComputeShaderInfo
    {
        std::vector<ShaderDataBufferInfo> bufferInfos;
    };

    struct ComputeShaderReference
    {
        std::string path;
        uint32_t id = 0;
        ComputeShaderInfo shaderInfo;
    };

#pragma endregion
}
