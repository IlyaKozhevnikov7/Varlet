#pragma once

#include "RenderingCore.h"
#include "OpenGLRendererAPI.h"
#include "OpenGL/Camera.h"

class Camera;
class Mesh;
class Material;

namespace Varlet
{
	namespace OpenGL
	{
		class Shader;
	}

	class OpenGLGraphics final : public Graphics, public IRendererAPIInitializer<OpenGLRendererAPI>
	{
	private:

		std::vector<RendererData> _rendererData;
		LightSourceData _lightSources;

		UniformBuffer* _globalData;
		UniformBuffer* _illuminationData;

		uint32_t _mainPipeline;

	public:

		~OpenGLGraphics() override;

		int32_t Init() override;

		void Update() override;

		void Shutdown() override;

	private:

		void UpdateIllumination() const;

		void SetupMaterial(const Material* material) const;

		void Render(const RendererData& rendererData, Shader* customShader = nullptr) const;

		void Draw(const std::vector<VertexArray*>& vertices) const;

		void PostDraw() const;

		void SetupProgramStages(const uint32_t& stages, OpenGL::Shader* shader) const;

		void SetupProgramStages(const Material* material) const;

		void InitWithEngine();

		void OnNewComponentCreated(Entity* entity, Component* ñomponent);
	};

	class OpenGLUtils
	{
	public:

		static uint32_t CreateStackTexture(const int32_t& width, const int32_t& height);

		static uint32_t CreateScreenVAO();
	};
}
