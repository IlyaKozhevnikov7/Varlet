#include "OpenGLCameraCore.h"
#include "GLM/glm.hpp"
#include "GLM/ext/matrix_transform.hpp"

namespace Varlet
{
	void OpenGLCameraCore::CulculateView(const glm::vec3& postion, const glm::vec3& rotation)
	{
		_view = glm::lookAtRH(postion, postion - rotation, glm::vec3(0.f, 1.f, 0.f));
	}
}
