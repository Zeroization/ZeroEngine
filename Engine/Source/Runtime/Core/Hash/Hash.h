#pragma once

namespace ZeroEngine
{
    class StrHash
    {
    public:
        static uint32_t FNVHash(std::string_view str);
        static uint32_t FNVHash(const char* data, size_t length);

        static uint32_t CRC32Hash(std::string_view str);
        static uint32_t CRC32Hash(const char* data, size_t length);
    };
}
