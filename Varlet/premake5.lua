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
		"%{includeDir}/Gameplay",
		"%{includeDir}/Input",
		"%{includeDir}/Rendering",
		"%{includeDir}/Scene",
		"%{includeDir}/Platform",
		"%{includeDir}/Platform/Windows",
		"%{includeDir}/Time",
		"%{includeDir}/Utils",

		"%{includeDirs.GLFW}",
		"%{includeDirs.GLAD}",
		"%{includeDirs.GLM}",
		"%{includeDirs.STB}",
		"%{includeDirs.ASSIMP}",
		"%{includeDirs.VULKAN}"
	}

	links {
		"glfw3dll",
		"assimp-vc143-mt",
		--"vulkan-1",
		"shaderc_shared"
	}

	libdirs {
		"%{libraryDirs.GLFW}",
		"%{libraryDirs.ASSIMP}",
		"%{libraryDirs.VULKAN}"
	}

	defines {
		"VARLET_DLL_EXPORT"
	}

	filter "configurations:Editor"
		defines {
			"EDITOR",
			"DEBUG",
			"META"
		}
		symbols "On"

	filter "configurations:Debug"
		defines {
			"DEBUG",
		}
		symbols "On"

	filter "configurations:Release"
		defines {
			"RELEASE",
		}
		symbols "On"