#pragma once

#include <cstdint>

namespace ZeroEngine
{
    // ref: glfw3.h
#pragma region 键盘 Keyboard
    using Key = enum class KeyCode : uint16_t
    {
        /* 可打印字符 */
        Space = 32,
        Apostrophe = 39, /* ' */
        Comma = 44, /* , */
        Minus = 45, /* - */
        Period = 46, /* . */
        Slash = 47, /* / */

        D0 = 48, /* 0 */
        D1 = 49, /* 1 */
        D2 = 50, /* 2 */
        D3 = 51, /* 3 */
        D4 = 52, /* 4 */
        D5 = 53, /* 5 */
        D6 = 54, /* 6 */
        D7 = 55, /* 7 */
        D8 = 56, /* 8 */
        D9 = 57, /* 9 */

        Semicolon = 59, /* ; */
        Equal = 61, /* = */

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        LeftBracket = 91, /* [ */
        Backslash = 92, /* \ */
        RightBracket = 93, /* ] */
        GraveAccent = 96, /* ` */

        World1 = 161, /* non-US #1 */
        World2 = 162, /* non-US #2 */

        /* 功能键 */
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        /* 小键盘 */
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };

    enum class KeyState
    {
        None = -1,
        Pressed,
        Held,
        Released
    };
#pragma endregion
#pragma region 鼠标 Mouse
    using MouseButton = enum class MouseButtonCode : uint16_t
    {
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,

        Left = Button0,
        Right = Button1,
        Middle = Button2
    };

    enum class CursorMode
    {
        Normal = 0,
        Hidden = 1,
        Locked = 2
    };
#pragma endregion
}

/* 可打印字符 */
#define ZERO_KEY_SPACE           ::ZeroEngine::Key::Space
#define ZERO_KEY_APOSTROPHE      ::ZeroEngine::Key::Apostrophe    /* ' */
#define ZERO_KEY_COMMA           ::ZeroEngine::Key::Comma         /* , */
#define ZERO_KEY_MINUS           ::ZeroEngine::Key::Minus         /* - */
#define ZERO_KEY_PERIOD          ::ZeroEngine::Key::Period        /* . */
#define ZERO_KEY_SLASH           ::ZeroEngine::Key::Slash         /* / */
#define ZERO_KEY_0               ::ZeroEngine::Key::D0
#define ZERO_KEY_1               ::ZeroEngine::Key::D1
#define ZERO_KEY_2               ::ZeroEngine::Key::D2
#define ZERO_KEY_3               ::ZeroEngine::Key::D3
#define ZERO_KEY_4               ::ZeroEngine::Key::D4
#define ZERO_KEY_5               ::ZeroEngine::Key::D5
#define ZERO_KEY_6               ::ZeroEngine::Key::D6
#define ZERO_KEY_7               ::ZeroEngine::Key::D7
#define ZERO_KEY_8               ::ZeroEngine::Key::D8
#define ZERO_KEY_9               ::ZeroEngine::Key::D9
#define ZERO_KEY_SEMICOLON       ::ZeroEngine::Key::Semicolon     /* ; */
#define ZERO_KEY_EQUAL           ::ZeroEngine::Key::Equal         /* = */
#define ZERO_KEY_A               ::ZeroEngine::Key::A
#define ZERO_KEY_B               ::ZeroEngine::Key::B
#define ZERO_KEY_C               ::ZeroEngine::Key::C
#define ZERO_KEY_D               ::ZeroEngine::Key::D
#define ZERO_KEY_E               ::ZeroEngine::Key::E
#define ZERO_KEY_F               ::ZeroEngine::Key::F
#define ZERO_KEY_G               ::ZeroEngine::Key::G
#define ZERO_KEY_H               ::ZeroEngine::Key::H
#define ZERO_KEY_I               ::ZeroEngine::Key::I
#define ZERO_KEY_J               ::ZeroEngine::Key::J
#define ZERO_KEY_K               ::ZeroEngine::Key::K
#define ZERO_KEY_L               ::ZeroEngine::Key::L
#define ZERO_KEY_M               ::ZeroEngine::Key::M
#define ZERO_KEY_N               ::ZeroEngine::Key::N
#define ZERO_KEY_O               ::ZeroEngine::Key::O
#define ZERO_KEY_P               ::ZeroEngine::Key::P
#define ZERO_KEY_Q               ::ZeroEngine::Key::Q
#define ZERO_KEY_R               ::ZeroEngine::Key::R
#define ZERO_KEY_S               ::ZeroEngine::Key::S
#define ZERO_KEY_T               ::ZeroEngine::Key::T
#define ZERO_KEY_U               ::ZeroEngine::Key::U
#define ZERO_KEY_V               ::ZeroEngine::Key::V
#define ZERO_KEY_W               ::ZeroEngine::Key::W
#define ZERO_KEY_X               ::ZeroEngine::Key::X
#define ZERO_KEY_Y               ::ZeroEngine::Key::Y
#define ZERO_KEY_Z               ::ZeroEngine::Key::Z
#define ZERO_KEY_LEFT_BRACKET    ::ZeroEngine::Key::LeftBracket   /* [ */
#define ZERO_KEY_BACKSLASH       ::ZeroEngine::Key::Backslash     /* \ */
#define ZERO_KEY_RIGHT_BRACKET   ::ZeroEngine::Key::RightBracket  /* ] */
#define ZERO_KEY_GRAVE_ACCENT    ::ZeroEngine::Key::GraveAccent   /* ` */
#define ZERO_KEY_WORLD_1         ::ZeroEngine::Key::World1        /* non-US #1 */
#define ZERO_KEY_WORLD_2         ::ZeroEngine::Key::World2        /* non-US #2 */

