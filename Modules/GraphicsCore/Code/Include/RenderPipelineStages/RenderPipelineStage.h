#pragma once

#include "GraphicsCoreCore.h"
#include "RenderPipelineStage.generated.h"

namespace Varlet
{
	struct CameraInfo final
	{
		glm::mat4* view = nullptr;
		glm::mat4* projection = nullptr;
		glm::vec3* position = nullptr;
	};

	struct RenderContextInfo final
	{
		uint32_t width = 0;
		uint32_t height = 0;
		CameraInfo cameraInfo;
	};

	namespace Graphics
	{
		struct RenderPipelineCreateInfo;
	}
}

REFLECTABLE(Abstract)
class GRAPHICSCORE_API RenderPipelineStage
{
	GENERATION

public:

	static TypeOf<RenderPipelineStage> DefaultRenderPipeline;
	static Event<RenderPipelineStage*> OnNewRenderPipelineStageCreated;
	static Event<RenderPipelineStage*> OnNewRenderPipelineStageRemoved;

	Varlet::CameraInfo cameraInfo;
	RenderPipelineStage* next = nullptr;
	std::vector<int8_t> mappedAttachments;

public:

	RenderPipelineStage() = default;

	virtual ~RenderPipelineStage();

	static RenderPipelineStage* Create(TypeOf<RenderPipelineStage> type);

	virtual std::string FormatShader(const std::string& source);

	virtual void Update() = 0;

	virtual Varlet::Graphics::RenderPipelineCreateInfo GenerateCreateInfo(uint32_t width, uint32_t height, RenderPipelineStage* basePipeline) = 0;

	void UpdateInternal(bool isBasePipeline);
};