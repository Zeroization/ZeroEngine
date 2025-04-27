#include "CubeMap.h"
#include "Function/Render/RHI/RHI.h"

namespace ZeroEngine
{
    CubeMap::CubeMap(const std::vector<std::string>& path)
    {
        mID = RHI::GetInstance().LoadCubeMap(path);
    }

    CubeMap::~CubeMap()
    {
        RHI::GetInstance().DeleteTexture(mID);
    }

    uint32_t CubeMap::GetID() const
    {
        return mID;
    }
} // ZeroEngine
