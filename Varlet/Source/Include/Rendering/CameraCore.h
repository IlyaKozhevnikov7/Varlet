#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class CORE_API CameraCore
	{
	protected:

		glm::mat4 _view;
		glm::mat4 _projection;

		float _fov;

		FrameBuffer* _framebuffer;

	public:

		virtual ~CameraCore() = default;

		const glm::mat4& GetView() const { return _view; }

		const glm::mat4& GetProjection() const { return _projection; }

		glm::mat4 GetViewProjection() const { return _view * _projection; }

		virtual void CulculateView(const glm::vec3& postion, const glm::vec3& rotation) = 0;
	};
}
