#pragma once

namespace ZeroEngine
{
    class UUID
    {
    public:
        UUID();
        explicit UUID(uint64_t uuid);
        UUID(const UUID& copy);

        operator uint64_t() { return m_UUID; }
        operator const uint64_t() const { return m_UUID; }

    private:
        uint64_t m_UUID;
    };

    class UUID32
    {
    public:
        UUID32();
        explicit UUID32(uint32_t uuid);
        UUID32(const UUID32& copy);

        operator uint32_t() { return m_UUID; }
        operator const uint32_t() const { return m_UUID; }

    private:
        uint32_t m_UUID;
    };
} // ZeroEngine
