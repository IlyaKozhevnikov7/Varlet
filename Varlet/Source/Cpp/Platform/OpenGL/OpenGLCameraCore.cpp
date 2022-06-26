#include "OpenGLCameraCore.h"
#include "GLM/glm.hpp"
#include "GLM/ext/matrix_transform.hpp"

namespace Varlet
{
	void OpenGLCameraCore::CulculateView()
	{
		_view = glm::lookAtRH(_position, _position - _forward, glm::vec3(0.f, 1.f, 0.f));
	}
}
