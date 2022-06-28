#include "OpenGLRendererAPI.h"
#include "OpenGLCameraCore.h"

namespace Varlet
{
    CameraCore* OpenGLRendererAPI::CreateCameraCore() const
    {
        return new OpenGLCameraCore();
    }
}
