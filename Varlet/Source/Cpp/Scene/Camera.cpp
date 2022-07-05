#include "Camera.h"
#include "RenderingCore.h"
#include "Transform.h"
#include "Entity.h"

Camera::Camera()
{
	_core = Varlet::RendererAPI::CreateCameraCore();
}

void Camera::Update()
{
	static auto transform = _owner->GetComponent<Transform>();
	_core->CulculateView(transform->_position, transform->_rotation);
}

const glm::mat4& Camera::GetView() const
{
	return _core->GetView();
}

const glm::mat4& Camera::GetProjection() const
{
	return _core->GetProjection();
}

const glm::mat4& Camera::GetViewProjection() const
{
	return _core->GetViewProjection();
}

const Varlet::CameraCore* Camera::GetCore() const
{
	return _core;
}

const uint32_t& Camera::GetRendereTexture() const
{
	return _core->GetRendereTexture();
}
