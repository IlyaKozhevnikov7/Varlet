#pragma once

#include "VarletModules.h"
#include "Platform/OpenGL/OpenGLCameraCore.h"

#ifdef DLL_EXPORT
	#define EDITOR_API __declspec(dllexport)
#else
	#define EDITOR_API __declspec(dllimport)
#endif

struct GLFWwindow;

template<typename T>
class EditorCamera;

class Editor final : public Varlet::Module, public Varlet::IUpdatebleModule
{
private:

	GLFWwindow* _window;
	Varlet::GameModule* _context;

	EditorCamera<Varlet::OpenGLCameraCore>* _editorCamera;
	
public:

	EDITOR_API Editor(Varlet::GameModule* module);
	~Editor() override = default;

private:

	int32_t Init() override;

	int32_t PostInit() override;

	void Shutdown() override;

	void Update() override;

	void DrawDockSpace() const;

	void DrawViewPort() const;
};
