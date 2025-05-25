project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	
	files { "Source/**.h", "Source/**.cpp"}
	
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
	objdir ("Binaries/Intermediate/" .. outputdir)
	debugdir "%{cfg.targetdir}"
	
	postbuildcommands
	{
		'{COPYDIR} "Binaries/Shaders" "%{cfg.targetdir}/Shaders"'
	}
	
	filter "system:windows"
		systemversion "latest"
		buildoptions {"/utf-8"}
	
	filter "configurations:Debug"
		defines {"_DEBUG", "_CONSOLE"}
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines {"NDEBUG"}
		runtime "Release"
		symbols "On"
		optimize "On"
	
	filter "configurations:Shipping"
		defines {"NDEBUG"}
		runtime "Release"
		symbols "Off"
		optimize "On"