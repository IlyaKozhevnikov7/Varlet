#pragma once

#include "Renderer.h"

namespace Varlet
{
	// test settings, now only for opengl
	struct OpenGLSettings
	{
	public:

		bool blending = true;
		bool cullFace = true;
	};

	class OpenGLRenderer final : public Renderer
	{
	private:

		OpenGLSettings _settings;

	public:

		~OpenGLRenderer() override = default;

		const OpenGLSettings& GetSettings();

		virtual int32_t GetAPIId() const override;

		int32_t Init() override;

		void Update() override;
	};
}
