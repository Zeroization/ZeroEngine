#include "pch.h"
#include "UUID.h"

#include <random>

namespace ZeroEngine
{
    static std::random_device s_RandomDevice;

    static std::mt19937_64 s_UUIDEngine(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

    UUID::UUID() : m_UUID(s_UniformDistribution(s_UUIDEngine))
    {
    }

    UUID::UUID(uint64_t uuid) : m_UUID(uuid)
    {
    }

    UUID::UUID(const UUID& copy) : m_UUID(copy.m_UUID)
    {
    }

    static std::mt19937 s_UUID32Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint32_t> s_UniformDistribution32;

    UUID32::UUID32() : m_UUID(s_UniformDistribution32(s_UUID32Engine))
    {
    }

    UUID32::UUID32(uint32_t uuid) : m_UUID(uuid)
    {
    }

    UUID32::UUID32(const UUID32& copy) : m_UUID(copy.m_UUID)
    {
    }
} // ZeroEngine
