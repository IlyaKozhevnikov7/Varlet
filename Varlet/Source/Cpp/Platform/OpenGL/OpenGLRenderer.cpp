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

		{
			float triangle[] =
			{
				-0.5f, -0.5f,
				 0.5f, -0.5f,
				  0.f,  0.5f,
			};

			glGenVertexArrays(1, &_trianbgleVAO);
			glGenBuffers(1, &_trianbgleVBO);

			glBindVertexArray(_trianbgleVAO);

			glBindBuffer(GL_ARRAY_BUFFER, _trianbgleVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), &triangle[0], GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			_triangle = new	OpenGLShader();
		}

		return SUCCESSFUL_INITIALIZATION;
	}

	void OpenGLRenderer::Update()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_triangle->Use();
		glBindVertexArray(_trianbgleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
