#pragma once

#include "VarletCore.h"
#include "Module.h"

class VARLET_API VarletModule final : public Varlet::Core::Module, public Varlet::Core::IUpdatebleModule
{
public:

	~VarletModule() override = default;

	bool Initialize() override;

	void Update() override;

	void Shutdown() override;
};
