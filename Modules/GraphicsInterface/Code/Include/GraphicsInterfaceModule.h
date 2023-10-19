#pragma once

#include "GraphicsInterfaceCore.h"
#include "Module.h"

class GRAPHICSINTERFACE_API GraphicsInterfaceModule final : public Varlet::Core::Module
{
public:

	~GraphicsInterfaceModule() override = default;

	bool Initialize() override;
};
