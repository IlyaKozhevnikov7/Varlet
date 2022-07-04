#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "VarletAPI.h"
#include "Transform.h"

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
		glFrontFace(GL_CCW);

		if (_settings.blending)
			glEnable(GL_BLEND);

		if (_settings.cullFace)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		return Renderer::Init();
	}

	void OpenGLRenderer::Render(const RendererData& rendererData)
	{
		// todo when added meterial to mesh component
		static const auto defautlShader = RendererAPI::CreateShader({
			"W:/Varlet/Varlet/Shaders/defaultVertex.glsl",
			"W:/Varlet/Varlet/Shaders/defaultFragment.glsl",
			""});

		glm::mat4 model = glm::mat4(1.f);
		glm::translate(model, rendererData.transform->GetPosition());

		const glm::vec3 rotation = rendererData.transform->GetRotation();
		glm::rotate(model, rotation.x, glm::vec3(1.f, 0.f, 0.f));
		glm::rotate(model, rotation.y, glm::vec3(0.f, 1.f, 0.f));
		glm::rotate(model, rotation.z, glm::vec3(0.f, 0.f, 1.f));

		glm::scale(model, rendererData.transform->GetScale());

		//defautlShader->Use();
	}
}
