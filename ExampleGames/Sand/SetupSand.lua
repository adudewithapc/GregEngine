project "Sand"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	
	files { "Source/**.h", "Source/**.cpp" }
	
	includedirs
	{
		"../../Source/",
		"../../Includes/glad",
		"../../Includes/stb_image"
	}
	
	links
	{
		"GregEngine"
	}
	
	targetdir ("Binaries/" .. outputdir)
	objdir ("Binaries/Intermediate" .. outputdir)
	
	filter "system:windows"
		systemversion "latest"
		buildoptions {"/utf-8"}
		postbuildcommands
		{
			'{COPYDIR} "Resources" "%{cfg.targetdir}/Resources"',
		}
	
	filter "configurations:Debug"
		defines {"_DEBUG", "_CONSOLE"}
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines {"_NDEBUG"}
		runtime "Release"
		symbols "On"
		optimize "On"
	
	filter "configurations:Shipping"
		defines {"_NDEBUG"}
		runtime "Release"
		symbols "Off"
		optimize "On"