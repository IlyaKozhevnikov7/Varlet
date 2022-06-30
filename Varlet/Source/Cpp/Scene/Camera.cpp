#include "Camera.h"
#include "RenderingCore.h"
#include "Transform.h"
#include "Entity.h"

void Camera::Start()
{
	_core = Varlet::RendererAPI::CreateCameraCore();
	transform = _owner->GetComponent<Transform>();
}

void Camera::Update()
{
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

const glm::mat4 Camera::GetViewProjection() const
{
	return _core->GetViewProjection();
}
