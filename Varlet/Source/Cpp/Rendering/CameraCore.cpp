#include "CameraCore.h"
#include "RendererAPI.h"

namespace Varlet
{
    CameraCore::CameraCore()
    {
        _projection = glm::perspective(glm::radians(45.f), 960.f / 540.f, 0.1f, 250.f);
        _framebuffer = RendererAPI::CreateFrameBuffer(960, 540);
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

    const Texture* CameraCore::GetRendereTexture() const
    {
        return _framebuffer->GetTexture();
    }

    void CameraCore::ResizeView(const int32_t& width, const int32_t& height)
    {
        delete _framebuffer;
        _framebuffer = RendererAPI::CreateFrameBuffer(width, height);
    }
}
