#pragma once

#include "VarletCore.h"
#include "Rendering/RenderingCore.h"

namespace Varlet
{
	class OpenGLCameraCore final : public CameraCore
	{
	protected:

		void CulculateView(const glm::vec3& postion, const glm::vec3& rotation) override;
	};
}
