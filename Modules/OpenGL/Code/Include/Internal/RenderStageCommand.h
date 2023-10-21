#pragma once

#include "OpenGLCore.h"

namespace Varlet::Graphics::OpenGL
{
	struct RenderStageCommand final
	{
	private:

	public:

		static bool BeginRenderPipeline(void* descriptor);

		static bool EndRenderPipeline(void* descriptor);

		static void NextPass();
		
		static void BindShader(void* shaderDescriptor, void* pipelineDescriptor, const void* uniformBuffer);

		static void Draw(void* descriptor);

		static void DrawVertices(uint32_t vertexCount);
	};
}
