project "Varlet"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}")
	objdir("%{wks.location}/Build/Obj/%{prj.name}/%{cfg.buildcfg}")

	files {
		"%{sourceIncludeFiles}",
		"%{sourceCppFiles}"
	}

	includedirs {
		"%{includeDir}",
		"%{includeDir}/Core",
		"%{includeDir}/Platform"
	}

	links {
		"glfw3"
	}

	libdirs {
		"%{prj.location}/Dependencies/libs"
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