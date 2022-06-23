#pragma once

#include "VarletModules.h"

#ifdef DLL_EXPORT
	#define EDITOR_API __declspec(dllexport)
#else
	#define EDITOR_API __declspec(dllimport)
#endif

struct GLFWwindow;

class EDITOR_API Editor final : public Varlet::Module, public Varlet::IUpdatebleModule
{
private:

	GLFWwindow* _window;
	Varlet::GameModule* _context;

public:

	Editor(Varlet::GameModule* module);
	~Editor() override = default;

private:

	int32_t Init() override;

	void Shutdown() override;

	void Update() override;

	void DrawDockSpace() const;

	void DrawViewPort() const;
};
