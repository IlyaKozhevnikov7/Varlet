project "Varlet"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("Bin/%{cfg.buildcfg}")
	objdir("Obj/%{prj.name}/%{cfg.buildcfg}")

	defines {
		"VARLET_DLL_EXPORT"
	}

	files {
		"../Code/**",
		"Generation/**"
	}

	includedirs {
		"../Code/Include",
		"../Code/Include/Components",
		"../Code/Include/Graphics",
		"../Code/Include/Internal",
		"../Code/Include/Platform",

		"Generation",

		"../Dependencies/glm/include",
		"../Dependencies/stb/include",
		"../Dependencies/assimp/include",

		"%{wks.location}/VarletCore/Code/Include",
		"%{wks.location}/VarletCore/Cache/Generation"
	}

	libdirs {
		"../Dependencies/libs/assimp"
	}

	links {
		"VarletCore"
	}

	prebuildcommands "W:/VarletUtils/CodeGenerator/bin/Debug/net6.0/CodeGenerator.exe W:/Varlet/Modules/Varlet/"

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
			"VARLET_DEBUG"
		}
		symbols "On"

	filter "configurations:Release"
		defines {
			"VARLET_RELEASE",
		}
		symbols "On"