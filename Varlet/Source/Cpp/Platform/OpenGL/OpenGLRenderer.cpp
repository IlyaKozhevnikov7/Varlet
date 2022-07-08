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
			_processedCameraData->SetData(0,						sizeof(glm::mat4), glm::value_ptr(camera->GetView()));
			_processedCameraData->SetData(sizeof(glm::mat4),		sizeof(glm::mat4), glm::value_ptr(camera->GetProjection()));
			_processedCameraData->SetData(sizeof(glm::mat4) * 2,	sizeof(glm::mat4), glm::value_ptr(camera->GetViewProjection()));

			for (const auto data : _rendererData)
				Render(data);

			camera->GetCore()->UnBind();
		}
	}

	void OpenGLRenderer::Render(const RendererData& rendererData)
	{
		// todo when added meterial to mesh component
		static const auto defautlShader = RendererAPI::CreateShader({
			"../Varlet/Shaders/defaultVertex.glsl",
			"../Varlet/Shaders/defaultFragment.glsl",
			""});

		glm::mat4 model = glm::mat4(1.f);
		glm::translate(model, rendererData.transform->GetPosition());

		const glm::vec3 rotation = rendererData.transform->GetRotation();
		glm::rotate(model, glm::degrees(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		glm::rotate(model, glm::degrees(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		glm::rotate(model, glm::degrees(rotation.z), glm::vec3(0.f, 0.f, 1.f));

		glm::scale(model, rendererData.transform->GetScale());

		defautlShader->Use();

		defautlShader->SetMat4("u_Model", model);

		if (auto mesh = rendererData.meshRenderer->GetMesh())
		{
			for (auto subMesh : mesh->GetSubMeshes())
			{
				auto vertexArray = subMesh->GetVertexArray();

				glBindVertexArray(vertexArray->GetVAO());

				if (vertexArray->IsIndexed())
					glDrawElements(GL_TRIANGLES, vertexArray->GetElementsCount(), GL_UNSIGNED_INT, 0);
				else
					glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetElementsCount());
			}

			glBindVertexArray(0);
		}
	}
}
