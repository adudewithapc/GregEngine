project "GregEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	
	files {"Source/**.h", "Source/**.cpp", "Libraries/Source/**.c"}
	
	vulkan_sdk = os.getenv("VULKAN_SDK")
	
	if vulkan_sdk == nil then
		error("Vulkan SDK path was not found. Check if it is installed.")
	end
	
	includedirs
	{
		"Includes/glad",
		"Includes/KHR",
		"Includes/stb_image",
		vulkan_sdk .. "/Include"
	}
	
	libdirs
	{
		vulkan_sdk .. "/Lib"
	}
	
	links
	{
		"opengl32",
		"Ws2_32",
		"Winmm",
		"vulkan-1.lib"
	}
	
	defines
	{
		"GREG_OPENGL"
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