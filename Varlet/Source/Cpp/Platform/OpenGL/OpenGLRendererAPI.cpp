#include "OpenGLRendererAPI.h"
#include "OpenGLCameraCore.h"
#include "OpenGLUniformBuffer.h"

namespace Varlet
{
    CameraCore* OpenGLRendererAPI::CreateCameraCore() const
    {
        return new OpenGLCameraCore();
    }

    UniformBuffer* OpenGLRendererAPI::CreateUniformBuffer(const int64_t& size) const
    {
        return new OpenGLUniformBuffer(size);
    }
}
