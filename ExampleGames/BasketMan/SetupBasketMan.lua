project "BasketMan"
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
	
	filter "system:windows"
		systemversion "latest"
		buildoptions {"/utf-8"}
		postbuildcommands
		{
			'{COPY} "Shader/*" "%{cfg.targetdir}/Shader"',
			'{COPY} "Textures/*" "%{cfg.targetdir}/Textures"'
		}
	
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