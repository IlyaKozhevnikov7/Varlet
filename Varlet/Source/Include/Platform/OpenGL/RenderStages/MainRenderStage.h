#pragma once

#include "OpenGL/RenderStages/IRenderStage.h"
#include "OpenGL/UniformBuffer.h"
#include <vector>

class Renderer;
class Transform;
class PointLight;
class Material;
class Entity;
class Component;
class Mesh;

namespace Varlet
{
	class Shader;
}

namespace Varlet::OpenGL
{
	class Shader;

	class MainRenderStage final : public IRenderStage
	{
		struct RendererData final
		{
			Renderer* renderer;
			Transform* transform;
		};

		struct LightSourceData final
		{
			std::vector<std::pair<PointLight*, Transform*>> pointLights;
			// Directional lights
			// Spotlights
		};

	private:

		std::vector<RendererData> _rendererData;
		LightSourceData _lightSources;

		uint32_t _pipeline;
		UniformBuffer _globalData;
		UniformBuffer _illuminationData;

	public:

		~MainRenderStage() override = default;

		void Init() override;

		void Shutdown() override;

		void Update() override;

	private:

		void UpdateIllumination() const;

		void Render(const RendererData& rendererData, Varlet::Shader* customShader) const;

		void Draw(const Mesh* mesh) const;

		void PostDraw() const;

		void SetupMaterial(const Material* material) const;

		void SetupProgramStages(const uint32_t& stages, Shader* shader) const;

		void OnNewComponentCreated(Entity* entity, Component* ñomponent);
	};
}
