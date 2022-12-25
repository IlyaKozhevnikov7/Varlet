#include "OpenGL/Graphics.h"
#include "OpenGL/DescriptorPool.h"
#include "OpenGL/Command.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/Utils.h"

#ifdef DEBUG
	#include "OpenGL/DebugGeometry.h"
#endif // DEBUG

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Scene/Camera.h"

static void DebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	VARLET_LOG(LevelType::Error, "[" + std::to_string(id) + "]" + message);
}

namespace Varlet::OpenGL
{
	int32_t Graphics::Init()
	{
		CreateWindow();
		const int32_t result = InitOpenGL();

#ifdef DEBUG
		DebugGeometry::Init();
#endif // DEBUG

		_globalData = UniformBuffer(sizeof(glm::mat4) * 4 + // view & projection & projection-view & model
			sizeof(glm::vec3));     // camera position

		_illuminationData = UniformBuffer((sizeof(int32_t) + sizeof(glm::vec4) * 2 + sizeof(float) * 2 + sizeof(int32_t)) * 10);

		Entity::NewComponentCreatedEvent.Bind(this, &OpenGL::Graphics::OnNewComponentCreated);

		return Varlet::Graphics::Init() == result == SUCCESSFUL_INITIALIZATION;
	}

	void Graphics::Update()
	{
		PROFILE_OUT(GraphicsInfo::renderTime);
	
		PlatformGL::MakeCurrent(context);

		UpdateIllumination();

		for (auto[cameraComponent, camera] : OpenGL::DescriptorPool::GetCameras())
		{
			if (cameraComponent->IsActive() == false)
				continue;
		
			_globalData.Bind();
			_globalData.SetData(0,						sizeof(glm::mat4), glm::value_ptr(cameraComponent->GetView()));
			_globalData.SetData(sizeof(glm::mat4),		sizeof(glm::mat4), glm::value_ptr(cameraComponent->GetProjection()));
			_globalData.SetData(sizeof(glm::mat4) * 2,	sizeof(glm::mat4), glm::value_ptr(cameraComponent->GetViewProjection()));
			_globalData.SetData(sizeof(glm::mat4) * 4,	sizeof(glm::vec3), glm::value_ptr(cameraComponent->GetOwner()->GetComponent<Transform>()->position));
		
			const bool withPostProcessing = cameraComponent->postProcessing.enable && cameraComponent->postProcessing.material != nullptr;
			uint32_t postProcessingTexture;
		
			glViewport(0, 0, camera->framebuffer.width, camera->framebuffer.height);

			camera->framebuffer.Bind();
		
			if (withPostProcessing)
			{
				postProcessingTexture = OpenGLUtils::CreateStackTexture(camera->framebuffer.width, camera->framebuffer.height);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);
			}
		
			glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
			const auto& shader = cameraComponent->GetRenderShader();
		
			for (const auto& data : _rendererData)
				Render(data, shader);
		
			if (withPostProcessing)
			{
				glDisable(GL_DEPTH_TEST);
			
				const static auto screenVAO = OpenGLUtils::CreateScreenVAO();
			
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, camera->framebuffer.attachments[0], 0);
			
				SetupProgramStages(cameraComponent->postProcessing.material);
			
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
			
				glBindVertexArray(screenVAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
				glEnable(GL_DEPTH_TEST);
				glDeleteTextures(1, &postProcessingTexture);
			}
		
#ifdef DEBUG
			Command::DrawDebugGeomtry();
#endif // DEBUG
			
			camera->framebuffer.UnBind();
		}

