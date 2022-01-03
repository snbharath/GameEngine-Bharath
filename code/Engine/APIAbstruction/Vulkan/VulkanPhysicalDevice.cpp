#if defined(VULKAN)

#include "../EngineCommonIncludes.h"
#include "VulkanValidationLayerWrapper.h"
#include "VulkanWindow.h"
#include <set>
#include <string>
#include <cstdint> // Necessary for UINT32_MAX
#include <algorithm> // Necessary for std::clamp


#include "VulkanPhysicalDevice.h"

using namespace GE;
using namespace std;

const vector<const char*> c_deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// ----------------------------------------------------------------------

VulkanPhysicalDevice* VulkanPhysicalDevice::s_pInstance = nullptr;

// ----------------------------------------------------------------------

VulkanPhysicalDevice::VulkanPhysicalDevice()
	: m_numberOfPhysicalDevices(0u)
	, m_listOfPhysicalDevices(nullptr)
	, m_physicalDevice(VK_NULL_HANDLE)
	, m_logicalDevice(VK_NULL_HANDLE)
	, m_swapChain(VK_NULL_HANDLE)
{
	// initialize the queues
	for (u32 i = 0; i < static_cast<u32>(QueueFamilies::MAX); i++)
	{
		m_queues.push_back(nullptr);
	}
}

// ----------------------------------------------------------------------

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
	if (m_swapChain != nullptr)
	{
		vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
	}
	if (m_logicalDevice != nullptr)
	{
		vkDestroyDevice(m_logicalDevice, nullptr);
	}

	if (m_listOfPhysicalDevices != nullptr)
	{
		delete[] m_listOfPhysicalDevices;
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

bool VulkanPhysicalDevice::InitVulkanPhysicalDevice()
{
	VulkanWindow* pVulkanWindow = VulkanWindow::GetInstance();
	assert(pVulkanWindow != nullptr);
	if (pVulkanWindow == nullptr)
	{
		return false;
	}

	bool isSuccess = true;
	
	if (isSuccess)
	{
		cout << "[VulkanPhysicalDevice::InitVulkanPhysicalDevice] - Selecting a physical device. Line: " << __LINE__ << endl;
		isSuccess = SelectAPhysicalDevice(pVulkanWindow->GetVulkanInstance());
	}

	if (isSuccess)
	{
		cout << "[VulkanPhysicalDevice::InitVulkanPhysicalDevice] - Creating a logical device. Line: " << __LINE__ << endl;
		isSuccess = CreateLogicalDevice();
	}

	if (isSuccess)
	{
		cout << "[VulkanPhysicalDevice::InitVulkanPhysicalDevice] - Creating a Swap Chain. Line: " << __LINE__ << endl;
		isSuccess = CreateSwapChain();
	}

	return isSuccess;
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

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool isSwapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails details = QuerySwapChainSupport(device);
		isSwapChainAdequate = !details.formats.empty() && !details.presentModes.empty();
	}

	return (
		(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) &&
		(deviceFeatures.geometryShader) &&
		(indices.isComplete())&&
		(extensionsSupported) &&
		(isSwapChainAdequate));
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
	VulkanWindow* pVulkanWindow = VulkanWindow::GetInstance();
	assert(pVulkanWindow);

	QueueFamilyIndices indices;
	
	u32 queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	if (queueFamilyCount == 0)
	{
		assert(false); // What the hell ? 
	}

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	for (u32 i = 0; i < queueFamilyCount; i++)
	{
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.m_queueFamilies[static_cast<u32>(QueueFamilies::GRAPHICS)] = i;
		}
		
		VkBool32 presentSupport = false;
		auto res = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, pVulkanWindow->GetWindowSurface(), &presentSupport);
		if (presentSupport)
		{
			indices.m_queueFamilies[static_cast<u32>(QueueFamilies::PRESENTATION)] = i;
		}

		if (indices.isComplete())
		{
			break;
		}
	}

	return indices;
}

//----------------------------------------------------------------------

bool VulkanPhysicalDevice::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(GetSelectedVulkanPhysicalDevice());

	vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	for (u32 i = 0; i < static_cast<u32>(QueueFamilies::MAX); i++)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.m_queueFamilies[i].value();
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &c_queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Lets just use defaults for now
	VkPhysicalDeviceFeatures physicalDeviceFeatures{};

	//Create a logical device for the selected physical device
	VkDeviceCreateInfo logicalDeviceCreateInfo{};
	logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logicalDeviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	logicalDeviceCreateInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());	
	logicalDeviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
	logicalDeviceCreateInfo.enabledExtensionCount = static_cast<u32>(c_deviceExtensions.size());
	logicalDeviceCreateInfo.ppEnabledExtensionNames = c_deviceExtensions.data();

	// Check if validation layers are enabled
	VulkanValidationLayerWrapper* pValidationLayerInstance = VulkanValidationLayerWrapper::GetInstance();
	if (pValidationLayerInstance != nullptr && pValidationLayerInstance->IsEnableValidationLayer())
	{
		auto validationLayers = pValidationLayerInstance->GetValidationLayers();
		logicalDeviceCreateInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
		logicalDeviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		cerr << "Validation layer requested, is not available" << endl;
		logicalDeviceCreateInfo.enabledLayerCount = 0;
		return false;
	}

	if (vkCreateDevice(m_physicalDevice, &logicalDeviceCreateInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	for (u32 i = 0; i < static_cast<u32>(QueueFamilies::MAX); i++)
	{
		vkGetDeviceQueue(m_logicalDevice, indices.m_queueFamilies[i].value(), 0, &m_queues[i]);
	}

	return true;
}

//----------------------------------------------------------------------

SwapChainSupportDetails VulkanPhysicalDevice::QuerySwapChainSupport(VkPhysicalDevice physicalDevice)
{
	VulkanWindow* pVulkanWindow = VulkanWindow::GetInstance();
	assert(pVulkanWindow);

	SwapChainSupportDetails swapChainSupportDetails;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, pVulkanWindow->GetWindowSurface(), &swapChainSupportDetails.capabilities);


	// Query for window surface formats
	u32 formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, pVulkanWindow->GetWindowSurface(), &formatCount, nullptr);
	if (formatCount != 0)
	{
		swapChainSupportDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, pVulkanWindow->GetWindowSurface(), &formatCount, swapChainSupportDetails.formats.data());
	}

	// query for surface present modes
	u32 presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, pVulkanWindow->GetWindowSurface(), &presentModeCount, nullptr);
	if (presentModeCount != 0)
	{
		swapChainSupportDetails.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, pVulkanWindow->GetWindowSurface(), &presentModeCount, swapChainSupportDetails.presentModes.data());
	}


	return swapChainSupportDetails;
}

