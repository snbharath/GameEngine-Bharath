#if defined(VULKAN)

#include "../EngineCommonIncludes.h"
#include "VulkanPhysicalDevice.h"

using namespace GE;
using namespace std;

VulkanPhysicalDevice* VulkanPhysicalDevice::s_pInstance = nullptr;

// ----------------------------------------------------------------------

VulkanPhysicalDevice::VulkanPhysicalDevice()
	: m_numberOfPhysicalDevices(0u)
	, m_listOfPhysicalDevices(nullptr)
	, m_physicalDevice(VK_NULL_HANDLE)
	, m_numberOfQueueFamily(0u)
	, m_pQueueFamilyProperties(nullptr)
{}

// ----------------------------------------------------------------------

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
	if (m_listOfPhysicalDevices != nullptr)
	{
		delete[] m_listOfPhysicalDevices;
	}
	if (m_pQueueFamilyProperties != nullptr)
	{
		delete[] m_pQueueFamilyProperties;
	}
}

// ----------------------------------------------------------------------

VulkanPhysicalDevice* VulkanPhysicalDevice::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new VulkanPhysicalDevice();
	}

	return s_pInstance;
}

// ----------------------------------------------------------------------

bool VulkanPhysicalDevice::DeleteInstance()
{
	if (s_pInstance == nullptr)
	{
		return false;
	}
	
	delete s_pInstance;
	s_pInstance = nullptr;
	return true;
}

// ----------------------------------------------------------------------

bool VulkanPhysicalDevice::InitVulkanPhysicalDevice(const VkInstance vkInstance)
{
	return SelectAPhysicalDevice(vkInstance);
}

// ----------------------------------------------------------------------

bool VulkanPhysicalDevice::SelectAPhysicalDevice(const VkInstance& pInstance)
{
	vkEnumeratePhysicalDevices(pInstance, &m_numberOfPhysicalDevices, nullptr);

	if (m_numberOfPhysicalDevices == 0)
	{
		cerr << "[ERROR] Well, there are no physical devices\n";
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	m_listOfPhysicalDevices = new VkPhysicalDevice[m_numberOfPhysicalDevices];
	assert(m_listOfPhysicalDevices != nullptr);

	vkEnumeratePhysicalDevices(pInstance, &m_numberOfPhysicalDevices, m_listOfPhysicalDevices);

	// well, select the only one we have
	if (m_numberOfPhysicalDevices == 1)
	{
		auto device = m_listOfPhysicalDevices[0];
		if (IsDeviceSuitable(device))
		{
			m_physicalDevice = m_listOfPhysicalDevices[0];
		}
		else
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}

	}
	// if not, lets make it complicated
	else
	{
		vector<string> deviceNames(m_numberOfPhysicalDevices);
		GetListOfAllDeviceNames(m_listOfPhysicalDevices, m_numberOfPhysicalDevices, deviceNames);

		{
			cout << "Select from the following list of devices:\n";
			for (u32 i = 0; i < m_numberOfPhysicalDevices; i++)
			{
				cout << "[" << i << "]" << deviceNames[i] << endl;
			}

			int numberEntered = static_cast<int>(m_numberOfPhysicalDevices);
			while (!(numberEntered >= 0 && numberEntered < static_cast<int>(m_numberOfPhysicalDevices)))
			{
				cout << "Select a device of type 2 : ";
				cin >> numberEntered;
				if (numberEntered >= 0 && numberEntered < static_cast<int>(m_numberOfPhysicalDevices) && IsDeviceSuitable(m_listOfPhysicalDevices[numberEntered]))
				{
					m_physicalDevice = m_listOfPhysicalDevices[numberEntered];
					break;
				}
			}
		}
	}

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
	cout << "Selected : " << deviceProperties.deviceName << endl;

	return true;
}

//----------------------------------------------------------------------

bool VulkanPhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	QueueFamilyIndices indices = FindQueueFamilies(device);
	return (
			(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) &&
			(deviceFeatures.geometryShader) &&
			(indices.isComplete()));
}

//----------------------------------------------------------------------

void VulkanPhysicalDevice::GetListOfAllDeviceNames(VkPhysicalDevice const* devices, u32 numberOfDevices, vector<string>& deviceNames)
{
	VkPhysicalDeviceProperties deviceProperties;
	for (u32 i = 0; i < numberOfDevices; i++)
	{
		char buf[128];
		vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
		sprintf_s(buf, "Name : %s - type : %d", deviceProperties.deviceName, static_cast<int>(deviceProperties.deviceType));
		deviceNames.push_back(buf);
	}
}

//----------------------------------------------------------------------

QueueFamilyIndices VulkanPhysicalDevice::FindQueueFamilies(VkPhysicalDevice physicalDevice)
{
	QueueFamilyIndices indices;
	
	u32 queueFamilycount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &m_numberOfQueueFamily, nullptr);
	if (m_numberOfQueueFamily == 0)
	{
		assert(false); // What the hell ? 
	}

	m_pQueueFamilyProperties = new VkQueueFamilyProperties[queueFamilycount];
	assert(m_pQueueFamilyProperties != nullptr);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &m_numberOfQueueFamily, m_pQueueFamilyProperties);

	for (u32 i = 0; i < m_numberOfQueueFamily; i++)
	{
		if (m_pQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}
		if (indices.isComplete())
		{
			break;
		}
	}

	return indices;
}

#endif // VULKAN