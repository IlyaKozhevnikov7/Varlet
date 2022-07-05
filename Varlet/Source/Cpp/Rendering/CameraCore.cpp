#include "CameraCore.h"
#include "RendererAPI.h"

namespace Varlet
{
    CameraCore::CameraCore()
    {
        _projection = glm::perspective(glm::radians(90.f), 640.f / 480.f, 0.1f, 250.f);
        _framebuffer = RendererAPI::CreateFrameBuffer();
    }

    CameraCore::~CameraCore()
    {
        delete _framebuffer;
    }

    void CameraCore::Bind() const
    {
        _framebuffer->Bind();
    }

    void CameraCore::UnBind() const
    {
        _framebuffer->UnBind();
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
        return _projection * _view;
    }

    const uint32_t& CameraCore::GetRendereTexture() const
    {
        return _framebuffer->GetTexture();
    }
}
