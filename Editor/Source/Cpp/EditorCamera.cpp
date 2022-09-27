#include "EditorCamera.h"
#include "Rendering/Material.h"

void EditorCamera::InternalStart()
{
	_camera = GetOwner()->GetComponent<Camera>();
	_camera->postProcessing.enable = false;
	_camera->postProcessing.material = new Material(Varlet::RendererAPI::CreateShader({ "../Varlet/Shaders/defaultVertexPostProcessing.glsl", "../Varlet/Shaders/defaultFragmentPostProcessing.glsl" }));

	_selectedCamera = GetOwner()->AddComponent<Camera>();

	_selectedCamera->postProcessing.enable = false;
	_selectedCamera->SetRenderShader(Varlet::RendererAPI::CreateShader({ "", "../Editor/Shaders/selected.fragment.glsl" }));

	FramebufferConfiguration framebufferConfiguration;
	framebufferConfiguration.textureConfiguration.wrapType = WrapType::Repeat;
	framebufferConfiguration.textureConfiguration.filter = FilterType::Linear;
	framebufferConfiguration.textureConfiguration.format = TextureFormat::RGBA;
	framebufferConfiguration.textureConfiguration.mipmap = false;

	_selectedCamera->SetFramebufferConfiguration(framebufferConfiguration);
	_selectedCamera->SetActive(true);

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

const Varlet::Texture* EditorCamera::GetRendereTexture() const
{
	return _camera->GetTargetTexture();
}

uint8_t* EditorCamera::ReadSelectedPixel(const int32_t& x, const int32_t& y) const
{
	return static_cast<uint8_t*>(_selectedCamera->GetFramebuffer()->ReadPixels(x, y));
}

void EditorCamera::OnResize(const int32_t& width, const int32_t& height) const
{
	_camera->ResizeView(width, height);
	_selectedCamera->ResizeView(width, height);
}