#include "EditorCamera.h"

void EditorCamera::InternalStart()
{
	_camera = _owner->GetComponent<Camera>();
	_transform = _owner->GetComponent<Transform>();
	_transform->Rotate(glm::vec3(0.f, 0.f, 1.f));
}

void EditorCamera::InternalUpdate()
{
	VARLET_LOG(LevelType::Normal, "EditorCamera::InternalUpdate()");

	if (Input::GetKey(Key::A, KeyState::Press))
		_transform->Translate(glm::vec3(0.f, 0.f, 1.f));
	
	if (Input::GetKey(Key::D, KeyState::Press))
		_transform->Translate(glm::vec3(0.f, 0.f, -1.f));
	
	if (Input::GetKey(Key::W, KeyState::Press))
		_transform->Translate(glm::vec3(0.f, 1.f, 0.f));
	
	if (Input::GetKey(Key::S, KeyState::Press))
		_transform->Translate(glm::vec3(0.f, -1.f, 0.f));

	_camera->Update();
}

const Varlet::Texture* EditorCamera::GetRendereTexture() const
{
	return _camera->GetRendereTexture();
}
