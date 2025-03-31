#include "GlfwWindowManager.h"

#include "Function/Input/InputManager.h"

namespace ZeroEngine
{
    GlfwWindowManager::GlfwWindowManager()
    {
        if (!glfwInit())
        {
            LOG_CRITICAL("[GLFW] Failed to Initialize GLFW Window!");
            return;
        }

        std::string windowName = "ZeroEngine";
        std::string graphAPIName;

#if defined(ZERO_GRAPHIC_OPENGL)
        windowName += " <OpenGL 4.6>";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

        // TODO: EngineConfig写好后覆盖设置
        WindowPtr = glfwCreateWindow(1280, 720, windowName.c_str(), nullptr, nullptr);
        if (WindowPtr == nullptr)
        {
            const char* errDesc;
            int errCode = glfwGetError(&errDesc);
            LOG_CRITICAL(
                std::format("[GLFW] Failed to create GLFW window.\ncode: {}\nDescription: {}", errCode,
                    errDesc));
            glfwTerminate();
            return;
        }

#if defined(ZERO_GRAPHIC_OPENGL)
        glfwMakeContextCurrent(WindowPtr);
#endif

        // 启用垂直同步
        glfwSwapInterval(1);

        // 绑定GLFW报错回调
        glfwSetErrorCallback([](int error, const char* desc) -> void
        {
            LOG_ERROR(std::format("[GLFW] Error code: {}, description: {}", error, desc));
        });

        // 绑定键盘按键回调
        glfwSetKeyCallback(WindowPtr, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    InputManager::GetInstance()->UpdateKeyState(static_cast<KeyCode>(key), KeyState::Pressed);
                    break;
                }
                case GLFW_RELEASE:
                {
                    InputManager::GetInstance()->UpdateKeyState(static_cast<KeyCode>(key), KeyState::Released);
                    break;
                }
                case GLFW_REPEAT:
                {
                    InputManager::GetInstance()->UpdateKeyState(static_cast<KeyCode>(key), KeyState::Held);
                    break;
                }
                default:
                {
                    LOG_ERROR(std::format("[GLFW] Unknown keyboard input action, code {}!", key));
                }
            }
        });

        // 是否支持原生鼠标
        if (glfwRawMouseMotionSupported() == GLFW_TRUE)
        {
            glfwSetInputMode(WindowPtr, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        } else
        {
            LOG_WARN("[GLFW] This platform doesn't support raw mouse motion.");
        }

        // 绑定鼠标按键回调
        glfwSetMouseButtonCallback(WindowPtr, [](GLFWwindow* window, int button, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    InputManager::GetInstance()->UpdateMouseButtonState(static_cast<MouseButton>(button),
                                                                        KeyState::Pressed);
                    break;
                }
                case GLFW_RELEASE:
                {
                    InputManager::GetInstance()->UpdateMouseButtonState(static_cast<MouseButton>(button),
                                                                        KeyState::Released);
                    break;
                }
                default:
                {
                    LOG_ERROR(std::format("[GLFW] Unknown mouse button input action, code {}!", button));
                }
            }
        });
    }

    GlfwWindowManager::~GlfwWindowManager()
    {
        glfwDestroyWindow(WindowPtr);
        glfwTerminate();
    }

    void *GlfwWindowManager::GetCurrentContext()
    {
        return glfwGetCurrentContext();
    }

    void *GlfwWindowManager::GetWindowPtr()
    {
        return WindowPtr;
    }

    float GlfwWindowManager::GetCurFrameTime()
    {
        return glfwGetTime();
    }

    void GlfwWindowManager::CloseWindow()
    {
        glfwSetWindowShouldClose(WindowPtr, true);
    }

    bool GlfwWindowManager::WindowShouldClose()
    {
        return glfwWindowShouldClose(WindowPtr) == 1;
    }

    void GlfwWindowManager::Update()
    {
        glfwPollEvents();
    }

    std::pair<float, float> GlfwWindowManager::GetMousePos()
    {
        // TODO: 后续可能要改成如下情况：
        //  1. 编辑器模式: 以视口为坐标系, 视口边界为(0, 0), 而不是整个窗口
        //  2. 游戏模式(非编辑器): 窗口边界为(0, 0)

        double x, y;
        glfwGetCursorPos(WindowPtr, &x, &y);

        return {static_cast<float>(x), static_cast<float>(y)};
    }

    bool GlfwWindowManager::CheckKeyDown(Key key)
    {
#ifdef ZERO_EDITOR
        // TODO
#else
        auto state = glfwGetKey(WindowPtr, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
#endif
        return false;
    }

    bool GlfwWindowManager::CheckMouseDown(MouseButton mb)
    {
        // TODO: 抄Hazel
        return false;
    }
} // ZeroEngine
