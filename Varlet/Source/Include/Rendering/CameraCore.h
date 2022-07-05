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

		Framebuffer* _framebuffer;

	public:

		CameraCore();

		virtual ~CameraCore();

		void Bind() const;

		void UnBind() const;

		const glm::mat4& GetView() const;

		const glm::mat4& GetProjection() const;

		const glm::mat4 GetViewProjection() const;

		const uint32_t& GetRendereTexture() const;

		virtual void CulculateView(const glm::vec3& postion, const glm::vec3& rotation) = 0;
	};	
}
