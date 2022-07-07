#include "EditorCamera.h"

void EditorCamera::InternalStart()
{
	_camera = _owner->GetComponent<Camera>();
	_transform = _owner->GetComponent<Transform>();
}

void EditorCamera::InternalUpdate()
{
	static glm::vec2 lastMousePos = glm::vec2(0.f);

	const glm::vec2 mousePos = Input::GetMousePosition();
	const glm::vec2 deltas = glm::vec2(mousePos.x - lastMousePos.x, mousePos.y - lastMousePos.y);

	constexpr float sensivity = 0.05f;

	_transform->Rotate(glm::vec3(deltas.x, deltas.y, 0.f) * sensivity);

	lastMousePos = mousePos;


	if (Input::GetKey(Key::A, KeyState::Press))
		_transform->Translate(glm::vec3(-1.f, 0.f, 0.f));
	
	if (Input::GetKey(Key::D, KeyState::Press))
		_transform->Translate(glm::vec3(1.f, 0.f, 0.f));
	
	if (Input::GetKey(Key::W, KeyState::Press))
		_transform->Translate(glm::vec3(0.f, 0.f, -1.f));
	
	if (Input::GetKey(Key::S, KeyState::Press))
		_transform->Translate(glm::vec3(0.f, 0.f, 1.f));

	_camera->Update();
}

const Varlet::Texture* EditorCamera::GetRendereTexture() const
{
	return _camera->GetRendereTexture();
}

const Transform* EditorCamera::GetTransform() const
{
	return _transform;;
}
