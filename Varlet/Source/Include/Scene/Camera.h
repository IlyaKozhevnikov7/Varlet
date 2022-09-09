#pragma once

#include "VarletCore.h"
#include "Script.h"
#include "Rendering/RenderingCore.h"

struct PostProcessing final
{
	bool enable;
	Material* material;
};

class CORE_API Camera final : public Script
{
	TYPE_GENERATION(Camera, Script)

public:

	PostProcessing postProcessing;

private:

	glm::mat4 _view;
	glm::mat4 _projection;

	float _fov;
	glm::vec2 _resolution;

	Varlet::Framebuffer* _framebuffer;
	Varlet::Shader* _renderShader;

	FramebufferConfiguration _framebufferConfiguration;

public:

	void OnConstructed() override;

	void OnDestroyed() override;	

	void Update() override;

	const Varlet::Framebuffer* GetFramebuffer() const;

	const glm::mat4& GetView() const;

	const glm::mat4& GetProjection() const;

	const glm::mat4 GetViewProjection() const;

	const Varlet::Texture* GetTargetTexture() const;
	
	void SetFramebufferConfiguration(const FramebufferConfiguration& configuration);

	void ResizeView(const int32_t& width, const int32_t& height);

	void GetResolution(int32_t& width, int32_t& height) const;

	void SetRenderShader(Varlet::Shader* shader);

	void Bind() const;

	void UnBind() const;

	Varlet::Shader* GetRenderShader();

private:

	void CulculateView(const glm::vec3& position, const glm::quat& rotation);

};

