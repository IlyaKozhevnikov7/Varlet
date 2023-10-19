#pragma once

#include "GraphicsCoreCore.h"

class RenderPipelineStage;

namespace Varlet
{
	struct RenderContextInfo;

	class RenderPipelineSystemInternal final
	{
	private:

		static std::unordered_map<const Varlet::Core::Type*, std::vector<RenderPipelineStage*>> _pipelines;

	public:

		static bool CreateRenderContext(const Varlet::RenderContextInfo& info, RenderPipelineStage* pipeline);

		static std::unordered_map<const Varlet::Core::Type*, std::vector<RenderPipelineStage*>>& GetAllRenderPipelineStages();

		static void Update();
	};
}

class GRAPHICSCORE_API RenderPipelineSystem final
{
public:

	static bool CreateRenderContext(const Varlet::RenderContextInfo& info, RenderPipelineStage* pipeline);
};
