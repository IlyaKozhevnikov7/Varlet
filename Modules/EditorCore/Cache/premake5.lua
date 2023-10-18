project "EditorCore"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("Bin/%{cfg.buildcfg}")
	objdir("Obj/%{prj.name}/%{cfg.buildcfg}")

	defines {
		"EDITOR_CORE_DLL_EXPORT"
	}

	files {
		"../Code/**",
		"Generation/**",

		"../Dependencies/**",
	}

	includedirs {
		"../Code/Include",

		"Generation",

		"../Dependencies",

		"../../../VarletCore/Code/Include",
		"../../../VarletCore/Cache/Generation"
	}

	libdirs {
		"../Dependencies"
	}

	links {
		"VarletCore"
	}

	prebuildcommands "W:/VarletUtils/CodeGenerator/bin/Debug/net6.0/CodeGenerator.exe W:/Varlet/Modules/EditorCore/"

	filter "system:windows"
		defines "VARLET_WINDOWS"
		
		links {
			"glfw3dll"
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
