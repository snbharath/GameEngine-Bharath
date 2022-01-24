/*
* Author @Bharath - 2020
* This code abstracts the basic window creation of OpenGL
*
*/
#pragma once

#ifdef VULKAN
#include "../EngineCommonIncludes.h"

#include <optional>
#include "GEWindow.h"
#include "glfw3.h"

namespace GE 
{
	enum class QueueFamilies : u32
	{
		GRAPHICS,
		PRESENTATION,
		MAX
	};

	const float c_queuePriority = 1.0f;

	//---------------------------------------------------------------------------

	// a struct to hold the queue family index provided by vulkan
	struct QueueFamilyIndices
	{
		std::vector <std::optional<u32>> m_queueFamilies;

		QueueFamilyIndices()
		{
			for (u32 i = 0; i < static_cast<u32>(QueueFamilies::MAX); i++)
			{
				m_queueFamilies.push_back(std::optional<u32>());
			}
		}

		bool isComplete()
		{
			bool res = true;
			for (u32 i = 0; i < static_cast<u32>(QueueFamilies::MAX); i++)
			{
				res &= m_queueFamilies[i].has_value();
			}

			return res;
		}
	};

	//---------------------------------------------------------------------------

	// Swap chain support details
	struct SwapChainSupportDetails
	{
		// Basic Surface Capabilities. (min/max) number of images in swap chain, (min/max) width and height of images
		VkSurfaceCapabilitiesKHR capabilities;

		// Pixel format , color space
		std::vector<VkSurfaceFormatKHR> formats;

		// Available presentation modes
		std::vector<VkPresentModeKHR> presentModes;
	};

	// ---------------------------------------------------------------------------

	class VulkanWindow : public GEWindow
	{
	private:
		VulkanWindow();
		~VulkanWindow();

		// GLFW window instance
		GLFWwindow * m_pGLFWwindow;
		// Vulkan instance
		VkInstance	m_VulkanInstance;

		// Window surface creation
		VkSurfaceKHR m_windowSurface;

		// Get a list of all the physical devices
		u32 m_numberOfPhysicalDevices;
		VkPhysicalDevice* m_listOfPhysicalDevices;
		VkPhysicalDevice m_physicalDevice;

		// Logical device instance
		VkDevice m_logicalDevice;

		std::vector<VkQueue> m_queues;

		// Swap Chain handle
		VkSwapchainKHR m_swapChain;

		// Holder for swap chain images
		std::vector<VkImage> m_swapChainImages;

		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;

		// Swap chain image views
		std::vector<VkImageView> m_swapChainImageViews;

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

		// Get GLFW window
		GLFWwindow* GetGLFWwindow() const;

		// Get Vulkan Instance
		const VkInstance GetVulkanInstance();

		// Get created window surface
		const VkSurfaceKHR GetWindowSurface();

		// Create a OpenGL window instance here and return it the base class
		static VulkanWindow* GetInstance();
		//Delete singleton instance
		static bool DeleteInstance();

		u32 GetNumberOfVulkanPhysicalDevices() const;

		const VkPhysicalDevice GetSelectedVulkanPhysicalDevice();

		bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

	private:
		// Create Vulkan Instance
		bool CreateVulkanInstance();

		// utility function to check available vulkan extensions
		void PrintVulkanAvailableExtensions(const char ** pGivenExtensionNames, u32 givenExtensionCount);

		// A window surface is required to render the frames on to.
		bool CreateWindowSurface();

		// Perform a physical device selection
		bool SelectAPhysicalDevice(const VkInstance& pInstance);

		// Function to check suitable devices
		bool IsDeviceSuitable(VkPhysicalDevice device);

		void GetListOfAllDeviceNames(VkPhysicalDevice const* devices, u32 numberOfDevices, std::vector<std::string>& deviceNames);

		// Create a logical device for the selected physical device
		bool CreateLogicalDevice();

		// Query for swap chain support for a given physical device
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice);

		// Choose swap surface formats, color depth
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats);

		// Presentation modes, conditions for swapping images to the screen
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		// Swap extent, resolution of images in swap chain
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		// Find the queue families for the selected device
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);

		// Create a swap chain with the selected parameters
		bool CreateSwapChain();

		// Create image views for all the VkImages in the swap chain buffer
		bool CreateImageViews();

		/*Create Graphics pipeline
		* Graphics pipeline include the following modules. Ones within [] are non programmable but some parameters can be tweaked.
		* But, ones with in () are programmable through shaders
		* Vertex/Index buffer
		*         |
		*         v
		* [Input Assembler]
		*         |
		*         v
		* (Vertex Shader)
		*         |
		*         v
		* (Tessellation)
		*         |
		*         v
		* (Geometry Shader)
		*         |
		*         v
		* [Rasterization]
		*         |
		*         v
		* (Fragment Shader)
		*         |
		*         v
		* [Color Blending]
		*         |
		*         v
		* Frame Buffer.
		*/
		bool CreateGraphicsPipeline();

		// Create shader module
		void CreateShaderModule(VkShaderModule& shaderModule, const std::vector<char>& code);
	};

	// ----------------------------------------------------------------------------------
	
	inline GLFWwindow* VulkanWindow::GetGLFWwindow() const 
	{
		return m_pGLFWwindow;
	}

	// ----------------------------------------------------------------------------------

	inline const VkInstance VulkanWindow::GetVulkanInstance()
	{
		return m_VulkanInstance;
	}

	// ----------------------------------------------------------------------------------

	inline const VkSurfaceKHR VulkanWindow::GetWindowSurface()
	{
		return m_windowSurface;
	}

	// ----------------------------------------------------------------------------------

	inline u32 VulkanWindow::GetNumberOfVulkanPhysicalDevices() const
	{
		return m_numberOfPhysicalDevices;
	}

	//--------------------------------------------------------------------------------------

	inline const VkPhysicalDevice VulkanWindow::GetSelectedVulkanPhysicalDevice()
	{
		return m_physicalDevice;
	}

	//--------------------------------------------------------------------------------------
}

#endif // VULKANVulkanWindow