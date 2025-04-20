#include "RHI.h"
#if defined(ZERO_GRAPHIC_OPENGL)
#include "Function/Render/RHI/OpenGL/RHI_OpenGLImpl.h"
#endif

#include "Core/Event/EventManager.h"

namespace ZeroEngine
{
    std::shared_ptr<RHI> RHI::sInstance = nullptr;

    void RHI::Create()
    {
#if defined(ZERO_GRAPHIC_OPENGL)
        sInstance = std::make_shared<RHI_OpenGLImpl>();
#endif

        // EventMgr绑定相关事件
        auto eventMgr = EventManager::GetInstance();

        EventMetaData windowResizeEventMetadata{};
        windowResizeEventMetadata.Type = EventType::Builtin_WindowResize;
        windowResizeEventMetadata.Category = static_cast<uint8_t>(EventCategory::Builtin_WindowEvent);
        windowResizeEventMetadata.Priority = EventPriority::Medium;
        eventMgr->BindListener<RHI, &RHI::OnEvent>(sInstance.get(), windowResizeEventMetadata);
    }

    std::shared_ptr<RHI> RHI::GetInstance()
    {
        return sInstance;
    }
} // ZeroEngine
