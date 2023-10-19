project "GraphicsCore"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("Bin/%{cfg.buildcfg}")
	objdir("Obj/%{prj.name}/%{cfg.buildcfg}")

	defines {
		"GRAPHICSCORE_DLL_EXPORT"
	}

	files {
		"../Code/**",
		"Generation/**"
	}

	includedirs {
		"../Code/Include",
		"Generation",

		"../../../VarletCore/Code/Include",
		"../../../VarletCore/Cache/Generation",

		"../../Varlet/Code/Include",
		"../../Varlet/Cache/Generation",

		"../../GraphicsInterface/Code/Include",
		"../../GraphicsInterface/Cache/Generation",

		"../Dependencies/stb/include",
		"../Dependencies/assimp/include"
	}

	libdirs {
		"../Dependencies/libs/assimp"
	}

	links {
		"VarletCore",
		"Varlet",
		"GraphicsInterface",
		"assimp-vc143-mt"
	}

	prebuildcommands "W:/VarletUtils/CodeGenerator/bin/Debug/net6.0/CodeGenerator.exe W:/Varlet/Modules/GraphicsCore/"

	filter "system:Windows"
		defines "VARLET_WINDOWS"

	filter "configurations:Development"
		defines {
			"VARLET_DEVELOPMENT",
			"VARLET_DEBUG"
		}

		files {
			"../../Editor/Dependencies/**"
		}

		includedirs {
			"../../EditorCore/Code/Include",
			"../../EditorCore/Cache/Generation",

			"../../Editor/Code/Include",
			"../../Editor/Cache/Generation",
			"../../Editor/Dependencies"
		}

		libdirs {
			"../../Editor/Dependencies"
		}

		links {
			"EditorCore",
			"Editor",
			"glfw3dll"
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

		excludes {
			"../../EditorCore/**",
			"../../Editor/**"
		}

		symbols "Off"
