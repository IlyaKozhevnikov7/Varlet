#include "OpenGL/Graphics.h"
#include "OpenGL/DescriptorPool.h"
#include "OpenGL/RenderStages/MainRenderStage.h"

#ifdef DEBUG
	#include "OpenGL/RenderStages/DebugRenderStage.h"
#endif // DEBUG

#include "Entity.h"
#include "Scene/Camera.h"
#include "Core/Screen.h"

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

		_renderStages.push_back(new MainRenderStage());
#ifdef DEBUG
		_renderStages.push_back(new DebugRenderStage());
#endif // DEBUG

		for (auto& stage : _renderStages)
			stage->Init();

		return Varlet::Graphics::Init() == result == SUCCESSFUL_INITIALIZATION;
	}

	void Graphics::Update()
	{
		PROFILE_OUT(GraphicsInfo::renderTime);
	
		PlatformGL::MakeCurrent(context);

		for (auto& stage : _renderStages)
			stage->Update();

		PlatformGL::SwapBuffers(context);
	}

	void Graphics::Shutdown()
	{
		for (auto& stage : _renderStages)
		{
			stage->Shutdown();
			delete stage;
		}

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

		Entity::NewComponentCreatedEvent.Bind(this, &Graphics::OnNewComponentCreated);

		return SUCCESSFUL_INITIALIZATION;
	}

	void Graphics::CreateWindow()
	{
		Platform::CreateContext();
		context = static_cast<Platform::WindowDescriptor>(Screen::GetNative());
	}

	void Graphics::OnNewComponentCreated(Entity* entity, Component* ñomponent)
	{
		if (const auto camera = dynamic_cast<::Camera*>(ñomponent))
		{
			OpenGL::DescriptorPool::Register(camera);
			return;
		}
	}
}
