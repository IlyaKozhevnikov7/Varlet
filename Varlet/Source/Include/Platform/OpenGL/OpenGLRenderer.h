#pragma once

<<<<<<< HEAD
#include "Renderer.h"
=======
#include "RenderingCore.h"
#include "OpenGLRendererAPI.h"
>>>>>>> dev

namespace Varlet
{
	// test settings, now only for opengl
	struct OpenGLSettings
	{
	public:

		bool blending = true;
		bool cullFace = true;
	};

<<<<<<< HEAD
	class OpenGLRenderer final : public Renderer
=======
	class OpenGLRenderer final : public Renderer, public IRendererAPIInitializer<OpenGLRendererAPI>
>>>>>>> dev
	{
	private:

		OpenGLSettings _settings;

	public:

		~OpenGLRenderer() override = default;

		const OpenGLSettings& GetSettings();

<<<<<<< HEAD
		virtual int32_t GetAPIId() const override;

=======
>>>>>>> dev
		int32_t Init() override;

		void Update() override;
	};
}
