project "Editor"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}")
	objdir("%{wks.location}/Build/Obj/%{prj.name}/%{cfg.buildcfg}")

	defines {
		"EDITOR_DLL_EXPORT"
	}

	files {
		"%{sourceIncludeFiles}",
		"%{sourceCppFiles}",
		"%{includeDirs.ImGui}/**",

		"%{includeDirs.GLAD}/../src/glad.c"
	}

	includedirs {
		"%{wks.location}/Varlet/Source/Include",
		"%{includeDir}",
		"%{includeDir}/Panels",

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

	-- configurations
	
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

	-- platforms

	filter "platforms:Windows"
		defines {
			"VRT_WINDOWS"
		}