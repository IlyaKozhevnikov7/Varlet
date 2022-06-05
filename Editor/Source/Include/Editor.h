#pragma once

#include "Varlet.h"
#include "EntryPoint.h"

class Editor final : public Varlet::Module, public Varlet::IUpdatebleModule
{
public:

	Editor() = default;
	~Editor() override = default;

private:

	void Init() override;

	void Shutdown() override;

	void Update() override;
};

MAKE_TARGET_SYSTEM(Editor);
