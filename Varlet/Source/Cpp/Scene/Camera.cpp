#include "Camera.h"
#include "RenderingCore.h"
#include "RendererAPI.h"
#include "Transform.h"
#include "Entity.h"

START_PROPERTY_BLOCK(Camera)
PROPERTY("Enable post-processing", postProcessing.enable)
TYPED_PROPERTY("Material", postProcessing.material, Varlet::Type::Object)
END_PROPERTY_BLOCK;

void Camera::OnConstructed()
{
	_fov = 45.f;
	_projection = glm::perspective(glm::radians(_fov), 960.f / 540.f, 0.1f, 250.f);
	_renderShader = nullptr;

	_framebufferConfiguration =
	{
		960,
		540,
		{
			960,
			540,
			WrapType::Repeat,
			FilterType::Linear,
			false
		}
	};

	_framebuffer = Varlet::RendererAPI::CreateFrameBuffer(_framebufferConfiguration);
}

void Camera::OnDestroyed()
{
	delete _framebuffer;

	if (_renderShader)
		delete _renderShader;
}

void Camera::Update()
{
	static auto transform = GetOwner()->GetComponent<Transform>();
	CulculateView(transform->position, transform->rotation);
}

const Varlet::Framebuffer* Camera::GetFramebuffer() const
{
	return _framebuffer;
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

const Varlet::Texture* Camera::GetTargetTexture() const
{
	return _framebuffer->GetTexture();
}

void Camera::ResizeView(const int32_t& width, const int32_t& height)
{
	delete _framebuffer;

	_framebufferConfiguration.width = width;
	_framebufferConfiguration.height = height;

	_framebuffer = Varlet::RendererAPI::CreateFrameBuffer(_framebufferConfiguration);
	_projection = glm::perspective(glm::radians(_fov), static_cast<float>(width) / static_cast<float>(height), 0.1f, 250.f);
	_resolution.x = width;
	_resolution.y = height;
}

void Camera::SetFramebufferConfiguration(const FramebufferConfiguration& configuration)
{
	_framebufferConfiguration.textureConfiguration = configuration.textureConfiguration;

	delete _framebuffer;
	_framebuffer = Varlet::RendererAPI::CreateFrameBuffer(_framebufferConfiguration);
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

void Camera::CulculateView(const glm::vec3& position, const glm::quat& rotation)
{
	const glm::mat4 translateMat = glm::translate(glm::mat4(1.f), -position);
	const glm::mat4 orientationMat = glm::mat4_cast(glm::inverse(rotation));

	_view = orientationMat * translateMat;
}

void Camera::Bind() const
{
	_framebuffer->Bind();
}

void Camera::UnBind() const
{
	_framebuffer->UnBind();
}

Varlet::Shader* Camera::GetRenderShader()
{
	return _renderShader;
}
