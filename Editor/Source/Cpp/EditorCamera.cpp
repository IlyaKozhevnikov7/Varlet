#include "EditorCamera.h"

void EditorCamera::InternalStart()
{
	_camera = _owner->GetComponent<Camera>();
	_transform = _owner->GetComponent<Transform>();
}

void EditorCamera::InternalUpdate()
{
	const glm::vec2 mousePos = Input::GetMousePosition();
	static glm::vec2 lastMousePos = mousePos;

	const glm::vec2 delta = glm::vec2(ConstrainPitch(lastMousePos.y - mousePos.y), (lastMousePos.x - mousePos.x) * sensitivity);

	_transform->Rotate(delta.x, glm::vec3(1.f, 0.f, 0.f));
	_transform->Rotate(delta.y, glm::vec3(0.f, 1.f, 0.f), Space::World);

	lastMousePos = mousePos;

	constexpr float speed = 10.f;
	
	if (Input::GetKey(Key::A, KeyState::Press))
		_transform->Translate(-_transform->GetRight() * speed * Time::GetDeltaTime());
	
	if (Input::GetKey(Key::D, KeyState::Press))
		_transform->Translate(_transform->GetRight() * speed * Time::GetDeltaTime());
	
	if (Input::GetKey(Key::W, KeyState::Press))
		_transform->Translate(-_transform->GetForward() * speed * Time::GetDeltaTime());
	
	if (Input::GetKey(Key::S, KeyState::Press))
		_transform->Translate(_transform->GetForward() * speed * Time::GetDeltaTime());

	_camera->Update();
}

const Varlet::Texture* EditorCamera::GetRendereTexture() const
{
	return _camera->GetRendereTexture();
}

Transform* EditorCamera::GetTransform() const
{
	return _transform;
}

float EditorCamera::ConstrainPitch(const float& mouseDelta) const
{
	constexpr glm::vec2 pitchConstraint(-90.f, 90.f);
	static float lastPitch = _transform->GetEulerAngles().x;

	float delta = mouseDelta * sensitivity;
	float totalPitch = lastPitch + delta;

	if (totalPitch < pitchConstraint.x)
	{
		const float resultDelta = pitchConstraint.x - lastPitch;
		lastPitch = pitchConstraint.x;
		return resultDelta;
	}
	else if (totalPitch > pitchConstraint.y)
	{
		const float resultDelta = pitchConstraint.y - lastPitch;
		lastPitch = pitchConstraint.y;
		return resultDelta;
	}

	lastPitch += delta;
	return delta;
}
