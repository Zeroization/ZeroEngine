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
        static bool Init(const std::string &path);

        /// 正在运行中的游戏实例, 每帧执行的操作
        static void Tick();

        /// 关闭该游戏实例
        static void ShutDown();

    private:
        /// 逻辑相关的Tick
        static void LogicTick(float deltaTime);

        /// 渲染相关的Tick
        static void RenderTick();
    };
}


