#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class CORE_API Camera
	{
	protected:

		glm::mat4 _view;
		glm::mat4 _projection;

		float _fov;

		FrameBuffer* _framebuffer;

	public:

		virtual ~Camera() = default;

		const glm::mat4& GetView() const { return _view; }

		const glm::mat4& GetProjection() const { return _projection; }

		const glm::mat4& GetViewProjection() const { return _view * _projection; }

	protected:

		virtual void CulculateView() = 0;
	};
}
