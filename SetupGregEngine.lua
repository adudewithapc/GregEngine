project "GregEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	
	files {"Source/**.h", "Source/**.cpp", "Libraries/Source/**.c"}
	
	includedirs
	{
		"Includes/glad",
		"Includes/KHR",
		"Includes/stb_image"
	}
	
	links
	{
		"opengl32",
		"Ws2_32",
		"Winmm"
	}
	
	targetdir ("Binaries/" .. outputdir)
	objdir ("Binaries/Intermediate/" .. outputdir)
	
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