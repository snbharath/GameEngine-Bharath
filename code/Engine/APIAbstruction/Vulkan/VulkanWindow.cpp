#if defined(VULKAN)


#include "VulkanValidationLayerWrapper.h"
#include "VulkanWindow.h"

#include <set>
#include <string>
#include <cstdint> // Necessary for UINT32_MAX
#include <algorithm> // Necessary for std::clamp
#include "FileOperations.h"

namespace GE {

const std::vector<const char*> c_deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VulkanWindow* VulkanWindow::s_pInstance = nullptr;

//----------------------------------------------------------------------

VulkanWindow::VulkanWindow()
	: m_pGLFWwindow(nullptr)
	, m_VulkanInstance(nullptr)
	, m_windowSurface(nullptr)
	, m_numberOfPhysicalDevices(0u)
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

//----------------------------------------------------------------------

VulkanWindow::~VulkanWindow()
{
	TerminateGEWindow();
}

//----------------------------------------------------------------------

bool VulkanWindow::InitWindow()
{
	// GLFW initialization
	int retval = glfwInit();
	
	// initialize with no context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	return retval;
}

//----------------------------------------------------------------------

bool VulkanWindow::CreateGEWindow()
{
	m_pGLFWwindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Window - Bharath", nullptr, nullptr);
	
	// if failed to create a glfw window then return
	if (m_pGLFWwindow == nullptr)
	{
		TerminateGEWindow();
		return false;
	}

	glfwMakeContextCurrent(m_pGLFWwindow);

	std::cout << "[VulkanWindow::CreateGEWindow] - Creating a Vulkan instance. Line: " << __LINE__ << std::endl;
	bool isSuccessful = CreateVulkanInstance();
	
	// Vulkan instance creation failed
	if (m_VulkanInstance == nullptr || !isSuccessful)
	{
		return false;
	}

	if (isSuccessful)
	{
		std::cout << "[VulkanWindow::CreateGEWindow] - Creating window surface. Line: " << __LINE__ << std::endl;
		isSuccessful = CreateWindowSurface();
	}

	if (isSuccessful)
	{
		std::cout << "[VulkanWindow::InitVulkanPhysicalDevice] - Selecting a physical device. Line: " << __LINE__ << std::endl;
		isSuccessful = SelectAPhysicalDevice(GetVulkanInstance());
	}

	if (isSuccessful)
	{
		std::cout << "[VulkanWindow::InitVulkanPhysicalDevice] - Creating a logical device. Line: " << __LINE__ << std::endl;
		isSuccessful = CreateLogicalDevice();
	}

	if (isSuccessful)
	{
		std::cout << "[VulkanWindow::InitVulkanPhysicalDevice] - Creating a Swap Chain. Line: " << __LINE__ << std::endl;
		isSuccessful = CreateSwapChain();
	}

	if (isSuccessful)
	{
		std::cout << "[VulkanWindow::InitVulkanPhysicalDevice] - Creating Image views. Line: " << __LINE__ << std::endl;
		isSuccessful = CreateImageViews();
	}

	if (isSuccessful)
	{
		std::cout << "[VulkanWindow::InitVulkanPhysicalDevice] - Creating Graphics Pipeline. Line: " << __LINE__ << std::endl;
		isSuccessful = CreateGraphicsPipeline();
	}

	return isSuccessful;
}

//----------------------------------------------------------------------

// returns false if it terminates
bool VulkanWindow::RenderFrameUpdate()
{
	if (!glfwWindowShouldClose(m_pGLFWwindow))
	{
		//handle input
		ProcessInput(m_pGLFWwindow);
		glfwPollEvents();

		return true;
	}

	return false;
}

//----------------------------------------------------------------------

void VulkanWindow::ProcessInput(GLFWwindow* pWindow)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		CloseGEWindow();
		TerminateGEWindow();
	}
}

//----------------------------------------------------------------------

void VulkanWindow::CloseGEWindow()
{
	glfwSetWindowShouldClose(m_pGLFWwindow, true);
}

//----------------------------------------------------------------------

void VulkanWindow::TerminateGEWindow()
{
	VulkanValidationLayerWrapper::DeleteInstance();
	
	if (m_logicalDevice != nullptr)
	{
		for (auto imageView : m_swapChainImageViews)
		{
			vkDestroyImageView(m_logicalDevice, imageView, nullptr);
		}

		if (m_swapChain != nullptr)
		{
			vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
		}

		vkDestroyDevice(m_logicalDevice, nullptr);
	}

	if (m_listOfPhysicalDevices != nullptr)
	{
		delete[] m_listOfPhysicalDevices;
	}

	// window surface instance
	if(m_windowSurface != nullptr)
	{
		vkDestroySurfaceKHR(m_VulkanInstance, m_windowSurface, nullptr);
	}

	assert(m_VulkanInstance != nullptr);
	vkDestroyInstance(m_VulkanInstance, nullptr);
	glfwDestroyWindow(m_pGLFWwindow);
	glfwTerminate();
}

