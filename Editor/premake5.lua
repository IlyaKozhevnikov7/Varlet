project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}")
	objdir("%{wks.location}/Build/Obj/%{prj.name}/%{cfg.buildcfg}")

	files {
		"%{sourceIncludeFiles}",
		"%{sourceCppFiles}",
		"%{includeDirs.ImGui}/**",

		"%{includeDirs.GLAD}/../src/glad.c"
	}

	includedirs {
		"%{entryPointPath}",
		"%{wks.location}/Varlet/Source/Include",
		"%{includeDir}",

		"%{prj.location}/Dependencies",
		"%{includeDirs.GLAD}",
		"%{includeDirs.GLFW}"
	}

	links {
		"Varlet",
		"glfw3dll",
		"opengl32"
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