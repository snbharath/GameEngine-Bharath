#pragma once

/*
* class definition for selecting vulkan physical device and queue families for various command buffers
* VulkanPhysicalDevice.h
*/

#if defined(VULKAN)

#include <optional>
#include "../EngineCommonIncludes.h"

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
		vector < optional<u32>> m_queueFamilies;

		QueueFamilyIndices()
		{
			for (u32 i = 0; i < static_cast<u32>(QueueFamilies::MAX); i++)
			{
				m_queueFamilies.push_back(optional<u32>());
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
		vector<VkSurfaceFormatKHR> formats;

		// Available presentation modes
		vector<VkPresentModeKHR> presentModes;
	};

	//---------------------------------------------------------------------------

	class VulkanPhysicalDevice
	{
		private:
			VulkanPhysicalDevice();
			~VulkanPhysicalDevice();

			// Get a list of all the physical devices
			u32 m_numberOfPhysicalDevices;
			VkPhysicalDevice* m_listOfPhysicalDevices;
			VkPhysicalDevice m_physicalDevice;

			// Logical device instance
			VkDevice m_logicalDevice;
			
			vector<VkQueue> m_queues;

			// Swap Chain handle
			VkSwapchainKHR m_swapChain;

			// Holder for swap chain images
			vector<VkImage> m_swapChainImages;

			VkFormat m_swapChainImageFormat;
			VkExtent2D m_swapChainExtent;


			static VulkanPhysicalDevice* s_pInstance;

		public:

			static VulkanPhysicalDevice* GetInstance();

			static bool DeleteInstance();

			// Select vulkan physical device and select queue families
			bool InitVulkanPhysicalDevice();

			u32 GetNumberOfVulkanPhysicalDevices() const;

			const VkPhysicalDevice GetSelectedVulkanPhysicalDevice();

			bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

		private:
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
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableSurfaceFormats);

			// Presentation modes, conditions for swapping images to the screen
			VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes);

			// Swap extent, resolution of images in swap chain
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

			// Find the queue families for the selected device
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);

			// Create a swap chain with the selected parameters
			bool CreateSwapChain();

	};

	//--------------------------------------------------------------------------------------

	inline u32 VulkanPhysicalDevice::GetNumberOfVulkanPhysicalDevices() const
	{
		return m_numberOfPhysicalDevices;
	}

	//--------------------------------------------------------------------------------------

	inline const VkPhysicalDevice VulkanPhysicalDevice::GetSelectedVulkanPhysicalDevice()
	{
		return m_physicalDevice;
	}

	//--------------------------------------------------------------------------------------
}

#endif // VULKAN