//----------------------------------------------------------------------

VulkanWindow* VulkanWindow::GetInstance()
{
	if (!s_pInstance)
	{
		s_pInstance = new VulkanWindow();
	}

	return s_pInstance;
}

//----------------------------------------------------------------------

bool VulkanWindow::DeleteInstance()
{
	if (s_pInstance == nullptr)
	{
		return false;
	}

	delete s_pInstance;
	s_pInstance = nullptr;
	return true;
}

//----------------------------------------------------------------------

bool VulkanWindow::CreateVulkanInstance()
{
	// Vulkan application info
	VkApplicationInfo applicationInfo{};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "Vulkan Renderer";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = "Vulkan Physics Engine";
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	

	// Vulkan global and validation layers info.
	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	// Check if validation layers are enabled
	VulkanValidationLayerWrapper* pValidationLayerInstance = VulkanValidationLayerWrapper::GetInstance();
	if (pValidationLayerInstance != nullptr && pValidationLayerInstance->IsEnableValidationLayer())
	{
		if (pValidationLayerInstance->CheckValidationLayerSupport() == false)
		{
			std::cerr << "Validation layer requested, is not available" << std::endl;
			instanceCreateInfo.enabledLayerCount = 0;
		}
		else
		{
			auto validationLayers = pValidationLayerInstance->GetValidationLayers();
			instanceCreateInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		}
		pValidationLayerInstance->PrintAvailableValidationLayer();
	}


	// Vulkan extension support
	u32 glfwExtentionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount);

	instanceCreateInfo.enabledExtensionCount = glfwExtentionCount;
	instanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;
	instanceCreateInfo.enabledLayerCount = 0;

	bool isSuccessful = true;

	// Create Vulkan instance
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_VulkanInstance);

	if (VkResult::VK_SUCCESS == result)
	{
		std::cout << "Vulkan instance creation successful." << std::endl;
	}
	else if(VkResult::VK_ERROR_EXTENSION_NOT_PRESENT == result)
	{
		std::cout << "Vulkan extension not present." << std::endl;
		PrintVulkanAvailableExtensions(glfwExtensions, glfwExtentionCount);
		isSuccessful = false;
	}
	else if (VkResult::VK_ERROR_LAYER_NOT_PRESENT == result)
	{
		std::cout << "Validation Layer not present" << std::endl;
		isSuccessful = false;
	}
	else
	{
		std::cout << "Vulkan instance creation failed." << std::endl;
		isSuccessful = false;
	}

	return isSuccessful;
}

//----------------------------------------------------------------------

void VulkanWindow::PrintVulkanAvailableExtensions(const char ** pGivenExtensionNames, u32 givenExtensionCount)
{
	std::cout << "Given Extension names:" << std::endl;
	for (u32 i = 0; i < givenExtensionCount; i++)
	{
		std::cout << i << ". " << pGivenExtensionNames[i] << std::endl;
	}
	std::cout << std::endl;

	// check for extension support
	u32 vulkanExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, nullptr); // get the number of extension count available
	vector<VkExtensionProperties> extensions(vulkanExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, extensions.data()); // get all the extension details


	std::cout << "vulkan supported extensions:" << std::endl;
	for (const auto& extension : extensions)
	{
		std::cout << extension.extensionName << ", Version : " << extension.specVersion << std::endl;
	}
}

//----------------------------------------------------------------------

