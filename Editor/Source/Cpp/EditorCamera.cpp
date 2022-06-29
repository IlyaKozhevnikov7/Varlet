#include "EditorCamera.h"

void EditorCamera::InternalStart()
{
	_transform = _owner->GetComponent<Transform>();
}

void EditorCamera::InternalUpdate()
{
	VARLET_LOG(LevelType::Normal, "EditorCamera::InternalUpdate()");

	if (Input::GetKey(Key::A, KeyState::Press))
		_transform->Translate(glm::vec3(1.f, 0.f, 0.f));
	
	if (Input::GetKey(Key::D, KeyState::Press))
		_transform->Translate(glm::vec3(-1.f, 0.f, 0.f));
	
	if (Input::GetKey(Key::W, KeyState::Press))
		_transform->Translate(glm::vec3(0.f, 1.f, 0.f));
	
	if (Input::GetKey(Key::S, KeyState::Press))
		_transform->Translate(glm::vec3(0.f, -1.f, 0.f));
}
