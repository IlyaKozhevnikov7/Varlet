project "Editor"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("Bin/%{cfg.buildcfg}")
	objdir("Obj/%{prj.name}/%{cfg.buildcfg}")

	files {
		"../Code/**",
		"Generation/**",

		"../Dependencies/**",

		"../../OpenGL/Dependencies/glad/src/**"
	}

	includedirs {
		"../Code/Include",
		"../Code/Include/Panels",
		"../Code/Include/Panels/AssetPanels",
		"../Code/Include/Components",
		"../Code/Include/Platform",

		"Generation",

		"../Dependencies",

		-- todo instead wks.location use define engine folder
		"../../../VarletCore/Code/Include",
		"../../../VarletCore/Cache/Generation",

		"../../EditorCore/Code/Include",
		"../../EditorCore/Cache/Generation",

		"../../Varlet/Code/Include",
		"../../Varlet/Cache/Generation",

		"../../GraphicsInterface/Code/Include",
		"../../GraphicsInterface/Cache/Generation",

		"../../OpenGL/Dependencies/glad/include"
	}

	libdirs {
		"../Dependencies"
	}

	links {
		"VarletCore",
		"Varlet",
		"GraphicsInterface",
		"EditorCore",
		"opengl32"
	}

	defines {
		"EDITOR_DLL_EXPORT"
	}

	prebuildcommands "W:/VarletUtils/CodeGenerator/bin/Debug/net6.0/CodeGenerator.exe W:/Varlet/Modules/Editor/"

	filter "system:windows"
		defines "VARLET_WINDOWS"

		includedirs {
			"../Dependencies/Windows"
		}

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
