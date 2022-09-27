#pragma once

#include "RenderingCore.h"
#include "OpenGLRendererAPI.h"

class Mesh;
class Material;

namespace Varlet
{
	class OpenGLShader;

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
		uint32_t _mainPipeline;

	public:

		~OpenGLGraphics() override = default;

		const OpenGLSettings& GetSettings();

		int32_t Init() override;

		void Update() override;

		void Shutdown() override;

	private:

		void UpdateIllumination() const;

		void SetupMaterial(const Material* material) const;

		void Render(const RendererData& rendererData, Shader* customShader = nullptr) const;

		void Draw(const std::vector<VertexArray*>& vertices) const;

		void PostDraw() const;

		void SetupProgramStages(const uint32_t& stages, OpenGLShader* shader) const;

		void SetupProgramStages(const Material* material) const;
	};

	class OpenGLUtils
	{
	public:

		static uint32_t CreateStackTexture(const int32_t& width, const int32_t& height);

		static uint32_t CreateScreenVAO();
	};
}
