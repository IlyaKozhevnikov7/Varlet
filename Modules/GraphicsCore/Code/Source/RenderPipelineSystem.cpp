#include "RenderPipelineSystem.h"
#include "../../GraphicsInterface/Code/Include/GraphicsAPI.h"
#include "RenderPipelineStages/RenderPipelineStage.h"

namespace Varlet
{
	std::unordered_map<const Varlet::Core::Type*, std::vector<RenderPipelineStage*>> RenderPipelineSystemInternal::_pipelines;

	bool RenderPipelineSystemInternal::CreateRenderContext(const Varlet::RenderContextInfo& info, RenderPipelineStage* pipeline)
	{
		assert(pipeline);

		RenderPipelineStage* currentProcessPipeline = pipeline;
		Varlet::Graphics::RenderPipelineCreateInfo finalCreateInfo;

		while (currentProcessPipeline != nullptr)
		{
			currentProcessPipeline->cameraInfo = info.cameraInfo;

			Varlet::Graphics::RenderPipelineCreateInfo createInfo = currentProcessPipeline->GenerateCreateInfo(info.width, info.height, pipeline);
			finalCreateInfo.Combine(createInfo);
			currentProcessPipeline = currentProcessPipeline->next;
		}

		// chech is exist
		_pipelines[pipeline->GetType()].push_back(pipeline);

		return Varlet::Graphics::CreateRenderPipeline(pipeline, finalCreateInfo);
	}

	std::unordered_map<const Varlet::Core::Type*, std::vector<RenderPipelineStage*>>& RenderPipelineSystemInternal::GetAllRenderPipelineStages()
	{
		return _pipelines;
	}

	void RenderPipelineSystemInternal::Update()
	{
		for (auto& set : _pipelines)
			for (auto& pipeline : set.second)
				pipeline->UpdateInternal(true);
	}
}

bool RenderPipelineSystem::CreateRenderContext(const Varlet::RenderContextInfo& info, RenderPipelineStage* pipeline)
{
	return Varlet::RenderPipelineSystemInternal::CreateRenderContext(info, pipeline);
}
