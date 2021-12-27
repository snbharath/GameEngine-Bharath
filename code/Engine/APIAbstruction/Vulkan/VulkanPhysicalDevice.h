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
	// a struct to hold the queue family index provided by vulkan
	struct QueueFamilyIndices
	{
		std::optional<u32> graphicsFamily;

		bool isComplete() {
			return graphicsFamily.has_value();
		}
	};

	class VulkanPhysicalDevice
	{
		private:
			VulkanPhysicalDevice();
			~VulkanPhysicalDevice();

			// Get a list of all the physical devices
			u32 m_numberOfPhysicalDevices;
			VkPhysicalDevice* m_listOfPhysicalDevices;
			VkPhysicalDevice m_physicalDevice;

			// Queue family data
			u32 m_numberOfQueueFamily; // for selected devices ofcourse!
			VkQueueFamilyProperties* m_pQueueFamilyProperties;
			
			static VulkanPhysicalDevice* s_pInstance;

		public:

			static VulkanPhysicalDevice* GetInstance();

			static bool DeleteInstance();

			// Select vulkan physical device and select queue families
			bool InitVulkanPhysicalDevice(const VkInstance pInstance);

			u32 GetNumberOfVulkanPhysicalDevices() const;

			VkPhysicalDevice GetSelectedVulkanPhysicalDevice() const;

		private:
			// Perform a physical device selection
			bool SelectAPhysicalDevice(const VkInstance& pInstance);

			// Function to check suitable devices
			bool IsDeviceSuitable(VkPhysicalDevice device);

			void GetListOfAllDeviceNames(VkPhysicalDevice const* devices, u32 numberOfDevices, std::vector<std::string>& deviceNames);

			// Find the queue families for the selected device
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
	};

	//--------------------------------------------------------------------------------------

	inline u32 VulkanPhysicalDevice::GetNumberOfVulkanPhysicalDevices() const
	{
		return m_numberOfPhysicalDevices;
	}

	//--------------------------------------------------------------------------------------

	inline VkPhysicalDevice VulkanPhysicalDevice::GetSelectedVulkanPhysicalDevice() const
	{
		return m_physicalDevice;
	}

	//--------------------------------------------------------------------------------------
}

#endif // VULKAN