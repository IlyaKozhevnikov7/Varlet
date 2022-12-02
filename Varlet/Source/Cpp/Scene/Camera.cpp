#include "Camera.h"
#include "RendererAPI.h"
#include "Transform.h"
#include "Entity.h"

START_PROPERTY_BLOCK(Camera)
PROPERTY("Enable post-processing", postProcessing.enable)
TYPED_PROPERTY("Material", postProcessing.material, Varlet::Type::Object)
END_PROPERTY_BLOCK;

void Camera::OnConstructed()
{
	BaseClass::OnConstructed();

	dynamicResolution = false;
	_fov = 45.f;
	_projection = glm::perspective(glm::radians(_fov), 960.f / 540.f, 0.1f, 250.f);
	_renderShader = nullptr;
	_resolution = { 960.f, 540.f };
}

void Camera::OnDestroyed()
{
	if (_renderShader)
		delete _renderShader;
}

void Camera::Update()
{
	// TODO FIX
	static auto transform = GetOwner()->GetComponent<Transform>();
	CulculateView(transform->position, transform->GetOrientation());
}

const glm::mat4& Camera::GetView() const
{
	return _view;
}

const glm::mat4& Camera::GetProjection() const
{
	return _projection;
}

const glm::mat4 Camera::GetViewProjection() const
{
	return _projection * _view;
}

const Varlet::Texture* Camera::GetTargetTexture()
{
	static auto texture = Varlet::RendererAPI::GetTextureOf(this);
	return texture;
}

void Camera::ChangeResolution(const int32_t& width, const int32_t& height)
{
	if (dynamicResolution == false)
	{
		VARLET_LOG(LevelType::Warning, "Change resolution allow only with enable dynamic resolution");
		return;
	}

	_projection = glm::perspective(glm::radians(_fov), static_cast<float>(width) / static_cast<float>(height), 0.1f, 250.f);
	_resolution.x = width;
	_resolution.y = height;

	Varlet::RendererAPI::ChangeResolution(this);
}

void Camera::GetResolution(int32_t& width, int32_t& height) const
{
	width = _resolution.x;
	height = _resolution.y;
}

void Camera::SetRenderShader(Varlet::Shader* shader)
{
	if (shader == nullptr && _renderShader != nullptr)
		delete _renderShader;
	else
		_renderShader = shader;
}

void Camera::CulculateView(const glm::vec3& position, const glm::quat& orientation)
{
	const glm::mat4 translateMat = glm::translate(glm::mat4(1.f), -position);
	const glm::mat4 orientationMat = glm::mat4_cast(glm::inverse(orientation));

	_view = orientationMat * translateMat;
}

Varlet::Shader* Camera::GetRenderShader()
{
	return _renderShader;
}
