#include "RenderPassManager.h"

#include "Core/GlobalDataManager.h"

namespace ZeroEngine
{
    void RenderPassManager::Create()
    {
        GetInstance().mAllPasses.resize(static_cast<size_t>(RenderPassType::USE_FOR_COUNT));
        // TODO: 填满Pass
    }

    void RenderPassManager::Destroy()
    {
    }

    RenderPassManager& RenderPassManager::GetInstance()
    {
        static RenderPassManager sInstance;
        return sInstance;
    }

    void RenderPassManager::SetupRenderPasses()
    {
        mCurPasses.clear();
        std::vector<std::unique_ptr<RenderPassBase>>{}.swap(mCurPasses);

        RenderPipelineType pipelineTy = GlobalDataManager::GetInstance().GetGlobalDataRef()->renderPipeline;
        switch (pipelineTy)
        {
            case RenderPipelineType::Forward:
            {
                mCurPasses.resize(4);
                break;
            }
            case RenderPipelineType::Deferred:
            {
                mCurPasses.resize(4);
                break;
            }
            case RenderPipelineType::RayTracing:
            {
                ZERO_CORE_ASSERT(false, "TODO");
                mCurPasses.resize(3);
                break;
            }
            default:
            {
                ZERO_CORE_ASSERT(false, "Unknown renderPipeline type");
                break;
            }
        }
    }
} // ZeroEngine
