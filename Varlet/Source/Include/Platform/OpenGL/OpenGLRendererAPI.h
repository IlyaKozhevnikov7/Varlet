#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class OpenGLRendererAPI final : public IRendererAPI
	{
	public:

		CameraCore* CreateCameraCore() const override;

		virtual UniformBuffer* CreateUniformBuffer(const int64_t& size) const override;
	};
}
