#include "OpenGL/RenderStages/MainRenderStage.h"
#include "OpenGL/DescriptorPool.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/Utils.h"

#include "Entity.h"
#include "Component.h"
#include "Renderer.h"
#include "Transform.h"
#include "Camera.h"
#include "PointLight.h"
#include "Material.h"

namespace Varlet::OpenGL
{
	void MainRenderStage::Init()
	{
		glGenProgramPipelines(1, &_pipeline);

		_globalData = UniformBuffer(sizeof(glm::mat4) * 4 + // view & projection & projection-view & model
			sizeof(glm::vec3));								// camera position

		_illuminationData = UniformBuffer((sizeof(int32_t) + sizeof(glm::vec4) * 2 + sizeof(float) * 2 + sizeof(int32_t)) * 100);

		Entity::NewComponentCreatedEvent.Bind(this, &MainRenderStage::OnNewComponentCreated);
	}

	void MainRenderStage::Shutdown()
	{
		glDeleteProgramPipelines(1, &_pipeline);
	}

	void MainRenderStage::Update()
	{
		UpdateIllumination();

		glBindProgramPipeline(_pipeline);

		for (auto [cameraComponent, camera] : DescriptorPool::GetCameras())
		{
			if (cameraComponent->IsActive() == false)
				continue;

			_globalData.Bind();
			_globalData.SetData(0, sizeof(glm::mat4), glm::value_ptr(cameraComponent->GetView()));
			_globalData.SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cameraComponent->GetProjection()));
			_globalData.SetData(sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(cameraComponent->GetViewProjection()));
			_globalData.SetData(sizeof(glm::mat4) * 4, sizeof(glm::vec3), glm::value_ptr(cameraComponent->GetOwner()->GetComponent<Transform>()->position));

			camera->framebuffer.Bind();

			glViewport(0, 0, camera->framebuffer.width, camera->framebuffer.height);
			glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			const auto& shader = cameraComponent->GetRenderShader();

			for (const auto& data : _rendererData)
				Render(data, shader);

			camera->framebuffer.UnBind();
		}
	}

	void MainRenderStage::UpdateIllumination() const
	{
		constexpr int32_t sizeOfPointLight = sizeof(int32_t) * 2 + sizeof(glm::vec4) * 2 + sizeof(float) * 2;

		_illuminationData.Bind();

		for (int32_t i = 0; i < _lightSources.pointLights.size(); i++)
		{
			const int32_t offset = sizeOfPointLight * i;
			const auto& light = _lightSources.pointLights[i].first;

			const int32_t isActive = light->IsActive();
			_illuminationData.SetData(offset + sizeof(glm::vec4) * 2 + sizeof(float) * 2, sizeof(int32_t), &isActive);

			if (isActive == false)
				continue;

			_illuminationData.SetData(offset, sizeof(glm::vec4), glm::value_ptr(light->color));
			_illuminationData.SetData(offset + sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(_lightSources.pointLights[i].second->position));
			_illuminationData.SetData(offset + sizeof(glm::vec4) * 2, sizeof(float), &light->linear);
			_illuminationData.SetData(offset + sizeof(glm::vec4) * 2 + sizeof(float), sizeof(float), &light->quadratic);
		}
	}

	void MainRenderStage::Render(const RendererData& rendererData, Varlet::Shader* customShader) const
	{
		if (rendererData.renderer->isVisible == false)
			return;

		const auto mesh = rendererData.renderer->GetMesh();
		if (mesh == nullptr)
			return;

		rendererData.renderer->OnPreRender();

		_globalData.Bind();
		_globalData.SetData(sizeof(glm::mat4) * 3, sizeof(glm::mat4), glm::value_ptr(rendererData.renderer->GetModelMatrix()));

		if (customShader != nullptr)
		{
			const auto& shader = DescriptorPool::GetShader(customShader);
			shader->SetUInt32("u_EntityId", rendererData.renderer->GetOwner()->GetId());

			const uint32_t customShaderBits = shader->GetShaderBits();
			const uint32_t materialShaderBits = (GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT | GL_GEOMETRY_SHADER_BIT) ^ customShaderBits;

			SetupProgramStages(customShaderBits, shader);

			for (const auto& material : rendererData.renderer->materials)
			{
				const auto& materialShader = DescriptorPool::GetShader(material->GetShader());
				SetupProgramStages(materialShaderBits, materialShader);
				SetupMaterial(material);

				Draw(mesh);
			}
		}
		else
		{
			for (const auto& material : rendererData.renderer->materials)
			{
				if (material->isActive == false)
					continue;

				const auto& shader = DescriptorPool::GetShader(material->GetShader());

				SetupProgramStages(shader->GetShaderBits(), shader);
				SetupMaterial(material);

				Draw(mesh);
				PostDraw();
			}
		}
	}

	void MainRenderStage::Draw(const Mesh* mesh) const
	{
		const auto& set = DescriptorPool::GetVertexBufferSet(mesh);

		for (const auto& buffer : set)
		{
			glBindVertexArray(buffer.vao);

			if (buffer.ebo > 0)
				glDrawElements(GL_TRIANGLES, buffer.verticesCount, GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, buffer.verticesCount);
		}
	}

	void MainRenderStage::PostDraw() const
	{
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glEnable(GL_DEPTH_TEST);
	}

	void MainRenderStage::SetupMaterial(const Material* material) const
	{
		material->Activate();

		if (material->settings.stencilTest.enable)
		{
			const auto& stencilSettings = material->settings.stencilTest;

			glStencilMask(0xFF);

			glStencilOp(
				Utils::ConvertToGLOp(stencilSettings.failOp),
				Utils::ConvertToGLOp(stencilSettings.zFailOp),
				Utils::ConvertToGLOp(stencilSettings.allPass));

			glStencilFunc(
				Utils::ConvertToGLFunc(stencilSettings.function),
				stencilSettings.ref,
				stencilSettings.mask);
		}

		if (material->settings.depthTest == false)
			glDisable(GL_DEPTH_TEST);
	}

	void MainRenderStage::SetupProgramStages(const uint32_t& stages, Shader* shader) const
	{
		for (int32_t offest = 0; offest < 3; offest++)
		{
			// 3 = its supported bits amount
			// now rendering support only vertex, fragment and geometry shaders

			const uint32_t current = 0x00000001 << offest;

			if ((current & stages) == current)
				glUseProgramStages(_pipeline, current, shader->GetProgram(current));
		}
	}

	void MainRenderStage::OnNewComponentCreated(Entity* entity, Component* ñomponent)
	{
		if (const auto renderer = dynamic_cast<Renderer*>(ñomponent))
		{
			assert(entity->HasComponent<Transform>());
			_rendererData.push_back({ renderer, entity->GetComponent<Transform>() });
			return;
		}

		if (dynamic_cast<LightSource*>(ñomponent))
		{
			if (const auto pointLight = dynamic_cast<PointLight*>(ñomponent))
			{
				assert(entity->HasComponent<Transform>());
				_lightSources.pointLights.push_back({ pointLight, entity->GetComponent<Transform>() });
				return;
			}
		}
	}
}
