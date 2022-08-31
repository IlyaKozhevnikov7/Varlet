#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "OpenGLVertexArray.h"
#include "VarletAPI.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

static int32_t ConvertToGlFunc(const StensilFunction& func)
{
	switch (func)
	{
	case StensilFunction::Never:
		return GL_NEVER;

	case StensilFunction::Less:
		return GL_LESS;

	case StensilFunction::LessOrEqual:
		return GL_LEQUAL;

	case StensilFunction::Greater:
		return GL_GREATER;

	case StensilFunction::GreaterOrEqual:
		return GL_GEQUAL;

	case StensilFunction::Equal:
		return GL_EQUAL;

	case StensilFunction::NotEqual:
		return GL_NOTEQUAL;

	case StensilFunction::Always:
		return GL_ALWAYS;
	}
}

static int32_t ConvertToGlOp(const StensilOp& op)
{
	switch (op)
	{
	case StensilOp::Keep:
		return GL_KEEP;

	case StensilOp::Zero:
		return GL_ZERO;

	case StensilOp::Replace:
		return GL_REPLACE;

	case StensilOp::Incr:
		return GL_INCR;

	case StensilOp::IncrWrap:
		return GL_INCR_WRAP;

	case StensilOp::Decr:
		return GL_DECR;

	case StensilOp::DecrWrap:
		return GL_DECR_WRAP;

	case StensilOp::Invert:
		return GL_INVERT;
	}
}

static void DebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	VARLET_LOG(LevelType::Error, "[" + std::to_string(id) + "]" + message);
}

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

		glEnable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glFrontFace(GL_CCW);

		if (_settings.blending)
			glEnable(GL_BLEND);

		if (_settings.cullFace)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugMessage, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif // DEBUG

		return Renderer::Init();
	}

	void OpenGLRenderer::Update()
	{
		UpdateIllumination();

		for (const auto& camera : _cameras)
		{
			if (camera->IsActive() == false)
				continue;

			camera->Bind();

			int32_t width, height;
			camera->GetResolution(width, height);
			glViewport(0, 0, width, height);

			_globalData->Bind();
			_globalData->SetData(0, sizeof(glm::mat4), glm::value_ptr(camera->GetView()));
			_globalData->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetProjection()));
			_globalData->SetData(sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(camera->GetViewProjection()));
			_globalData->SetData(sizeof(glm::mat4) * 4, sizeof(glm::vec3), glm::value_ptr(camera->GetOwner()->GetComponent<Transform>()->position));

			glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			const auto& shader = camera->GetRenderShader();

			for (const auto& data : _rendererData)
				Render(data, shader);

			camera->UnBind();
		}
	}

	void OpenGLRenderer::UpdateIllumination() const
	{
		constexpr int32_t sizeOfPointLight = sizeof(int32_t) * 2 + sizeof(glm::vec4) * 2 + sizeof(float) * 2;

		_illuminationData->Bind();

		for (int32_t i = 0; i < _lightSources.pointLights.size(); i++)
		{
			const int32_t offset = sizeOfPointLight * i;
			const auto& light = _lightSources.pointLights[i].first;
		
			const int32_t isActive = light->IsActive();
			_illuminationData->SetData(offset + sizeof(glm::vec4) * 2 + sizeof(float) * 2, sizeof(int32_t), &isActive);
		
			if (isActive == false)
				continue;
		
			_illuminationData->SetData(offset,											sizeof(glm::vec4), glm::value_ptr(light->color));
			_illuminationData->SetData(offset + sizeof(glm::vec4),						sizeof(glm::vec4), glm::value_ptr(_lightSources.pointLights[i].second->position));
			_illuminationData->SetData(offset + sizeof(glm::vec4) * 2,					sizeof(float), &light->linear);
			_illuminationData->SetData(offset + sizeof(glm::vec4) * 2 + sizeof(float),	sizeof(float), &light->quadratic);
		}
	}

	void OpenGLRenderer::SetupMaterial(const Material* material) const
	{
		material->Activate();

		if (material->settings.stencilTest.enable)
		{
			const auto stencilSettings = material->settings.stencilTest;

			glStencilMask(0xFF);

			glStencilOp(
				ConvertToGlOp(stencilSettings.failOp),
				ConvertToGlOp(stencilSettings.zFailOp),
				ConvertToGlOp(stencilSettings.allPass));

			glStencilFunc(
				ConvertToGlFunc(stencilSettings.function),
				stencilSettings.ref,
				stencilSettings.mask);
		}

		if (material->settings.depthTest == false)
			glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRenderer::Render(const RendererData& rendererData, const Shader* customShader) const
	{
		if (rendererData.meshRenderer->isVisible == false)
			return;

		const Mesh* mesh = rendererData.meshRenderer->GetMesh();
		if (mesh == nullptr)
			return;

		glm::mat4 model = glm::translate(glm::mat4(1.f), rendererData.transform->position);
		model = model * glm::mat4_cast(rendererData.transform->rotation);
		model = glm::scale(model, rendererData.transform->scale);

		_globalData->SetData(sizeof(glm::mat4) * 3, sizeof(glm::mat4), glm::value_ptr(model));
		_globalData->SetData(sizeof(glm::mat4) * 4 + sizeof(glm::vec3), sizeof(int32_t), &rendererData.meshRenderer->GetRenderId());

		if (customShader)
		{
			customShader->Use();
			Draw(mesh);
		}
		else
		{
			for (const auto& material : rendererData.meshRenderer->GetMaterials())
			{
				if (material->isActive == false)
					continue;

				SetupMaterial(material);

				Draw(mesh);
				PostDraw();
			}
		}
	}

	void OpenGLRenderer::Draw(const Mesh* mesh) const
	{
		for (const auto& subMesh : mesh->GetSubMeshes())
		{
			glBindVertexArray(subMesh->GetVAO());

			if (subMesh->IsIndexed())
				glDrawElements(GL_TRIANGLES, subMesh->GetElementsCount(), GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, subMesh->GetElementsCount());
		}
	}

	void OpenGLRenderer::PostDraw() const
	{
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glEnable(GL_DEPTH_TEST);
	}
}