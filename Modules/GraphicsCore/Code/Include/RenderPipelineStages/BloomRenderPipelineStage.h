#pragma once

#include "RenderPipelineStages/RenderPipelineStage.h"
#include "BloomRenderPipelineStage.generated.h"

REFLECTABLE()
class GRAPHICSCORE_API BloomRenderPipelineStage final : public RenderPipelineStage
{
	GENERATION

	struct DownSamplingData final
	{
		glm::vec2 sourceResolution;
		int32_t mipLevel;
		float radius;
		void* sourceTexture;
	};

	struct UpSamplingData final
	{
		float filterRadius;
		void* sourceTexture;
	};

	struct FinalData
	{
		float exposure;
		float bloomStrength;
		void* t1;
		void* t2;
	};

	FIELD(EditorAccessible)
	float _filterRadius = 0.005f;

	FIELD(EditorAccessible)
	float _anotherFilterRadius = 0.005f;

	FIELD(EditorAccessible)
	float exposure = 1.f;

	FIELD(EditorAccessible)
	float bloomStrength = 0.04f;

private:

	void* _mainTexture = nullptr;
	uint32_t _mipCount = 0;
	glm::vec2 _resolution;

	Varlet::Descriptor _initialColor;
	Varlet::Descriptor _emissionColor;

	Shader* _downSamplingShader;
	Shader* _upSamplingShader;
	Shader* _finalShader;
	Shader* _emissionDetectShader;

public:

	BloomRenderPipelineStage();

	~BloomRenderPipelineStage() override = default;

private:

	void Update() override;

	std::string FormatShader(const std::string& source);

	Varlet::Graphics::RenderPipelineCreateInfo GenerateCreateInfo(uint32_t width, uint32_t height, RenderPipelineStage* basePipeline) override;
};