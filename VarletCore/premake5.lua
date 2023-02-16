project "VarletCore"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}")
	objdir("%{wks.location}/Build/Obj/%{prj.name}/%{cfg.buildcfg}")

	files {
		"Code/**"
	}

	includedirs {
		"Code/Include",
		"Dependencies/Windows"
	}

	defines {
		"CORE_DLL_EXPORT"
	}

	filter "system:Windows"
		defines "VARLET_WINDOWS"

		links {
			"glfw3dll"
		}

		libdirs {
			"Dependencies/Windows/glfw"
		}

	filter "configurations:Editor"
		defines {
			"VARLET_EDITOR",
			"VARLET_DEBUG",
			"VARLET_META"
		}
		symbols "On"

	filter "configurations:Debug"
		defines {
			"VARLET_DEBUG",
		}
		symbols "On"

	filter "configurations:Release"
		defines {
			"VARLET_RELEASE",
		}
		symbols "On"