#pragma once

#include "Rendering/Graphics.h"
#include "OpenGL/RendererAPI.h"
#include "OpenGL/Camera.h"
#include "OpenGL/UniformBuffer.h"

class Entity;
class Component;
class MeshRenderer;
class TextureRenderer;
class Renderer;
class Transform;
class Camera;
class PointLight;
class Mesh;
class Material;

namespace Varlet::OpenGL
{
	struct RendererData
	{
		Renderer* renderer;
		Transform* transform;
	};

	class Shader;
	struct UniformBuffer;

	class Graphics final : public Varlet::Graphics, public IRendererAPIInitializer<RendererAPI>
	{
	private:

		std::vector<RendererData> _rendererData;
		LightSourceData _lightSources;

		UniformBuffer _globalData;
		UniformBuffer _illuminationData;

		uint32_t _mainPipeline;

	public:

		~Graphics() override = default;

		int32_t Init() override;

		void Update() override;

		void Shutdown() override;

	private:

		void UpdateIllumination() const;

		void SetupMaterial(const Material* material) const;

		void Render(const RendererData& rendererData, Varlet::Shader* customShader = nullptr) const;

		void Draw(const Mesh* mesh) const;

		void PostDraw() const;

		void SetupProgramStages(const uint32_t& stages, OpenGL::Shader* shader) const;

		void SetupProgramStages(const Material* material) const;

		void OnNewComponentCreated(Entity* entity, Component* ñomponent);
	};

	class OpenGLUtils
	{
	public:

		static uint32_t CreateStackTexture(const int32_t& width, const int32_t& height);

		static uint32_t CreateScreenVAO();
	};
}
