#include "OpenGLCameraCore.h"
#include "GLM/glm.hpp"
#include "GLM/ext/matrix_transform.hpp"

namespace Varlet
{
	void OpenGLCameraCore::CulculateView(const glm::vec3& postion, const glm::vec3& rotation)
	{
		glm::quat pitch = glm::angleAxis(rotation.y, glm::vec3(1.f, 0.f, 0.f));
		glm::quat yaw = glm::angleAxis(rotation.x, glm::vec3(0.f, 1.f, 0.f));
		glm::quat roll = glm::angleAxis(rotation.z, glm::vec3( 0.f, 0.f, 1.f));

		glm::quat orientation = glm::normalize(pitch * yaw * roll);
		glm::mat4 orientationMat = glm::mat4_cast(orientation);
		
		glm::mat4 translate = glm::mat4(1.0f);
		translate = glm::translate(translate, -postion);

		_view = orientationMat * translate;
	}
}
