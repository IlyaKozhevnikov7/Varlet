#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "VarletAPI.h"
#include "Transform.h"
#include "Mesh.h"
#include "OpenGLVertexArray.h"

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

	void OpenGLRenderer::Update()
	{
		for (const auto camera : _cameras)
		{
			if (camera->IsActive() == false)
				continue;

			camera->GetCore()->Bind();

			glClearColor(0.f, 0.5f, 0.5f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			_processedCameraData->Bind();
			_processedCameraData->SetData(0, sizeof(glm::mat4), glm::value_ptr(camera->GetView()));
			_processedCameraData->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetProjection()));
			_processedCameraData->SetData(sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(camera->GetViewProjection()));

			for (const auto data : _rendererData)
				Render(data);

			camera->GetCore()->UnBind();
		}
	}

	void OpenGLRenderer::Render(const RendererData& rendererData)
	{
		if (auto mesh = rendererData.meshRenderer->GetMesh())
		{
			glm::mat4 model = glm::translate(glm::mat4(1.f), rendererData.transform->GetPosition());
			model = model * glm::mat4_cast(rendererData.transform->GetRotation());
			model = glm::scale(model, rendererData.transform->GetScale());

			for (auto shader : mesh->GetShaders())
			{
				shader->Use();
				shader->SetMat4("u_Model", model);

				for (auto subMesh : mesh->GetSubMeshes())
				{
					glBindVertexArray(subMesh->GetVAO());

					if (subMesh->IsIndexed())
						glDrawElements(GL_TRIANGLES, subMesh->GetElementsCount(), GL_UNSIGNED_INT, 0);
					else
						glDrawArrays(GL_TRIANGLES, 0, subMesh->GetElementsCount());
				}

				glBindVertexArray(0);
			}
		}
	}
}
