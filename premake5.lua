include "Dependencies.lua"

workspace "Varlet"
	architecture "x64"

	configurations {
		"Debug",
		"Release"
	}

	--startproject "Editor"

	entryPointPath = "%{wks.location}/Varlet/Source/Include/EntryPoint"

	includeDir = "Source/Include"

	sourceIncludeFiles = "Source/Include/**.h"
	sourceCppFiles = "Source/Cpp/**.cpp"

	include "Editor"
	include "Varlet"
	include "Sandbox"