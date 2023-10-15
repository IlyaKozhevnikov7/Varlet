project "VarletCore"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}")
	objdir("%{wks.location}/Build/Obj/%{prj.name}/%{cfg.buildcfg}")

	files {
		"Code/**",
		"Cache/Generation/**"
	}

	includedirs {
		"Code/Include",
		"Cache/Generation"
	}

	defines {
		"CORE_DLL_EXPORT"
	}

	prebuildcommands "W:/VarletUtils/CodeGenerator/bin/Debug/net6.0/CodeGenerator.exe W:/Varlet/VarletCore/ -core"

	filter "system:Windows"
		defines "VARLET_WINDOWS"

		includedirs {
			"Dependencies/Windows"
		}

		links {
			"glfw3dll"
		}

		libdirs {
			"Dependencies/Windows/glfw"
		}

	filter "configurations:Development"
		defines {
			"VARLET_DEVELOPMENT",
			"VARLET_DEBUG"
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