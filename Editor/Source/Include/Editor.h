#pragma once

#include "VarletModules.h"
#include "EntryPoint.h"

struct GLFWwindow;
struct ImGuiIO;

class Editor final : public Varlet::Module, public Varlet::IUpdatebleModule
{
private:

	GLFWwindow* _context;

public:

	Editor();
	~Editor() override = default;

private:

	int32_t Init() override;

	void Shutdown() override;

	void Update() override;

	void DrawDockSpace() const;

	void DrawViewPort() const;
};

MAKE_TARGET_SYSTEM(Editor);
