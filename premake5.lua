include "Dependencies.lua"

workspace "Varlet"
	
	architecture "x64"

	configurations {
		"Editor",
		"Debug",
		"Release"
	}

	startproject "Sandbox"

	entryPointPath = "%{wks.location}/Varlet/Source/Include/EntryPoint"

	includeDir = "Source/Include"

	sourceIncludeFiles = "Source/Include/**.h"
	sourceCppFiles = "Source/Cpp/**.cpp"

	include "Editor"
	include "Varlet"
	include "Sandbox"
