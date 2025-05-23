workspace "GregEngine"
	architecture "x64"
	configurations {"Debug", "Release", "Shipping"}
	startproject "BasketMan"
	
	filter "system:windows"
		buildoptions {"/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

include "SetupGregEngine.lua"

group "ExampleGames"
	include "ExampleGames/BasketMan/SetupBasketMan.lua"
	include "ExampleGames/Sand/SetupSand.lua"
	include "ExampleGames/Sandbox/SetupSandbox.lua"
group ""