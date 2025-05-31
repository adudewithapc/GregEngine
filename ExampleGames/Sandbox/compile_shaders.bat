@echo off
if not exist "Binaries" mkdir "Binaries"
if not exist "Binaries\Shaders" mkdir "Binaries\Shaders"
echo Compiling Shaders/triangle_vertex.shader...
Z:\VulkanSDK\Bin\glslc.exe Shaders/triangle_vertex.shader -o Binaries/Shaders/triangle_vertex.spv
echo Compiling Shaders/triangle_fragment.shader...
Z:\VulkanSDK\Bin\glslc.exe Shaders/triangle_fragment.shader -o Binaries/Shaders/triangle_fragment.spv
pause