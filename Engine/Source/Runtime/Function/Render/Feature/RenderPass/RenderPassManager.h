#pragma once

#include "pch.h"

namespace ZeroEngine
{
    enum class RenderPassType : uint8_t
    {
        ShadowGeneration,    ///< 阴影生成Pass
        ForwardRendering,    ///< 前向渲染Pass
        GBufferGeneration,   ///< G-Buffer生成Pass
        DeferredRendering,   ///< 延迟渲染Pass
        PostEffectRendering, ///< 后处理Pass
        UIRendering,         ///< UI渲染Pass
        USE_FOR_COUNT
    };

    class RenderPassBase;

    class RenderPassManager
    {
    public:
        static void Create();
        static void Destroy();
        static RenderPassManager& GetInstance();

        RenderPassManager() = default;
        virtual ~RenderPassManager() = default;
        RenderPassManager(const RenderPassManager&) = delete;
        RenderPassManager(RenderPassManager&&) = delete;
        RenderPassManager& operator=(const RenderPassManager&) = delete;
        RenderPassManager& operator=(RenderPassManager&&) = delete;

        void SetupRenderPasses();

    public:
        std::vector<std::unique_ptr<RenderPassBase>> mCurPasses;
        std::vector<std::unique_ptr<RenderPassBase>> mAllPasses;
    };
} // ZeroEngine
