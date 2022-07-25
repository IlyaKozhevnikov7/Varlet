#include "EditorCamera.h"

void EditorCamera::InternalStart()
{
	_camera = _owner->GetComponent<Camera>();
	_transform = _owner->GetComponent<Transform>();

	_lastMousePos = Input::GetMousePosition();
}

void EditorCamera::InternalUpdate()
{
	if (_isControlled)
		UpdateMoveAndRotation();

	_camera->Update();
}

void EditorCamera::UpdateMoveAndRotation()
{
	const glm::vec2 mousePos = Input::GetMousePosition();
	const glm::vec2 delta = glm::vec2(_lastMousePos.y - mousePos.y, _lastMousePos.x - mousePos.x) * _sensitivity;

	_transform->Rotate(delta.x, glm::vec3(1.f, 0.f, 0.f));
	_transform->Rotate(delta.y, glm::vec3(0.f, 1.f, 0.f), Space::World);

	_lastMousePos = mousePos;

	if (Input::GetKey(Key::A, KeyState::Press))
		_transform->Translate(-_transform->GetRight() * _speed * Time::GetDeltaTime());

	if (Input::GetKey(Key::D, KeyState::Press))
		_transform->Translate(_transform->GetRight() * _speed * Time::GetDeltaTime());

	if (Input::GetKey(Key::W, KeyState::Press))
		_transform->Translate(-_transform->GetForward() * _speed * Time::GetDeltaTime());

	if (Input::GetKey(Key::S, KeyState::Press))
		_transform->Translate(_transform->GetForward() * _speed * Time::GetDeltaTime());
}

void EditorCamera::SetControl(const bool&& control)
{
	_isControlled = control;
	
	if (_isControlled)
		_lastMousePos = Input::GetMousePosition();
}

bool EditorCamera::IsControlled() const
{
	return _isControlled;
}

const Varlet::Texture* EditorCamera::GetRendereTexture() const
{
	return _camera->GetRendereTexture();
}

void EditorCamera::OnResize(const int32_t& width, const int32_t& height) const
{
	_camera->ResizeView(width, height);
}

Transform* EditorCamera::GetTransform() const
{
	return _transform;
}
