#pragma once

#include "VarletModules.h"

<<<<<<< HEAD
struct GLFWwindow;
struct ImGuiIO;

class Editor final : public Varlet::Module, public Varlet::IUpdatebleModule
{
private:

	GLFWwindow* _context;

public:

	Editor();
=======
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
>>>>>>> dev
	~Editor() override = default;

private:

	int32_t Init() override;

	void Shutdown() override;

	void Update() override;

	void DrawDockSpace() const;

	void DrawViewPort() const;
};
