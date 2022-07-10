#pragma once

#include "VarletModules.h"

#ifdef EDITOR_DLL_EXPORT
	#define EDITOR_API __declspec(dllexport)
#else
	#define EDITOR_API __declspec(dllimport)
#endif

namespace Varlet
{
	class Entity;
}

struct GLFWwindow;
class EditorCamera;

class Editor final : public Varlet::Module, public Varlet::IUpdatebleModule
{
private:

	GLFWwindow* _window;
	Varlet::GameModule* _context;

	//Varlet::Entity* _mainCamera;
	EditorCamera* _mainCamera;

public:

	EDITOR_API Editor(Varlet::GameModule* module);
	~Editor() override = default;

private:

	int32_t Init() override;

	int32_t PostInit() override;

	void Shutdown() override;

	void Update() override;

	void DrawCameraInfo();

	void DrawDockSpace() const;

	void DrawViewPort() const;
};
