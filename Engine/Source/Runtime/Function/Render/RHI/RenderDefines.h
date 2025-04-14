#pragma once

#include <cstdint>

namespace ZeroEngine
{
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

    enum class FrameBufferAttachmentFlag : uint8_t
    {
        None     = 0b00000000,
        Color    = 0b00000001,
        Depth    = 0b00000010,
        Position = 0b00000100,
        Normal   = 0b00001000,
    };

    enum class FrameBufferType : uint8_t
    {
        GBuffer,
        ShadowMap
    };

    enum class ShaderDataBufferType : uint8_t
    {
        Uniform, // UBO
        Storage, // SSBO
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
        UI           // 游戏内UI
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
        Directional, // 平行光
        Point,       // 点光源
        Spot,        // 聚光灯
    };

    enum class NormalGeometryType : uint8_t
    {
        // TODO: 参考ZXEngine和人宅教程
        Box,
        NormalSphere,
        TessellatedSphere,
        Cylinder,
        Plane,
        Quad,
        Cone,
        Circle
    };
}
