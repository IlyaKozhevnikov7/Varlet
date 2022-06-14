project "Varlet"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}")
	objdir("%{wks.location}/Build/Obj/%{prj.name}/%{cfg.buildcfg}")

	files {
		"%{sourceIncludeFiles}",
		"%{sourceCppFiles}",

		"%{includeDirs.GLAD}/../src/glad.c"
	}

	includedirs {
		"%{includeDir}",
		"%{includeDir}/Core",
		"%{includeDir}/Platform/Windows",
		"%{includeDir}/Platform/OpenGL",
		"%{includeDir}/Rendering",

		"%{includeDirs.GLFW}",
		"%{includeDirs.GLAD}"
	}

	links {
		"glfw3dll"
	}

	libdirs {
		"%{libraryDirs.GLFW}"
	}

	defines {
		"DLL_EXPORT"
	}

	filter "configurations:Debug"
		defines {
			"DEBUG",
		}
		symbols "On"

	filter "configurations:Release"
		defines {
			"RELEASE"
		}
		optimize "On"