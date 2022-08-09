#pragma once

#include "VarletCore.h"
#include "Script.h"
#include "Rendering/RenderingCore.h"

class Camera final : public Script
{
private:

	glm::mat4 _view;
	glm::mat4 _projection;

	float _fov;
	glm::vec2 _resolution;

	Varlet::Framebuffer* _framebuffer;
	Varlet::Shader* _renderShader;

	FramebufferConfiguration _framebufferConfiguration;

public:

	CORE_API Camera();

	~Camera();

	CORE_API void Update() override;

	CORE_API const Varlet::Framebuffer* GetFramebuffer() const;

	CORE_API const glm::mat4& GetView() const;

	CORE_API const glm::mat4& GetProjection() const;

	CORE_API const glm::mat4 GetViewProjection() const;

	CORE_API const Varlet::Texture* GetTargetTexture() const;

	CORE_API void SetFramebufferConfiguration(const FramebufferConfiguration& configuration);

	CORE_API void ResizeView(const int32_t& width, const int32_t& height);

	CORE_API void GetResolution(int32_t& width, int32_t& height) const;

	CORE_API void SetRenderShader(Varlet::Shader* shader);

	void Bind() const;

	void UnBind() const;

	Varlet::Shader* GetRenderShader();

private:

	void CulculateView(const glm::vec3& position, const glm::quat& rotation);

};

