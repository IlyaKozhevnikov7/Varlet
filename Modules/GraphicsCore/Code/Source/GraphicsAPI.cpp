#include "GraphicsAPI.h"
#include "GraphicsBaseTypes.h"
#include "GraphicsCoreModule.h"
#include "InternalCall.h"
#include "RenderPipelineSystem.h"

namespace Varlet::Graphics
{
	GraphicsCoreModule* API::_module = nullptr;

	void API::Initialize(GraphicsCoreModule* module)
	{
		assert(_module == nullptr);
		assert(module);

		_module = module;
	}

	bool API::CreateShader(void* descriptor, const std::string& source, bool compileAsSource)
	{
		assert(descriptor);

		bool success = false;

		Graphics::ShaderCreateInfo createInfo;
		
		if (compileAsSource)
		{
			createInfo.source = &source;
			createInfo.pipelineType = nullptr;

			return Varlet::Graphics::InternalCall::CreateShader(descriptor, createInfo);
		};

		auto& pipelineStages = Varlet::RenderPipelineSystemInternal::GetAllRenderPipelineStages();

		for (auto pipelineStageSet : pipelineStages)
		{
			const auto pipelineStage = pipelineStageSet.second[0];

			const auto formattedSource = pipelineStage->FormatShader(source);

			createInfo.source = &formattedSource;
			createInfo.pipelineType = (void*)pipelineStage->GetType();

			const bool result = Varlet::Graphics::InternalCall::CreateShader(descriptor, createInfo);

			if (success == false)
				success = result;
		}

		return success;
	}
}
