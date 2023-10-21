#include "OpenGLModule.h"
#include "InternalCall.h"
#include "Internal/InterfaceFunctions.h"
#include "glad/glad.h"

static void DebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	LogVerbosity verbosity = Normal;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_MEDIUM:
	case GL_DEBUG_SEVERITY_LOW:
		verbosity = Warning;
		break;

	case GL_DEBUG_SEVERITY_HIGH:
		verbosity = Error;
		break;
	}

	auto validMessage = Varlet::Core::Platform::ConvertToWide(message);
	VT_LOG(verbosity, "OpenGL: [%i] %s", id, validMessage.c_str());
}

bool OpenGLModule::Initialize()
{
	if (gladLoadGL() == 0)
	{
		VT_LOG(Error, "Failed to initialize OpenGL");
		return false;
	}

#ifdef VARLET_DEBUG
	glEnable(GL_DEBUG_OUTPUT);

#ifdef VARLET_DEVELOPMENT
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif // VARLET_DEVELOPMENT

	glDebugMessageCallback(DebugMessage, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif // VARLET_DEBUG

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glDisable(GL_BLEND);

	Varlet::Graphics::InternalCall::CreateShader = Varlet::Graphics::OpenGL::CreateShader;
	Varlet::Graphics::InternalCall::CreateTexture = Varlet::Graphics::OpenGL::CreateTexture;
	Varlet::Graphics::InternalCall::CreateModel = Varlet::Graphics::OpenGL::CreateModel; 
	Varlet::Graphics::InternalCall::CreateUniformBuffer = Varlet::Graphics::OpenGL::CreateUniformBuffer;
	Varlet::Graphics::InternalCall::CreateRenderPipeline = Varlet::Graphics::OpenGL::CreateRenderPipeline;

	Varlet::Graphics::InternalCall::BeginRenderPipeline = Varlet::Graphics::OpenGL::BeginRenderPipeline;
	Varlet::Graphics::InternalCall::EndRenderPipeline = Varlet::Graphics::OpenGL::EndRenderPipeline;
	Varlet::Graphics::InternalCall::NextPass = Varlet::Graphics::OpenGL::NextPass;
	Varlet::Graphics::InternalCall::BindShader = Varlet::Graphics::OpenGL::BindShader;
	Varlet::Graphics::InternalCall::Draw = Varlet::Graphics::OpenGL::Draw;
	Varlet::Graphics::InternalCall::DrawVertices = Varlet::Graphics::OpenGL::DrawVertices;

	Varlet::Graphics::InternalCall::GetNativeTexture = Varlet::Graphics::OpenGL::GetNativeTexture;
	Varlet::Graphics::InternalCall::GetShaderUniformInfos = Varlet::Graphics::OpenGL::GetShaderUniformInfos;
	Varlet::Graphics::InternalCall::UpdateUniformBufferData = Varlet::Graphics::OpenGL::UpdateUniformBufferData;

	return true;
}
