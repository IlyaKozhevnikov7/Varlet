#pragma once

#include "OpenGLCore.h"
#include "Module.h"
#include "Platform/Current.h"

class OPENGL_API OpenGLModule final : public Varlet::Core::Module
{
private:

	Varlet::Core::Platform::WindowDescriptor _context;

public:

	~OpenGLModule() override = default;

	bool Initialize() override;
};
