#include "RHI.h"
#if defined(ZERO_GRAPHIC_OPENGL)
#include "Function/Render/RHI/OpenGL/RHI_OpenGLImpl.h"
#endif

namespace ZeroEngine
{
    std::shared_ptr<RHI> RHI::Instance = nullptr;

    void RHI::Create()
    {
#if defined(ZERO_GRAPHIC_OPENGL)
        Instance = std::make_shared<RHI_OpenGLImpl>();
#endif
    }

    std::shared_ptr<RHI> RHI::GetInstance()
    {
        return Instance;
    }
} // ZeroEngine
