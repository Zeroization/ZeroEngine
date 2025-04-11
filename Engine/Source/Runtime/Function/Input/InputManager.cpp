#include "InputManager.h"

#include "Function/Render/Window/WindowManager.h"
#if defined(ZERO_PLATFORM_DESKTOP)
#include "Function/Input/GlfwInputManager.h"
#endif

namespace ZeroEngine
{
    std::shared_ptr<InputManager> InputManager::Instance = nullptr;

    std::shared_ptr<InputManager> InputManager::GetInstance()
    {
        return Instance;
    }

    void InputManager::Create()
    {
#if defined(ZERO_PLATFORM_DESKTOP)
        Instance = std::make_shared<GlfwInputManager>();
#endif
    }

    void InputManager::Destroy()
    {
        Instance.reset();
    }

    void InputManager::TransitionPressed()
    {
        for (const auto& [key, keyData]: sKeyData)
        {
            if (keyData.CurState == KeyState::Pressed)
            {
                UpdateKeyState(key, KeyState::Held);
            }
        }

        for (const auto& [mb, mbData]: sMouseData)
        {
            if (mbData.CurState == KeyState::Pressed)
            {
                UpdateMouseButtonState(mb, KeyState::Held);
            }
        }
    }

    void InputManager::ClearReleased()
    {
        for (const auto& [key, keyData]: sKeyData)
        {
            if (keyData.CurState == KeyState::Released)
            {
                UpdateKeyState(key, KeyState::None);
            }
        }

        for (const auto& [mb, mbData]: sMouseData)
        {
            if (mbData.CurState == KeyState::Released)
            {
                UpdateMouseButtonState(mb, KeyState::None);
            }
        }
    }

    void InputManager::UpdateKeyState(Key key, KeyState newState)
    {
        auto& [Key, CurState, OldState] = sKeyData[key];
        Key = key;
        OldState = CurState;
        CurState = newState;

        // TODO: 接入事件系统, Trigger事件
        LOG_DEBUG(std::format("[{}] Key {} {}.", __FUNCTION__, static_cast<int>(key), static_cast<int>(newState)));
    }

    bool InputManager::IsKeyPressed(Key key)
    {
        return sKeyData.contains(key) && sKeyData[key].CurState == KeyState::Pressed;
    }

    bool InputManager::IsKeyHeld(Key key)
    {
        return sKeyData.contains(key) && sKeyData[key].CurState == KeyState::Held;
    }

    bool InputManager::IsKeyDown(Key key)
    {
        return WindowManager::GetInstance()->CheckKeyDown(key);
    }

    bool InputManager::isKeyReleased(Key key)
    {
        return sKeyData.contains(key) && sKeyData[key].CurState == KeyState::Released;
    }

    void InputManager::UpdateMouseButtonState(MouseButton mb, KeyState newState)
    {
        auto& [MouseButton, CurState, OldState] = sMouseData[mb];
        MouseButton = mb;
        OldState = CurState;
        CurState = newState;

        // TODO: 接入事件系统, Trigger事件
        LOG_DEBUG(std::format("[{}] Key {} {}.", __FUNCTION__, static_cast<int>(mb), static_cast<int>(newState)));
    }

    bool InputManager::IsMouseButtonPressed(MouseButton mb)
    {
        return sMouseData.contains(mb) && sMouseData[mb].CurState == KeyState::Pressed;
    }

    bool InputManager::isMouseButtonHeld(MouseButton mb)
    {
        return sMouseData.contains(mb) && sMouseData[mb].CurState == KeyState::Held;
    }

    bool InputManager::isMouseButtonDown(MouseButton mb)
    {
        return WindowManager::GetInstance()->CheckMouseDown(mb);
    }

    bool InputManager::isMouseButtonReleased(MouseButton mb)
    {
        return sMouseData.contains(mb) && sMouseData[mb].CurState == KeyState::Released;
    }
}