bool VulkanWindow::CreateWindowSurface()
{
	if (glfwCreateWindowSurface(GetVulkanInstance(), GetGLFWwindow(), nullptr, &m_windowSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
	return true;
}

//----------------------------------------------------------------------

bool VulkanWindow::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
	u32 deviceExtensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &deviceExtensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(deviceExtensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &deviceExtensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(c_deviceExtensions.begin(), c_deviceExtensions.end());
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

//----------------------------------------------------------------------

bool VulkanWindow::SelectAPhysicalDevice(const VkInstance& pInstance)
{
	vkEnumeratePhysicalDevices(pInstance, &m_numberOfPhysicalDevices, nullptr);

	if (m_numberOfPhysicalDevices == 0)
	{
		std::cerr << "[ERROR] Well, there are no physical devices\n";
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
			std::cout << "Select from the following list of devices:\n";
			for (u32 i = 0; i < m_numberOfPhysicalDevices; i++)
			{
				std::cout << "[" << i << "]" << deviceNames[i] << std::endl;
			}

			int numberEntered = static_cast<int>(m_numberOfPhysicalDevices);
			while (!(numberEntered >= 0 && numberEntered < static_cast<int>(m_numberOfPhysicalDevices)))
			{
				std::cout << "Select a device of type \'1\'(Integrated GPU) or \'2\'(Discrete GPU) : ";
				std::cin >> numberEntered;
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
	std::cout << "Selected : " << deviceProperties.deviceName << std::endl;

	return true;
}

//----------------------------------------------------------------------

bool VulkanWindow::IsDeviceSuitable(VkPhysicalDevice device)
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

	// Since Raspberry pi has integrated GPU, approve the device type in the device selection.
#if defined(PC)
	bool isDeviceTypeApproved = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
#else // rpi(VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) or linux-VM (VK_PHYSICAL_DEVICE_TYPE_CPU)
	bool isDeviceTypeApproved = (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU || deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU);
#endif

	return (
		(isDeviceTypeApproved) &&
		(deviceFeatures.geometryShader) &&
		(indices.isComplete()) &&
		(extensionsSupported) &&
		(isSwapChainAdequate));
}

//----------------------------------------------------------------------

void VulkanWindow::GetListOfAllDeviceNames(VkPhysicalDevice const* devices, u32 numberOfDevices, vector<string>& deviceNames)
{
	VkPhysicalDeviceProperties deviceProperties;
	for (u32 i = 0; i < numberOfDevices; i++)
	{
		char buf[128];
		memset(buf, 0, 128);

		vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
#if defined(PC)
		sprintf_s(buf, "Name : %s - type : %d", deviceProperties.deviceName, static_cast<int>(deviceProperties.deviceType));
#elif defined(LINUX)
		sprintf(buf, "Name : %s - type : %d", deviceProperties.deviceName, static_cast<int>(deviceProperties.deviceType));
#endif

		deviceNames[i] = buf;
	}
}

//----------------------------------------------------------------------

QueueFamilyIndices VulkanWindow::FindQueueFamilies(VkPhysicalDevice physicalDevice)
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

bool VulkanWindow::CreateLogicalDevice()
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
		logicalDeviceCreateInfo.enabledLayerCount = 0;
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

SwapChainSupportDetails VulkanWindow::QuerySwapChainSupport(VkPhysicalDevice physicalDevice)
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

VkSurfaceFormatKHR VulkanWindow::ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableSurfaceFormats)
{
	for (const auto& availableFormat : availableSurfaceFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	std::cout << "[Warning] Didn't find the required swap surface format, selecting one from the list" << std::endl;
	return availableSurfaceFormats[0];
}

//----------------------------------------------------------------------

VkPresentModeKHR VulkanWindow::ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR)
		{
			return availablePresentMode;
		}
	}

	std::cout << "[Warning] Didn't find the required present mode, selecting the one that is guaranteed to be available" << std::endl;
	return VK_PRESENT_MODE_FIFO_KHR;
}

//----------------------------------------------------------------------

VkExtent2D VulkanWindow::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
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

bool VulkanWindow::CreateSwapChain()
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

bool VulkanWindow::CreateImageViews()
{
	// Resize to fit all the VkImage objects
	m_swapChainImageViews.resize(m_swapChainImages.size());

	bool ret = true;

	for (u32 i = 0; i < m_swapChainImages.size(); i++)
	{
		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = m_swapChainImages[i];

		// this allows us to treat images as 1D, 2D, 3D or cube maps
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = m_swapChainImageFormat;

		// setup components
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		// The subsource range field describes what the images's purpose is and which part of image should
		// be accessed. we use our images as color targets without any mipmapping or multiple layers
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(m_logicalDevice, &imageViewCreateInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image views!");
			ret &= false;
		}
	}

	return ret;
}

//----------------------------------------------------------------------

void VulkanWindow::CreateShaderModule(VkShaderModule& shaderModule, const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const u32*>(code.data());

	if (vkCreateShaderModule(m_logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create shader module\n");
	}
}

//----------------------------------------------------------------------

bool VulkanWindow::CreateGraphicsPipeline()
{
	bool ret = true;

	// Read Vertex shader
	auto vertShaderFile = FileOperations::ReadFile("Shaders/vert.spv");
	auto fragShaderFile = FileOperations::ReadFile("Shaders/frag.spv");

	// Shader module creation. a thin wrapper aroung the shader bytecode array
	VkShaderModule vertShaderModule;
	CreateShaderModule(vertShaderModule, vertShaderFile);
	VkShaderModule fragShaderModule;
	CreateShaderModule(fragShaderModule, fragShaderFile);

	// Shader stage creation
	// Vertex shader 
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; // Specifying which pipeline stage the shader is going to be used
	vertShaderStageInfo.module = vertShaderModule; // bytecode of the shader
	// Specify the entry point to the shader function, which is "main". However, you can have multiple entry point in the shader and specify the entry point in here
	vertShaderStageInfo.pName = "main";

	// Fragment shader 
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; // Specifying which pipeline stage the shader is going to be used
	fragShaderStageInfo.module = fragShaderModule; // bytecode of the shader
	// Specify the entry point to the shader function, which is "main". However, you can have multiple entry point in the shader and specify the entry point in here
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	vkDestroyShaderModule(m_logicalDevice, vertShaderModule, nullptr);
	vkDestroyShaderModule(m_logicalDevice, fragShaderModule, nullptr);
	return ret;
}

} // namespace GE
//----------------------------------------------------------------------
#endif // VULKAN