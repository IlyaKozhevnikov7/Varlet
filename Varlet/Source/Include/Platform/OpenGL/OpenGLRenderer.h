#pragma once

#include "RenderingCore.h"
#include "OpenGLRendererAPI.h"

namespace Varlet
{
	// test settings, now only for opengl
	struct OpenGLSettings
	{
	public:

		bool blending = true;
		bool cullFace = true;
	};

	class OpenGLRenderer final : public Renderer, public IRendererAPIInitializer<OpenGLRendererAPI>
	{
	private:

		OpenGLSettings _settings;

	public:

		~OpenGLRenderer() override = default;

		const OpenGLSettings& GetSettings();

		int32_t Init() override;

		void Update() override;

		void Render(const RendererData& rendererData, Shader* cameraShader);
	};
}
