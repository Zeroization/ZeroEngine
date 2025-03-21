#pragma once

#include "pch.h"

namespace ZeroEngine
{
    struct KeyData
    {
        KeyCode Key;
        KeyState CurState = KeyState::None;
        KeyState OldState = KeyState::None;
    };

    struct MouseBtnData
    {
        MouseButton MouseBtn;
        KeyState CurState = KeyState::None;
        KeyState OldState = KeyState::None;
    };

    class InputManager
    {
    public:
        static std::shared_ptr<InputManager> GetInstance();
        static void Create();
        static void Destroy();

        InputManager() = default;
        virtual ~InputManager() = default;

        virtual void Update() = 0;

        /// 将上一帧为Pressed状态的key, mb设置为Held状态
        void TransitionPressed();
        /// 将上一帧为Released状态的key, mb设置为None状态
        void ClearReleased();

        /// 键盘输入相关
        void UpdateKeyState(Key key, KeyState newState);
        bool IsKeyPressed(Key key);
        bool IsKeyHeld(Key key);
        /// 这里的Down强调 “正在” 按下, 因此检测逻辑不一样
        bool IsKeyDown(Key key);
        bool isKeyReleased(Key key);

        /// 鼠标输入相关
        void UpdateMouseButtonState(MouseButton mb, KeyState newState);
        bool IsMouseButtonPressed(MouseButton mb);
        bool isMouseButtonHeld(MouseButton mb);
        bool isMouseButtonDown(MouseButton mb);
        bool isMouseButtonReleased(MouseButton mb);

    private:
        inline static std::map<KeyCode, KeyData> sKeyData;
        inline static std::map<MouseButton, MouseBtnData> sMouseData;
        static std::shared_ptr<InputManager> Instance;
    };
}
