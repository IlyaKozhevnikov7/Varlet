#include "RenderPipelineStages/RenderPipelineStage.h"
#include "InternalCall.h"

TypeOf<RenderPipelineStage> RenderPipelineStage::DefaultRenderPipeline;
Event<RenderPipelineStage*> RenderPipelineStage::OnNewRenderPipelineStageCreated;
Event<RenderPipelineStage*> RenderPipelineStage::OnNewRenderPipelineStageRemoved;

RenderPipelineStage::~RenderPipelineStage()
{
	OnNewRenderPipelineStageRemoved.Invoke(this);
}

RenderPipelineStage* RenderPipelineStage::Create(TypeOf<RenderPipelineStage> type)
{
	assert(type);

	const auto newPipelineStage = CastMemory<RenderPipelineStage>(type, (int8_t*)(*type).CreateInstance());
	OnNewRenderPipelineStageCreated.Invoke(newPipelineStage);

	return newPipelineStage;
}

std::string RenderPipelineStage::FormatShader(const std::string& source)
{
	return source;
}

void RenderPipelineStage::UpdateInternal(bool isBasePipeline)
{
	if (isBasePipeline)
		Varlet::Graphics::InternalCall::BeginRenderPipeline(this);

	Update();

	if (next != nullptr)
	{
		Varlet::Graphics::InternalCall::NextPass();
		next->UpdateInternal(false);
	}

	if (isBasePipeline)
		Varlet::Graphics::InternalCall::EndRenderPipeline(this);
}