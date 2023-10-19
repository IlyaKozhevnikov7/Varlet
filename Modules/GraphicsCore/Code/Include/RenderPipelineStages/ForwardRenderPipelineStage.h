#pragma once

#include "RenderPipelineStages/RenderPipelineStage.h"
#include "Types/Color.h"
#include "ForwardRenderPipelineStage.generated.h"

class Entity;
class Component;
class Renderer;
class PointLight;

namespace Varlet::Graphics
{
	struct RenderPipelineCreateInfo;

	struct PointLightData final
	{
		Color color;		// 12
		float intensity;	// 16
		glm::vec3 position;	// 28
		float attenuation;	// 32
		bool isActive;		// 33
		PAD(15);			// 48
	};

	struct IlluminationData final
	{
		constexpr static uint32_t MAX_POINT_LIGHT_COUNT = 64;

		std::array<PointLightData, MAX_POINT_LIGHT_COUNT> pointLights;
	};

	struct PointLightSet final
	{
		PointLight* pointLight = nullptr;
		Transform* transform = nullptr;
	};
}

REFLECTABLE()
class GRAPHICSCORE_API ForwardRenderPipelineStage final : public RenderPipelineStage
{
	GENERATION

private:

	std::vector<Renderer*> _renderers;
	std::vector<Varlet::Graphics::PointLightSet> _pointLightSets;

public:

	ForwardRenderPipelineStage();

	virtual ~ForwardRenderPipelineStage() = default;

	void Update() override;

	std::string FormatShader(const std::string& source) override;

	Varlet::Graphics::RenderPipelineCreateInfo GenerateCreateInfo(uint32_t width, uint32_t height, RenderPipelineStage* basePipeline) override;

private:

	void UpdateIllumination();

	void NewComponentCreated(Entity* entity, Component* component);

	void OnRendererCreated(Renderer* renderer);

	void OnRendererDestroyed(Renderer* renderer);

	void OnLightSourceCreated(LightSource* lightSource);

	void OnLightSourceDestroyed(LightSource* lightSource);
};

