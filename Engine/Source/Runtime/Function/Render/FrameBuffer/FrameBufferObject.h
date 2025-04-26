#pragma once
#include "pch.h"

namespace ZeroEngine
{
    class FrameBufferObject
    {
    public:
        FrameBufferObject() { mType = FrameBufferType::Reserve; }

        explicit FrameBufferObject(FrameBufferType type) : mType(type)
        {
        }

    public:
        uint32_t mID = 0;
        uint32_t mWidth = 0;
        uint32_t mHeight = 0;

        uint32_t mDepthAttachmentID = UINT32_MAX;
        uint32_t mColorAttachment0ID = UINT32_MAX; // ColorBuffer
        uint32_t mColorAttachment1ID = UINT32_MAX; // GBuffer: PositionBuffer
        uint32_t mColorAttachment2ID = UINT32_MAX; // GBuffer: NormalBuffer

        ClearInfo mClearInfo;

        bool mbIsValid = true;          ///< 该FBO是否合法
        bool mbIsFitWithScreen = false; ///< 该FBO是否要适配屏幕大小
        FrameBufferType mType;
    };
}
