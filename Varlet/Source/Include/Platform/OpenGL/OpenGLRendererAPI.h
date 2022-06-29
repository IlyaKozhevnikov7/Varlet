#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class OpenGLRendererAPI final : public IRendererAPI
	{
	public:

		Shader* CreateShader(const ShaderInitializer* initializer) const override;

		CameraCore* CreateCameraCore() const override;

		UniformBuffer* CreateUniformBuffer(const int64_t& size) const override;
	};
}
