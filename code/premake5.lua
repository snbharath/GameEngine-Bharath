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
----------------------------------------------------------------------------------------

-- premake5.lua
workspace "GameEngineMain" -- can also be used as workspace("GameEngineMain")

-- record the platform
platform = os.target()

----------------------------------------------------------------------------------------

local lang = "C++"
local Operating_System
local vulkan_string = "VULKAN"
local opengl_string = "OPENGL"
local directx_string = "DIRECTX"
local vulkan_sdk = os.getenv("VK_SDK_PATH")
local machine_type = os.getenv("ARCH_TYPE")

----------------------------------------------------------------------------------------

--platform dependent configuration files
if( platform == "windows" ) then
   configurations { "Debug_DX", "Debug_GL", "Debug_Vulkan", "Release_DX", "Release_GL", "Release_Vulkan" }
   platforms {"Win64"}
   systemversion("10.0.18362.0") -- make sure to change this to whichever version of Windows SDK you have on your computer. if not it defaults to 8.1
else
   configurations { "Debug_GL", "Debug_Vulkan", "Release_GL", "Release_Vulkan" } -- just OpenGL rendering subsystem only
   if(machine_type == "aarch64") then
      platforms {"arm64"}
   else
      platforms {"amd64"}
   end
end

----------------------------------------------------------------------------------------

-- Generation of project files starts from here
project "GameEngine"
kind "ConsoleApp"
language (lang)
targetdir "./bin/%{cfg.buildcfg}/%{cfg.platform}"
-- libdirs  ( <follow the target dir pattern> )
location "Engine"
files { "Engine/**" }

----------------------------------------------------------------------------------------

if( platform == "windows" ) then
flags { "FatalCompileWarnings", "FatalLinkWarnings", "FatalWarnings"}
end

----------------------------------------------------------------------------------------

cppdialect "C++20"

----------------------------------------------------------------------------------------

-- adding in the library directory
if( platform == "windows" ) then
   --configuration {"Debug_Vulkan", "Release_Vulkan"}
   libdirs { "RenderingLibraries/lib/%{cfg.platform}/**" }
else
   libdirs { "RenderingLibraries/lib/%{cfg.platform}/**" }
end

----------------------------------------------------------------------------------------

--files { "Engine/**.c" }
--files { "Engine/**.cpp" }

-- opengl32 comes with the windows sdk
if( platform == "windows" ) then
   includedirs { "Engine/**/", "RenderingLibraries/include/**", "Engine/include/opengl_ver/" }
   filter "Debug_GL"
      links { "opengl32", "glfw3_d" }
   filter "Release_GL"
      links { "opengl32", "glfw3" }
   filter "Debug_Vulkan"
      libdirs { vulkan_sdk .. "/Lib/" }
      includedirs { vulkan_sdk .. "/Include" }
      links {"vulkan-1", "glfw3_d"}
   filter "Release_Vulkan"
      libdirs { vulkan_sdk .. "/Lib/" }
      includedirs { vulkan_sdk .. "/Include" }
      links {"vulkan-1", "glfw3"}
else
   includedirs { "Engine/**/", "RenderingLibraries/include/**", "Engine/include/opengl_ver/", "/usr/include/GL/"}
   links { "glfw", "dl", "GL" } 
   
   filter "Debug_Vulkan"
      libdirs { vulkan_sdk .. "/lib" }
      includedirs { vulkan_sdk .. "/include" }
      links { "vulkan" }
   filter "Release_Vulkan"
      libdirs { vulkan_sdk .. "/lib" }
      includedirs { vulkan_sdk .. "/include" }
      links { "vulkan" }
end

----------------------------------------------------------------------------------------

-- Degub stuff here
-- if you want OPENGL by default change "DIRECT3D" to "OPENGL"
if( platform == "windows" ) then
   filter { "configurations:Debug_DX" }
      defines { "DEBUG", directx_string}
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

----------------------------------------------------------------------------------------

-- platform configuration for 64-bit build
if( platform == "windows" ) then 
   filter{ "platforms:Win64" } 
      system "Windows" 
      architecture "x64"
      defines{ "PC" }
else
   if(machine_type == "aarch64") then -- check the architecture for setting up the project files.
      filter{ "platforms:arm64" }
         system "Linux"
         architecture "ARM64"
         defines{ "LINUX" }
   else
      filter{ "platforms:amd64" } 
         system "Linux" 
         architecture "x64"
   end
end

----------------------------------------------------------------------------------------