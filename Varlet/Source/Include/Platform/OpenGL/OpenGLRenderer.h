#pragma once

#include "RenderingCore.h"
#include "OpenGLRendererAPI.h"

class Mesh;
class Material;

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

	private:

		void UpdateIllumination() const;

		void SetupMaterial(const Material* material) const;

		void Render(const RendererData& rendererData, const Shader* customShader = nullptr) const;

		void Draw(const Mesh* mesh) const;

		void PostDraw() const;
	};
}
