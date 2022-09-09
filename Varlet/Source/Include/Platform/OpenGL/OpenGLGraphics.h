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
		bool cullFace = false;
	};

	class OpenGLGraphics final : public Graphics, public IRendererAPIInitializer<OpenGLRendererAPI>
	{
	private:

		OpenGLSettings _settings;

	public:

		~OpenGLGraphics() override = default;

		const OpenGLSettings& GetSettings();

		int32_t Init() override;

		void Update() override;

	private:

		void UpdateIllumination() const;

		void SetupMaterial(const Material* material) const;

		void Render(const RendererData& rendererData, const Shader* customShader = nullptr) const;

		void Draw(const std::vector<VertexArray*>& vertices) const;

		void PostDraw() const;
	};

	class OpenGLUtils
	{
	public:

		static uint32_t CreateStackTexture(const int32_t& width, const int32_t& height);

		static uint32_t CreateScreenVAO();
	};
}
