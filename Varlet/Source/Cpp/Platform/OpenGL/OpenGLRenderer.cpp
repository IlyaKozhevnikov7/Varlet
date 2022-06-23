#include "OpenGLRenderer.h"
<<<<<<< HEAD

#include "glad/glad.h"

=======
#include "OpenGLShader.h"
#include "VarletAPI.h"

#include "glad/glad.h"
>>>>>>> dev
#include "GLFW/glfw3.h"

namespace Varlet
{
	const OpenGLSettings& OpenGLRenderer::GetSettings()
	{
		return _settings;
	}

<<<<<<< HEAD
	int32_t OpenGLRenderer::GetAPIId() const
	{
		return 1;
	}

	int32_t OpenGLRenderer::Init()
	{
=======
	int32_t OpenGLRenderer::Init()
	{
		Renderer::Init();

>>>>>>> dev
		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false)
		{
			std::cout << "Failed to initialize OpenGL" << std::endl;
			return FAILED_INITIALIZATION;
		}

		glEnable(GL_DEPTH_TEST);
<<<<<<< HEAD
=======
		glFrontFace(GL_CCW);
>>>>>>> dev

		if (_settings.blending)
			glEnable(GL_BLEND);

		if (_settings.cullFace)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
<<<<<<< HEAD
			glFrontFace(GL_CCW);
=======
>>>>>>> dev
		}

		return SUCCESSFUL_INITIALIZATION;
	}

	void OpenGLRenderer::Update()
	{
	}
}
