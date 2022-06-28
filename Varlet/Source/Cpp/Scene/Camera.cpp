#include "Camera.h"
#include "RenderingCore.h"
#include "Transform.h"
#include "Entity.h"

Camera::Camera()
{
	_core = Varlet::RendererAPI::CreateCameraCore();
}

void Camera::Start()
{
	transform = _owner->GetComponent<Transform>();
}

void Camera::Update()
{
	_core->CulculateView(transform->_position, transform->_rotation);
}
