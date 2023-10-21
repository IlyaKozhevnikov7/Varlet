project "OpenGL"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("Bin/%{cfg.buildcfg}")
	objdir("Obj/%{prj.name}/%{cfg.buildcfg}")

	files {
		"../Code/**",
		"Generation/**",
		"../Dependencies/glad/**"
	}

	includedirs {
		"../Code/Include",
		"Generation",

		"../Dependencies/glad/include",

		"../../../VarletCore/Code/Include",
		"../../../VarletCore/Cache/Generation",

		"../../GraphicsInterface/Code/Include",
		"../../GraphicsInterface/Cache/Generation",
	}

	libdirs {
		"../../Varlet/Cache/Bin/%{cfg.buildcfg}"
	}

	links {
		"VarletCore",
		"GraphicsInterface"
	}

	defines {
		"OPENGL_DLL_EXPORT"
	}

	prebuildcommands "W:/VarletUtils/CodeGenerator/bin/Debug/net6.0/CodeGenerator.exe W:/Varlet/Modules/OpenGL/"

	filter "system:Windows"
		defines "VARLET_WINDOWS"

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
