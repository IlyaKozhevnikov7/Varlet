#include "CameraCore.h"
#include "RendererAPI.h"

namespace Varlet
{
    CameraCore::CameraCore()
    {
        // init _framebuffer
    }

    const glm::mat4& CameraCore::GetView() const
    {
        return _view;
    }

    const glm::mat4& CameraCore::GetProjection() const
    {
        return _projection;
    }

    const glm::mat4 CameraCore::GetViewProjection() const
    {
        return _view * _projection;
    }
}
