#pragma once
#include <cstdint>

namespace ZeroEngine
{
    enum class EditorPanelEdgeFlag : uint8_t
    {
        None   = 0b00000000,
        Left   = 0b00000001,
        Right  = 0b00000010,
        Top    = 0b00000100,
        Bottom = 0b00001000
    };

    inline uint8_t operator|=(uint8_t& lhs, EditorPanelEdgeFlag rhs)
    {
        lhs |= static_cast<uint8_t>(rhs);
        return lhs;
    }
}
