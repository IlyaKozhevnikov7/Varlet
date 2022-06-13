#include "OpenGLRenderer.h"

#include "glad/glad.h"

#include "GLFW/glfw3.h"

namespace Varlet
{
	const OpenGLSettings& OpenGLRenderer::GetSettings()
	{
		return _settings;
	}

	int32_t OpenGLRenderer::Init()
	{
		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false)
		{
			std::cout << "Failed to initialize OpenGL" << std::endl;
			return FAILED_INITIALIZATION;
		}

		glEnable(GL_DEPTH_TEST);

		if (_settings.blending)
			glEnable(GL_BLEND);

		if (_settings.cullFace)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
		}

		return SUCCESSFUL_INITIALIZATION;
	}

	void OpenGLRenderer::Update()
	{
	}
}