/* 功能键 */
#define ZERO_KEY_ESCAPE          ::ZeroEngine::Key::Escape
#define ZERO_KEY_ENTER           ::ZeroEngine::Key::Enter
#define ZERO_KEY_TAB             ::ZeroEngine::Key::Tab
#define ZERO_KEY_BACKSPACE       ::ZeroEngine::Key::Backspace
#define ZERO_KEY_INSERT          ::ZeroEngine::Key::Insert
#define ZERO_KEY_DELETE          ::ZeroEngine::Key::Delete
#define ZERO_KEY_RIGHT           ::ZeroEngine::Key::Right
#define ZERO_KEY_LEFT            ::ZeroEngine::Key::Left
#define ZERO_KEY_DOWN            ::ZeroEngine::Key::Down
#define ZERO_KEY_UP              ::ZeroEngine::Key::Up
#define ZERO_KEY_PAGE_UP         ::ZeroEngine::Key::PageUp
#define ZERO_KEY_PAGE_DOWN       ::ZeroEngine::Key::PageDown
#define ZERO_KEY_HOME            ::ZeroEngine::Key::Home
#define ZERO_KEY_END             ::ZeroEngine::Key::End
#define ZERO_KEY_CAPS_LOCK       ::ZeroEngine::Key::CapsLock
#define ZERO_KEY_SCROLL_LOCK     ::ZeroEngine::Key::ScrollLock
#define ZERO_KEY_NUM_LOCK        ::ZeroEngine::Key::NumLock
#define ZERO_KEY_PRINT_SCREEN    ::ZeroEngine::Key::PrintScreen
#define ZERO_KEY_PAUSE           ::ZeroEngine::Key::Pause
#define ZERO_KEY_F1              ::ZeroEngine::Key::F1
#define ZERO_KEY_F2              ::ZeroEngine::Key::F2
#define ZERO_KEY_F3              ::ZeroEngine::Key::F3
#define ZERO_KEY_F4              ::ZeroEngine::Key::F4
#define ZERO_KEY_F5              ::ZeroEngine::Key::F5
#define ZERO_KEY_F6              ::ZeroEngine::Key::F6
#define ZERO_KEY_F7              ::ZeroEngine::Key::F7
#define ZERO_KEY_F8              ::ZeroEngine::Key::F8
#define ZERO_KEY_F9              ::ZeroEngine::Key::F9
#define ZERO_KEY_F10             ::ZeroEngine::Key::F10
#define ZERO_KEY_F11             ::ZeroEngine::Key::F11
#define ZERO_KEY_F12             ::ZeroEngine::Key::F12
#define ZERO_KEY_F13             ::ZeroEngine::Key::F13
#define ZERO_KEY_F14             ::ZeroEngine::Key::F14
#define ZERO_KEY_F15             ::ZeroEngine::Key::F15
#define ZERO_KEY_F16             ::ZeroEngine::Key::F16
#define ZERO_KEY_F17             ::ZeroEngine::Key::F17
#define ZERO_KEY_F18             ::ZeroEngine::Key::F18
#define ZERO_KEY_F19             ::ZeroEngine::Key::F19
#define ZERO_KEY_F20             ::ZeroEngine::Key::F20
#define ZERO_KEY_F21             ::ZeroEngine::Key::F21
#define ZERO_KEY_F22             ::ZeroEngine::Key::F22
#define ZERO_KEY_F23             ::ZeroEngine::Key::F23
#define ZERO_KEY_F24             ::ZeroEngine::Key::F24
#define ZERO_KEY_F25             ::ZeroEngine::Key::F25

