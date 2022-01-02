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

			static VulkanPhysicalDevice* s_pInstance;

		public:

			static VulkanPhysicalDevice* GetInstance();

			static bool DeleteInstance();

			// Select vulkan physical device and select queue families
			bool InitVulkanPhysicalDevice();

			u32 GetNumberOfVulkanPhysicalDevices() const;

			const VkPhysicalDevice GetSelectedVulkanPhysicalDevice();

		private:
			// Perform a physical device selection
			bool SelectAPhysicalDevice(const VkInstance& pInstance);

			// Function to check suitable devices
			bool IsDeviceSuitable(VkPhysicalDevice device);

			void GetListOfAllDeviceNames(VkPhysicalDevice const* devices, u32 numberOfDevices, std::vector<std::string>& deviceNames);

			// Create a logical device for the selected physical device
			bool CreateLogicalDevice();

			// Find the queue families for the selected device
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
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