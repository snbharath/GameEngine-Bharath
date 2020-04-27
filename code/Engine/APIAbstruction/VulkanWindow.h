/*
* Author @Bharath - 2020
* This code abstracts the basic window creation of OpenGL
*
*/
#pragma once

#ifdef VULKAN

#include "GEWindow.h"
#include "glfw3.h"
#include "../EngineCommonIncludes.h"

namespace GE 
{
	class VulkanWindow : public GEWindow
	{
	private:
		VulkanWindow();
		~VulkanWindow();

		// GLFW window instance
		GLFWwindow * m_pGLFWwindow;
		// Vulkan instance
		VkInstance m_VulkanInstance;

		// Singleton instance
		static VulkanWindow* s_pInstance;

	public:

		// Function to init window
		bool InitWindow() override;

		// Function implementation for Window creation. returns 0 if everything is Ok, else non-zero
		bool CreateGEWindow() override;

		// All implementation of Open GL render window will go here
		bool RenderFrameUpdate() override;

		// Function implementation for Closing OpenGL window. return 0 if closed properly, 1 if closed with some errors
		void CloseGEWindow() override;

		// Terminate GE window
		void TerminateGEWindow() override;

		void ProcessInput(GLFWwindow*);

		// Create a OpenGL window instance here and return it the base class
		static VulkanWindow* GetInstance();
		//Delete singleton instance
		static bool DeleteInstance();

	private:
		// Create Vulkan Instance
		void CreateVulkanInstance();

		// utility function to check available vulkan extensions
		void PrintVulkanAvailableExtensions(const char ** pGivenExtensionNames, u32 givenExtensionCount);
	};
}

#endif // VULKAN