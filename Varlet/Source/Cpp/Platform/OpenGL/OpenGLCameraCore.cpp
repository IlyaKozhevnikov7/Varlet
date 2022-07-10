#include "OpenGLCameraCore.h"
#include "GLM/glm.hpp"
#include "GLM/ext/matrix_transform.hpp"

namespace Varlet
{
	void OpenGLCameraCore::CulculateView(const glm::vec3& position, const glm::quat& rotation)
	{
		const glm::mat4 translateMat = glm::translate(glm::mat4(1.f), -position);
		const glm::mat4 orientationMat = glm::mat4_cast(glm::inverse(rotation));
		
		_view = orientationMat * translateMat;
	}
}
