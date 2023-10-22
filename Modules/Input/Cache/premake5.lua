project "Input"
    kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir("Bin/%{cfg.buildcfg}")
	objdir("Obj/%{prj.name}/%{cfg.buildcfg}")

	defines {
		"INPUT_DLL_EXPORT"
	}

	files {
		"../Code/**",
		"Generation/**"
	}
    
    includedirs {
        "../Code/Include",
        "Generation",       

        "../../../VarletCore/Code/Include",
		"../../../VarletCore/Cache/Generation"
    }

	links {
		"VarletCore"
	}

	prebuildcommands "W:/VarletUtils/CodeGenerator/bin/Debug/net6.0/CodeGenerator.exe W:/Varlet/Modules/Input/"

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
