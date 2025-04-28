#pragma once

#include "pch.h"
#include "_generated/reflection/EngineConfig.reflgen.h"

namespace ZeroEngine
{
    struct GlobalData
    {
        // 各种配置
        EngineConfig engineCfg; ///< 引擎配置

        // 运行时全局变量
        float EngineDeltaTime = 0.0; ///< 引擎帧间DeltaTime
        float LastFrameTime = 0.0;   ///< 上一帧的时间
        float CurFrameTime = 0.0;    ///< 当前帧的时间

        uint32_t ScreenWidth;  ///< 当前屏幕宽度
        uint32_t ScreenHeight; ///< 当前屏幕高度

        // Editor相关
        bool IsGameStart;
        bool IsGamePause;
        bool IsGameView = true;
        RenderPipelineType renderPipeline;

        // 路径
        std::filesystem::path BuiltinAssetsDir; ///< BuiltinAssets文件夹路径
    };

    class GlobalDataManager
    {
    public:
        static bool Create(const std::filesystem::path& engineWorkDir);
        static void Destroy();
        static GlobalDataManager& GetInstance();

        GlobalDataManager() = default;
        virtual ~GlobalDataManager() = default;
        GlobalDataManager(const GlobalDataManager&) = delete;
        GlobalDataManager(GlobalDataManager&&) = delete;
        GlobalDataManager& operator=(const GlobalDataManager&) = delete;
        GlobalDataManager& operator=(GlobalDataManager&&) = delete;

        GlobalData* GetGlobalDataRef() const;
        bool InitGlobalData();
        bool SaveGlobalData() const;

    private:
        std::unique_ptr<GlobalData> mpGlobalData;
    };
}
