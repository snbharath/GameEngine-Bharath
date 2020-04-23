-- add all the binary directories that you want to clean
newaction {
   trigger     = "clean",
   description = "clean the software",
   execute     = function ()
      print("clean the build...")
      os.rmdir("./bin")
	   os.rmdir("./Engine/obj")
      print("done.")
   end
}


-- premake5.lua
workspace "GameEngineMain" -- can also be used as workspace("GameEngineMain")

-- record the platform
platform = os.target()

--platform dependent configuration files
if( platform == "windows" ) then
   configurations { "Debug_DX", "Debug_GL", "Debug_Vulkan", "Release_DX", "Release_GL", "Release_Vulkan" }
   platforms {"Win64"}
   systemversion("10.0.18362.0") -- make sure to change this to whichever version of Windows SDK you have on your computer. if not it defaults to 8.1
else
   configurations { "Debug_GL", "Debug_Vulkan", "Release_GL", "Release_Vulkan" } -- just OpenGL rendering subsystem only
   platforms {"amd64"}
end

local lang = "C++"
local Operating_System
local vulkan_string = "VULKAN"
local opengl_string = "OPENGL"
local directx_string = "DIRECTX"

-- Generation of project files starts from here
project "GameEngine"
kind "ConsoleApp"
language (lang)
targetdir "./bin/%{cfg.buildcfg}/%{cfg.platform}"
-- libdirs  ( <follow the target dir pattern> )
location "Engine"
files { "Engine/**" }
flags { "FatalCompileWarnings", "FatalLinkWarnings", "FatalWarnings"}

-- adding in the library directory
if( platform == "windows" ) then
   --configuration {"Debug_Vulkan", "Release_Vulkan"}
   libdirs { "RenderingLibraries/lib/%{cfg.platform}/**" }
else
   libdirs { "RenderingLibraries/lib/%{cfg.platform}/**" }
end

--files { "Engine/**.c" }
--files { "Engine/**.cpp" }

-- opengl32 comes with the windows sdk
if( platform == "windows" ) then
   includedirs { "Engine/**/", "RenderingLibraries/include/**", "Engine/include/opengl_ver/" }
   configuration "Debug_GL"
      links { "opengl32", "glfw3_d" }
   configuration "Release_GL"
      links { "opengl32", "glfw3" }
   configuration "Debug_Vulkan"
      libdirs { "C:/VulkanSDK/1.2.135.0/Lib/" }
      includedirs { "C:/VulkanSDK/1.2.135.0/Include" }
      links {"vulkan-1.lib", "glfw3_d"}
   configuration "Release_Vulkan"
      libdirs { "C:/VulkanSDK/1.2.135.0/Lib/" }
      includedirs { "C:/VulkanSDK/1.2.135.0/Include" }
      links {"vulkan-1.lib", "glfw3"}
else
   links { "glfw", "dl", "GL" } 
   includedirs { "Engine/**/", "RenderingLibraries/include/**", "Engine/include/opengl_ver/", "/usr/include/GL/"}
end


-- Degub stuff here
-- if you want OPENGL by default change "DIRECT3D" to "OPENGL"
if( platform == "windows" ) then
   filter { "configurations:Debug_DX" }
      defines { "DEBUG", directx_string }
      symbols "On"
end

-- General for both windows and linux
filter { "configurations:Debug_GL" }
   defines { "DEBUG", opengl_string }
   symbols "On"
filter { "configurations:Debug_Vulkan" }
   defines { "DEBUG", vulkan_string }
   symbols "On"


-- Release stuff here
-- Normal Release version, by default it uses DIRECT X, if you want OPENGL by default change "DIRECT3D" to "OPENGL"
if( platform == "windows" ) then 
   filter "configurations:Release_DX" 
      defines { "NDEBUG", directx_string } 
      optimize "On"
end

-- for both windows and linux
filter "configurations:Release_GL" 
   defines { "NDEBUG", opengl_string } 
   optimize "On"
filter "configurations:Release_Vulkan" 
   defines { "NDEBUG", vulkan_string } 
   optimize "On"

-- platform configuration for 64-bit build
if( platform == "windows" ) then 
   filter{ "platforms:Win64" } 
      system "Windows" 
      architecture "x64"
else 
   filter{ "platforms:amd64" } 
      system "Linux" 
      architecture "x64" 
end