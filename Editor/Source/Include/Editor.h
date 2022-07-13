#pragma once

#include "VarletModules.h"

#ifdef EDITOR_DLL_EXPORT
	#define EDITOR_API __declspec(dllexport)
#else
	#define EDITOR_API __declspec(dllimport)
#endif

struct EditorData
{
	static Varlet::GameModule* context;
};

namespace Varlet
{
	class Entity;
}

struct GLFWwindow;
class EditorCamera;
class Panel;

class Editor final : public Varlet::Module, public Varlet::IUpdatebleModule
{
private:

	std::vector<Panel*> _panels;
	GLFWwindow* _window;

public:

	EDITOR_API Editor(Varlet::GameModule* module);
	~Editor() override = default;

private:

	int32_t Init() override;

	void InitImGui();

	int32_t PostInit() override;

	void Shutdown() override;

	void Update() override;

	void DrawCameraInfo();
};