/* 小键盘 */
#define ZERO_KEY_KP_0            ::ZeroEngine::Key::KP0
#define ZERO_KEY_KP_1            ::ZeroEngine::Key::KP1
#define ZERO_KEY_KP_2            ::ZeroEngine::Key::KP2
#define ZERO_KEY_KP_3            ::ZeroEngine::Key::KP3
#define ZERO_KEY_KP_4            ::ZeroEngine::Key::KP4
#define ZERO_KEY_KP_5            ::ZeroEngine::Key::KP5
#define ZERO_KEY_KP_6            ::ZeroEngine::Key::KP6
#define ZERO_KEY_KP_7            ::ZeroEngine::Key::KP7
#define ZERO_KEY_KP_8            ::ZeroEngine::Key::KP8
#define ZERO_KEY_KP_9            ::ZeroEngine::Key::KP9
#define ZERO_KEY_KP_DECIMAL      ::ZeroEngine::Key::KPDecimal
#define ZERO_KEY_KP_DIVIDE       ::ZeroEngine::Key::KPDivide
#define ZERO_KEY_KP_MULTIPLY     ::ZeroEngine::Key::KPMultiply
#define ZERO_KEY_KP_SUBTRACT     ::ZeroEngine::Key::KPSubtract
#define ZERO_KEY_KP_ADD          ::ZeroEngine::Key::KPAdd
#define ZERO_KEY_KP_ENTER        ::ZeroEngine::Key::KPEnter
#define ZERO_KEY_KP_EQUAL        ::ZeroEngine::Key::KPEqual

#define ZERO_KEY_LEFT_SHIFT      ::ZeroEngine::Key::LeftShift
#define ZERO_KEY_LEFT_CONTROL    ::ZeroEngine::Key::LeftControl
#define ZERO_KEY_LEFT_ALT        ::ZeroEngine::Key::LeftAlt
#define ZERO_KEY_LEFT_SUPER      ::ZeroEngine::Key::LeftSuper
#define ZERO_KEY_RIGHT_SHIFT     ::ZeroEngine::Key::RightShift
#define ZERO_KEY_RIGHT_CONTROL   ::ZeroEngine::Key::RightControl
#define ZERO_KEY_RIGHT_ALT       ::ZeroEngine::Key::RightAlt
#define ZERO_KEY_RIGHT_SUPER     ::ZeroEngine::Key::RightSuper
#define ZERO_KEY_MENU            ::ZeroEngine::Key::Menu

/* 鼠标按键 */
#define ZERO_MOUSE_BUTTON_LEFT    ::ZeroEngine::MouseButton::Left
#define ZERO_MOUSE_BUTTON_RIGHT   ::ZeroEngine::MouseButton::Right
#define ZERO_MOUSE_BUTTON_MIDDLE  ::ZeroEngine::MouseButton::Middle
