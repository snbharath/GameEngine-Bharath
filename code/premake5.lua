-- premake5.lua
workspace "GameEngineMain" -- can also be used as workspace("GameEngineMain")

--platform dependent configuration files
if( os.get() == "windows" ) then
configurations { "Debug", "Debug_D3D", "Debug_OpenGL", "Release", "Release_D3D", "Release_OpenGL"}
platforms {"Win32", "Win64", "XBoxOne"}
else
configurations { "Debug", "Release"} -- just OpenGL rendering subsystem only
platforms {"i386", "amd64"}
end

local lang = "C++"
local Rendering_Subsystem
local Operating_System
local opengl_string = "OPENGL"
local directx_string = "DIRECT3D"

project "GameEngine"
kind "ConsoleApp"
language (lang)
targetdir "./bin/%{cfg.buildcfg}/%{cfg.platform}"
-- libdirs  ( <follow the target dir pattern> )
location "Engine"
files { "Engine/**" }
--files { "Engine/**.c" }
--files { "Engine/**.cpp" }

--detect the operating system
if(os.get() == "windows") then
	Rendering_Subsystem = "DIRECT3D"
else
	Rendering_Subsystem = "OPENGL"
end


-- Degub stuff here
-- Normal Debug version, by default it uses DIRECT X, if you want OPENGL by default change "DIRECT3D" to "OPENGL"
if( os.get() == "windows" ) then filter { "configurations:Debug" } defines { "DEBUG", directx_string } symbols "On"
else filter { "configurations:Debug" } defines { "DEBUG", opengl_string } symbols "On" end

-- Debug directx make sure this configuration exists only for windows 
if( os.get() == "windows" ) then filter "configurations:Debug_D3D" defines { "DEBUG", directx_string } symbols "On" end
  
-- Debug Open GL version, both Windows and Linux
filter "configurations:Debug_OpenGL" defines { "DEBUG", opengl_string } symbols "On"


-- Release stuff here
-- Normal Release version, by default it uses DIRECT X, if you want OPENGL by default change "DIRECT3D" to "OPENGL"
if( os.get() == "windows" ) then filter "configurations:Release" defines { "NDEBUG", directx_string } optimize "On"
else filter "configurations:Release" defines { "NDEBUG", opengl_string } optimize "On" end

--Release Directx make sure this configuration exists only for windows platform
if( os.get() == "windows" ) then filter "configurations:Release_D3D" defines { "NDEBUG", directx_string} optimize "On" end

--Release OpenGL, both for windows and Linux  
filter "configurations:Release_OpenGL" defines { "NDEBUG", directx_string} optimize "On"


-- platform configuration for 32-bit build
if( os.get() == "windows" ) then filter{ "platforms:Win32" } system "Windows" architecture "x32"
else filter{ "platforms:Win32" } system "Linux" architecture "x32" end

-- platform configuration for 64-bit build
if( os.get() == "windows" ) then filter{ "platforms:Win64" } system "Windows" architecture "x64"
else filter{ "platforms:Win64" } system "Linux" architecture "x64" end

--Make sure the stuff we are developing for Xbox is from windows, there is no support for Linux development environment for XBox
if(os.get() == "windows" ) then filter{ "platforms:Xbox" } system "Xbox360" end
