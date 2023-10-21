#include "GraphicsCoreModule.h"
#include "GraphicsAPI.h"
#include "RenderPipelineSystem.h"
#include "RenderPipelineStages/RenderPipelineStage.h"
#include "RenderPipelineStages/ForwardRenderPipelineStage.h"

bool GraphicsCoreModule::Initialize()
{
	RenderPipelineStage::DefaultRenderPipeline = ForwardRenderPipelineStage::GetTypeStatic();

	Varlet::Graphics::API::Initialize(this);

	//RenderPipelineStage::OnNewRenderPipelineStageCreated.Bind(this, &GraphicsCoreModule::OnNewRenderPipelineStageCreated);
	//RenderPipelineStage::OnNewRenderPipelineStageRemoved.Bind(this, &GraphicsCoreModule::OnNewRenderPipelineStageRemoved);

	return true;
}

void GraphicsCoreModule::Update()
{
	Varlet::RenderPipelineSystemInternal::Update();
}

void GraphicsCoreModule::OnNewRenderPipelineStageCreated(RenderPipelineStage* pipelineStage)
{
}

void GraphicsCoreModule::OnNewRenderPipelineStageRemoved(RenderPipelineStage* pipelineStage)
{
}
