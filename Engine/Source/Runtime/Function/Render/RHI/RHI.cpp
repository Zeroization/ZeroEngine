#include "RHI.h"
#if defined(ZERO_GRAPHIC_OPENGL)
#include "Function/Render/RHI/OpenGL/RHI_OpenGLImpl.h"
#endif

#include "Core/Event/EventManager.h"

namespace ZeroEngine
{
    void RHI::Create()
    {
        GetInstance();

        // EventMgr绑定相关事件
        auto& eventMgr = EventManager::GetInstance();

        EventMetaData windowResizeEventMetadata{};
        windowResizeEventMetadata.Type = EventType::Builtin_WindowResize;
        windowResizeEventMetadata.Category = static_cast<uint8_t>(EventCategory::Builtin_WindowEvent);
        windowResizeEventMetadata.Priority = EventPriority::Medium;
        eventMgr.BindListener<RHI, &RHI::OnEvent>(GetInstance(), windowResizeEventMetadata);
    }

    RHI& RHI::GetInstance()
    {
#if defined(ZERO_GRAPHIC_OPENGL)
        static RHI_OpenGLImpl sInstance;
#elif
        ZERO_CORE_ASSERT(false, "Other API\'s RHI todo");
#endif
        return sInstance;
    }
} // ZeroEngine
