#include "Hash.h"

namespace ZeroEngine
{
    uint32_t StrHash::FNVHash(std::string_view str)
    {
        return FNVHash(str.data(), str.length());
    }

    uint32_t StrHash::FNVHash(const char* data, const size_t length)
    {
        constexpr uint32_t FNV_PRIME = 16777619u;
        constexpr uint32_t OFFSET_BASIS = 2166136261u;

        uint32_t hash = OFFSET_BASIS;
        auto dataCopy = data;
        for (size_t i = 0; i < length; ++i)
        {
            hash ^= *dataCopy++;
            hash *= FNV_PRIME;
        }
        hash ^= '\0';
        hash *= FNV_PRIME;

        return hash;
    }

    constexpr auto gen_crc32_table()
    {
        constexpr int num_bytes = 256;
        constexpr int num_iterations = 8;
        constexpr uint32_t polynomial = 0xEDB88320;

        std::array<uint32_t, num_bytes> crc32_table{};

        for (int byte = 0; byte < num_bytes; ++byte)
        {
            uint32_t crc = (uint32_t) byte;
            for (int i = 0; i < num_iterations; ++i)
            {
                int mask = -((int) crc & 1);
                crc = (crc >> 1) ^ (polynomial & mask);
            }

            crc32_table[byte] = crc;
        }

        return crc32_table;
    }

    uint32_t StrHash::CRC32Hash(std::string_view str)
    {
        return CRC32Hash(str.data(), str.length());
    }

    uint32_t StrHash::CRC32Hash(const char* data, const size_t length)
    {
        static constexpr auto crc32_table = gen_crc32_table();
        ZERO_CORE_ASSERT(crc32_table.size() == 256 &&
                         crc32_table[1] == 0x77073096 &&
                         crc32_table[255] == 0x2D02EF8D,
                         "gen_crc32_table generated unexpected result.");

        auto crc = 0xFFFFFFFFu;

        for (size_t i = 0; i < length; ++i)
        {
            crc = crc32_table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
        }

        return ~crc;
    }
}