		PlatformGL::SwapBuffers(context);
	}

	void Graphics::Shutdown()
	{
		glDeleteProgramPipelines(1, &pipeline);

#ifdef DEBUG
		DebugGeometry::Shutdown();
#endif // DEBUG

		DescriptorPool::Shutdown();
	}

	int32_t Graphics::InitOpenGL()
	{
		if (gladLoadGL() == 0)
		{
			std::cout << "Failed to initialize OpenGL" << std::endl;
			return FAILED_INITIALIZATION;
		}

		GraphicsInfo::rendererName = glGetString(GL_RENDERER);

#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugMessage, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif // DEBUG

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glFrontFace(GL_CCW);

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		glGenProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);

		return SUCCESSFUL_INITIALIZATION;
	}

	void Graphics::CreateWindow()
	{
		Platform::CreateWindow();
		context = static_cast<Platform::WindowDescriptor>(Screen::GetNative());
	}

	void Graphics::UpdateIllumination() const
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
		
			_illuminationData.SetData(offset,											sizeof(glm::vec4), glm::value_ptr(light->color));
			_illuminationData.SetData(offset + sizeof(glm::vec4),						sizeof(glm::vec4), glm::value_ptr(_lightSources.pointLights[i].second->position));
			_illuminationData.SetData(offset + sizeof(glm::vec4) * 2,					sizeof(float), &light->linear);
			_illuminationData.SetData(offset + sizeof(glm::vec4) * 2 + sizeof(float),	sizeof(float), &light->quadratic);
		}
	}

	void Graphics::SetupMaterial(const Material* material) const
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

	void Graphics::Render(const RendererData& rendererData, Varlet::Shader* customShader) const
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
			const auto& shader = OpenGL::DescriptorPool::GetShader(customShader);
			shader->SetUInt32("u_EntityId", rendererData.renderer->GetOwner()->GetId());
			
			const uint32_t customShaderBits = shader->GetShaderBits();
			const uint32_t materialShaderBits = (GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT | GL_GEOMETRY_SHADER_BIT) ^ customShaderBits;
			
			SetupProgramStages(customShaderBits, shader);
			
			for (const auto& material : rendererData.renderer->materials)
			{
				const auto& materialShader = OpenGL::DescriptorPool::GetShader(material->GetShader());
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

				const auto& shader = OpenGL::DescriptorPool::GetShader(material->GetShader());

				SetupProgramStages(shader->GetShaderBits(), shader);
				SetupMaterial(material);

				Draw(mesh);
				PostDraw();
			}
		}
	}

	void Graphics::Draw(const Mesh* mesh) const
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

	void Graphics::PostDraw() const
	{
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glEnable(GL_DEPTH_TEST);
	}

	// TODO make one parameter
	void Graphics::SetupProgramStages(const uint32_t& stages, OpenGL::Shader* shader) const
	{
		for (int32_t offest = 0; offest < 3; offest++)
		{
			// 3 = its supported bits amount
			// now rendering support only vertex, fragment and geometry shaders

			const uint32_t current = 0x00000001 << offest;
			
			if ((current & stages) == current)
				glUseProgramStages(pipeline, current, shader->GetProgram(current));
		}
	}

	void Graphics::SetupProgramStages(const Material* material) const
	{
		SetupMaterial(material);

		const auto& shader = OpenGL::DescriptorPool::GetShader(material->GetShader());
		SetupProgramStages(shader->GetShaderBits(), shader);
	}

	void Graphics::OnNewComponentCreated(Entity* entity, Component* ñomponent)
	{
		if (const auto renderer = dynamic_cast<Renderer*>(ñomponent))
		{
			assert(entity->HasComponent<Transform>());
			_rendererData.push_back({ renderer, entity->GetComponent<Transform>() });
			VARLET_LOG(LevelType::Normal, "Entity added new renderer");
			return;
		}

		if (const auto camera = dynamic_cast<::Camera*>(ñomponent))
		{
			OpenGL::DescriptorPool::Register(camera);
			return;
		}

		if (dynamic_cast<LightSource*>(ñomponent))
		{
			if (const auto pointLight = dynamic_cast<PointLight*>(ñomponent))
			{
				assert(entity->HasComponent<Transform>());
				_lightSources.pointLights.push_back({ pointLight, entity->GetComponent<Transform>() });
				VARLET_LOG(LevelType::Normal, "Entity added new point light");
				return;
			}
		}
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
