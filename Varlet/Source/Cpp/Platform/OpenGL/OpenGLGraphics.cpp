#include "OpenGLGraphics.h"
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
	const OpenGLSettings& OpenGLGraphics::GetSettings()
	{
		return _settings;
	}

	int32_t OpenGLGraphics::Init()
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
		
		GraphicsInfo::rendererName = glGetString(GL_RENDERER);

		glGenProgramPipelines(1, &_mainPipeline);
		glBindProgramPipeline(_mainPipeline);

#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugMessage, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif // DEBUG

		return Graphics::Init();
	}

	void OpenGLGraphics::Update()
	{
		PROFILE_OUT(GraphicsInfo::renderTime);

		UpdateIllumination();

		for (const auto& camera : _cameras)
		{
			if (camera->IsActive() == false)
				continue;

			_globalData->Bind();
			_globalData->SetData(0, sizeof(glm::mat4), glm::value_ptr(camera->GetView()));
			_globalData->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetProjection()));
			_globalData->SetData(sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(camera->GetViewProjection()));
			_globalData->SetData(sizeof(glm::mat4) * 4, sizeof(glm::vec3), glm::value_ptr(camera->GetOwner()->GetComponent<Transform>()->position));

			int32_t width, height;
			camera->GetResolution(width, height);
			glViewport(0, 0, width, height);

			const bool withPostProcessing = camera->postProcessing.enable && camera->postProcessing.material != nullptr;
			uint32_t postProcessingTexture;

			camera->Bind();

			if (withPostProcessing)
			{
				postProcessingTexture = OpenGLUtils::CreateStackTexture(width, height);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);
			}

			glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			const auto& shader = camera->GetRenderShader();

			for (const auto& data : _rendererData)
				Render(data, shader);

			if (withPostProcessing)
			{
				glDisable(GL_DEPTH_TEST);

				const static auto screenVAO = OpenGLUtils::CreateScreenVAO();
		
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, camera->GetTargetTexture()->GetId(), 0);

				SetupProgramStages(camera->postProcessing.material);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, postProcessingTexture);

				glBindVertexArray(screenVAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
				glEnable(GL_DEPTH_TEST);
				glDeleteTextures(1, &postProcessingTexture);
			}

			camera->UnBind();
		}
	}

	void OpenGLGraphics::Shutdown()
	{
		glDeleteProgramPipelines(1, &_mainPipeline);
	}

	void OpenGLGraphics::UpdateIllumination() const
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

	void OpenGLGraphics::SetupMaterial(const Material* material) const
	{
		material->Activate();

		if (material->settings.stencilTest.enable)
		{
			const auto& stencilSettings = material->settings.stencilTest;

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

	void OpenGLGraphics::Render(const RendererData& rendererData, Shader* customShader) const
	{
		if (rendererData.renderer->isVisible == false)
			return;
		
		const auto vertices = rendererData.renderer->GetVertices();
		if (vertices == nullptr || vertices->size() == 0)
			return;

		rendererData.renderer->OnPreRender();

		_globalData->Bind();
		_globalData->SetData(sizeof(glm::mat4) * 3, sizeof(glm::mat4), glm::value_ptr(rendererData.renderer->GetModelMatrix()));

		if (customShader != nullptr)
		{
			const auto& shader = OpenGLShaderCache::Get(customShader);
			shader->SetUInt32("u_EntityId", rendererData.renderer->GetOwner()->GetId());

			const uint32_t customShaderBits = shader->GetShaderBits();
			const uint32_t materialShaderBits = (GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT | GL_GEOMETRY_SHADER_BIT) ^ customShaderBits;

			SetupProgramStages(customShaderBits, shader);

			for (const auto& material : rendererData.renderer->materials)
			{
				const auto& materialShader = OpenGLShaderCache::Get(material->GetShader());
				SetupProgramStages(materialShaderBits, materialShader);

				Draw(*vertices);
			}
		}
		else
		{
			for (const auto& material : rendererData.renderer->materials)
			{
				if (material->isActive == false)
					continue;

				const auto& shader = OpenGLShaderCache::Get(material->GetShader());

				SetupProgramStages(shader->GetShaderBits(), shader);
				SetupMaterial(material);

				Draw(*vertices);
				PostDraw();
			}
		}
	}

	void OpenGLGraphics::Draw(const std::vector<VertexArray*>& vertices) const
	{
		for (const auto& subMesh : vertices)
		{
			glBindVertexArray(subMesh->GetVAO());

			if (subMesh->IsIndexed())
				glDrawElements(GL_TRIANGLES, subMesh->GetElementsCount(), GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, subMesh->GetElementsCount());
		}
	}

	void OpenGLGraphics::PostDraw() const
	{
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLGraphics::SetupProgramStages(const uint32_t& stages, OpenGLShader* shader) const
	{
		for (int32_t offest = 0; offest < 3; offest++)
		{
			// 3 = its supported bits amount
			// now rendering support only vertex, fragment and geometry shaders

			const uint32_t current = 0x00000001 << offest;

			if ((current & stages) == current)
				glUseProgramStages(_mainPipeline, current, shader->GetProgram(current));
		}
	}

	void OpenGLGraphics::SetupProgramStages(const Material* material) const
	{
		SetupMaterial(material);

		const auto& shader = static_cast<OpenGLShader*>(const_cast<Shader*>(material->GetShader()));
		SetupProgramStages(shader->GetShaderBits(), shader);
	}

	uint32_t OpenGLUtils::CreateStackTexture(const int32_t& width, const int32_t& height)
	{
		uint32_t id;
	
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		return id;
	}

	uint32_t OpenGLUtils::CreateScreenVAO()
	{
		// todo destroy when exit
		uint32_t vao;
		uint32_t vbo;
		uint32_t ebo;

		constexpr float positions[] =
		{
			-1.f,  1.f,
			-1.f, -1.f,
			 1.f, -1.f,
			 1.f,  1.f
		};

		constexpr float textureCoordinates[]
		{
			0.f, 1.f,
			0.f, 0.f,
			1.f, 0.f,
			1.f, 1.f
		};

		constexpr uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(textureCoordinates), nullptr, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(textureCoordinates), &textureCoordinates[0]);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, reinterpret_cast<void*>(0));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, reinterpret_cast<void*>(sizeof(positions)));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return vao;
	}
}
