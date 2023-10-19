#pragma once

#include "GraphicsCoreCore.h"
#include "RenderPipelineStages/RenderPipelineStage.h"

class GraphicsCoreModule;

namespace Varlet::Graphics
{
	class GRAPHICSCORE_API API final
	{
	private:

		static GraphicsCoreModule* _module;

	public:

		static void Initialize(GraphicsCoreModule* module);

		static bool CreateShader(void* descriptor, const std::string& source, bool compileAsSource);
	};
}