//----------------------------------------------------------------------

bool VulkanPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
	u32 deviceExtensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &deviceExtensionCount, nullptr);

	vector<VkExtensionProperties> availableExtensions(deviceExtensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &deviceExtensionCount, availableExtensions.data());

	std::set<string> requiredExtensions(c_deviceExtensions.begin(), c_deviceExtensions.end());
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

//----------------------------------------------------------------------

VkSurfaceFormatKHR VulkanPhysicalDevice::ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableSurfaceFormats)
{
	for (const auto& availableFormat : availableSurfaceFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	cout << "[Warning] Didn't find the required swap surface format, selecting one from the list" << endl;
	return availableSurfaceFormats[0];
}

//----------------------------------------------------------------------

VkPresentModeKHR VulkanPhysicalDevice::ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR)
		{
			return availablePresentMode;
		}
	}

	cout << "[Warning] Didn't find the required present mode, selecting the one that is guaranteed to be available" << endl;
	return VK_PRESENT_MODE_FIFO_KHR;
}

//----------------------------------------------------------------------

VkExtent2D VulkanPhysicalDevice::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else // clamping is necessary when you have a window created on a high DPI screen because the screen coordinates don't correspond to the pixels
	{
		const auto* pVulkanWindow = VulkanWindow::GetInstance();
		assert(pVulkanWindow != nullptr);

		int width, height;
		glfwGetFramebufferSize(pVulkanWindow->GetGLFWwindow(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<u32>(width),
			static_cast<u32>(height)
		};

		actualExtent.width = clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

//----------------------------------------------------------------------

bool VulkanPhysicalDevice::CreateSwapChain()
{
	VulkanWindow* pVulkanWindow = VulkanWindow::GetInstance();
	assert(pVulkanWindow);
	if (pVulkanWindow == nullptr)
	{
		return false;
	}

	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(GetSelectedVulkanPhysicalDevice());

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extents = ChooseSwapExtent(swapChainSupport.capabilities);

	u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// Create Vulkan swap chain
	VkSwapchainCreateInfoKHR swapChainCreateInfo{};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = pVulkanWindow->GetWindowSurface();
	swapChainCreateInfo.minImageCount = imageCount;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = extents;
	swapChainCreateInfo.imageArrayLayers = 1; // amount of layers each image consists of 
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;// Bit Field, What kinda operations we'll use the images in the swap chain for.

	QueueFamilyIndices indices = FindQueueFamilies(GetSelectedVulkanPhysicalDevice());

	if (indices.m_queueFamilies[static_cast<u32>(QueueFamilies::GRAPHICS)] != indices.m_queueFamilies[static_cast<u32>(QueueFamilies::PRESENTATION)])
	{
		// An image is owned by one queue family at a time and ownership must be explicitly transferred before using it in another queue family. 
		// This option offers the best performance.
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = static_cast<u32>(indices.m_queueFamilies.size());
		vector<u32> queueFamilyIndices;
		for (u32 i = 0; i < static_cast<u32>(QueueFamilies::MAX); i++)
		{
			queueFamilyIndices.push_back(indices.m_queueFamilies[i].value());
		}
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	}
	else
	{
		// Images can be used across multiple queue families without explicit ownership transfers.
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0; // Optional
		swapChainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	// If you want any transformations, like rotating images by 90 degrees, specify here
	swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;

	// Simply ignore alpha channel here.
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	// Present mode
	swapChainCreateInfo.presentMode = presentMode;
	// if set to true, we dont care about the color of pixel that are obscured, for ex, because another window is in front of them.
	swapChainCreateInfo.clipped = VK_TRUE;

	// It is possible that a swap chain becomes invalid or unoptimized while the app is running because, for ex, the window is resized. In this 
	// case we have to recreate a swap chain from scratch and specify the old swap chain while creating a new one.For now it is set to NULL
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	//Create the swap chain
	if (vkCreateSwapchainKHR(m_logicalDevice, &swapChainCreateInfo, nullptr, &m_swapChain))
	{
		throw std::runtime_error("failed to create swap chain!");
		return false;
	}

	// Get just created swap chain images in an array
	vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, nullptr);
	m_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, m_swapChainImages.data());

	m_swapChainImageFormat = surfaceFormat.format;
	m_swapChainExtent = extents;

	return true;
}

//----------------------------------------------------------------------
#endif // VULKAN