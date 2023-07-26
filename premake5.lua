workspace "Varlet"
	
	architecture "x64"

	configurations {
		"Editor",
		"Debug",
		"Release"
	}

	startproject "Launcher"

	-- Core
	include "VarletCore"
	include "Launcher"

	-- Modules
	include "Modules/Editor/Cache"
	include "Modules/Varlet/Cache"
	include "Modules/OpenGL/Cache"
	include "Modules/Input/Cache"

	include "Modules/EditorCore/Cache"

	-- Try new graphics module
	include "Modules/Graphics/Cache"
	