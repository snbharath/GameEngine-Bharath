-- premake5.lua
workspace "GameEngineMain" -- can also be used as workspace("GameEngineMain")
configurations { "Debug", "Debug_D3D", "Debug_OpenGL", "Release", "Release_D3D", "Release_OpenGL"}
platforms {"Win32", "Win64", "XBoxOne"}


local lang = "C++"

project "GameEngine"
kind "ConsoleApp"
language (lang)
targetdir "../bin/%{cfg.buildcfg}"
location "Engine"
files { "Engine/**" }
--files { "Engine/**.c" }
--files { "Engine/**.cpp" }

-- Normal Debug version
filter { "configurations:Debug" }
  defines { "DEBUG" }
  symbols "On"

-- Debug and Debug directx are all same 
filter "configurations:Debug_D3D"
  defines { "DEBUG" }
  symbols "On"
-- Debug Open GL version
filter "configurations:Debug_OpenGL"
  defines { "DEBUG" }
  symbols "On"

-- Normal Release version
filter "configurations:Release"
  defines { "NDEBUG" }
  optimize "On"

filter "configurations:Release_D3D"
  defines { "NDEBUG" }
  optimize "On"
  
filter "configurations:Release_OpenGL"
  defines { "NDEBUG" }
  optimize "On"
  
filter{ "platforms:Win32"}
  system "Windows"
  architecture "x32"
  
filter{"platforms:Win64"}
  system "Windows"
  architecture "x64"
  
filter{ "platforms:Xbox" }
  system "Xbox360"
  
