#pragma once

#include "pch.h"

namespace ZeroEngine
{
    class Game
    {
    public:
        /// 对游戏本体(和编辑器)进行初始化操作
        /// @param path 游戏本体路径
        /// @return 游戏初始化是否成功
        static bool Init(const std::string& path);

        /// 正在运行中的游戏实例, 每帧执行的操作
        static void Tick();

        /// 关闭该游戏实例
        static void ShutDown();

    private:
        /// 运行时全局变量 TODO: 可能封装到一个类中
        static float gEngineDeltaTime;
        static float gLastFrameTime;
        static float gCurFrameTime;

        /// 逻辑相关的Tick
        static void LogicTick(float deltaTime);
        /// 按定长时间更新的Tick操作, 适合用于物理模拟等子系统, 让它们更稳定
        static void FixedTick(float deltaTime);
        /// 渲染相关的Tick
        static void RenderTick();
    };
}


