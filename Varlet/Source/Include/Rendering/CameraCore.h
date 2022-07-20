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
		glm::vec2 _resolution;

		Framebuffer* _framebuffer;

	public:

		CameraCore();

		virtual ~CameraCore();

		void Bind() const;

		void UnBind() const;

		const glm::mat4& GetView() const;

		const glm::mat4& GetProjection() const;

		const glm::mat4 GetViewProjection() const;

		const Texture* GetRendereTexture() const;

		void ResizeView(const int32_t& width, const int32_t& height);

		void GetResolution(int32_t& width, int32_t& height) const;

		virtual void CulculateView(const glm::vec3& postion, const glm::quat& rotation) = 0;
	};	
}
