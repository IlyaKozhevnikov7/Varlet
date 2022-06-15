#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "VarletAPI.h"

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
		Renderer::Init();

		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false)
		{
			std::cout << "Failed to initialize OpenGL" << std::endl;
			return FAILED_INITIALIZATION;
		}

		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);

		if (_settings.blending)
			glEnable(GL_BLEND);

		if (_settings.cullFace)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		return SUCCESSFUL_INITIALIZATION;
	}

	void OpenGLRenderer::Update()
	{
	}
}
