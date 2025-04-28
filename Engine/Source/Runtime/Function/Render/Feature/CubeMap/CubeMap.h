#pragma once

namespace ZeroEngine
{
    class CubeMap
    {
    public:
        CubeMap(const std::vector<std::string>& path);
        virtual ~CubeMap();

        uint32_t GetID() const;

    private:
        uint32_t mID;
    };
} // ZeroEngine
