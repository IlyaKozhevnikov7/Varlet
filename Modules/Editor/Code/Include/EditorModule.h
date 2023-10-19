#pragma once

#include "EditorCore.h"
#include "Module.h"

class EditorModule final : public Varlet::Core::Module, public Varlet::Core::IUpdatebleModule
{
public:

	~EditorModule() override = default;

private:

	bool Initialize() override;

	bool PostInitialize() override;

	void Update() override;

	void Shutdown() override;
};
