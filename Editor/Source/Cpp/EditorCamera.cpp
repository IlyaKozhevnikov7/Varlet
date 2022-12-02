#include "EditorCamera.h"
#include "Rendering/Material.h"
#include "Rendering/RendererAPI.h"

bool s = false;

void EditorCamera::InternalStart()
{
	_camera = GetOwner()->GetComponent<Camera>();
	_camera->dynamicResolution = true;
	_camera->postProcessing.enable = false;
	_camera->postProcessing.material = new Material(Varlet::RendererAPI::CreateShader({ "W:/Varlet/Varlet/Shaders/defaultVertexPostProcessing.glsl", "W:/Varlet/Varlet/Shaders/defaultFragmentPostProcessing.glsl" }));

	_selectedCamera = GetOwner()->AddComponent<Camera>();
	_selectedCamera->dynamicResolution = true;
	_selectedCamera->postProcessing.enable = false;
	_selectedCamera->SetRenderShader(Varlet::RendererAPI::CreateShader({ "", "W:/Varlet/Editor/Shaders/selected.fragment.glsl" }));

	_transform = GetOwner()->GetComponent<Transform>();

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

	_transform->Rotate(glm::vec3(delta, 0.f));

	_lastMousePos = mousePos;

	if (Input::GetKey(Key::A, KeyState::Press))
		_transform->Translate(-_transform->GetRight() * _speed * Time::GetDeltaTime());

	if (Input::GetKey(Key::D, KeyState::Press))
		_transform->Translate(_transform->GetRight() * _speed * Time::GetDeltaTime());

	if (Input::GetKey(Key::W, KeyState::Press))
		_transform->Translate(-_transform->GetForward() * _speed * Time::GetDeltaTime());

	if (Input::GetKey(Key::S, KeyState::Press))
		_transform->Translate(_transform->GetForward() * _speed * Time::GetDeltaTime());

	s = Input::GetKey(Key::Q, KeyState::Press);
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

const Camera* EditorCamera::GetCore() const
{
	return _camera;
}

const void* EditorCamera::GetRendereTexture() const
{
	return Varlet::RendererAPI::GetNativeRenderTexture(_camera);
}

std::vector<uint8_t> EditorCamera::ReadSelectedPixel(const int32_t& x, const int32_t& y) const
{
	int32_t renderResolutionWidth, renderResolutionHeight, resolutionWidth, resolutionHeight;

	Varlet::RendererAPI::GetFramebufferSize(_selectedCamera, renderResolutionWidth, renderResolutionHeight);
	_selectedCamera->GetResolution(resolutionWidth, resolutionHeight);
	
	const glm::ivec2 correctPixel =
	{
		static_cast<float>(x) * (static_cast<float>(renderResolutionWidth) / static_cast<float>(resolutionWidth)),
		static_cast<float>(y) * (static_cast<float>(renderResolutionHeight) / static_cast<float>(resolutionHeight))
	};

	return Varlet::RendererAPI::ReadRenderTexturePixels(_selectedCamera, correctPixel.x, correctPixel.y);
}

void EditorCamera::OnResize(const int32_t& width, const int32_t& height) const
{
	_camera->ChangeResolution(width, height);
	_selectedCamera->ChangeResolution(width, height);
}