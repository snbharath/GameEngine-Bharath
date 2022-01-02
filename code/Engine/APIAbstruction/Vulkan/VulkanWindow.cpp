#if defined(VULKAN)

#include "VulkanValidationLayerWrapper.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanWindow.h"

using namespace GE;
using namespace std;

VulkanWindow* VulkanWindow::s_pInstance = nullptr;

//----------------------------------------------------------------------

VulkanWindow::VulkanWindow()
	: m_pGLFWwindow(nullptr)
	, m_VulkanInstance(nullptr)
	, m_pVulkanPhysicalDevice(nullptr)
	, m_windowSurface(nullptr)
{
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

	m_pVulkanPhysicalDevice = VulkanPhysicalDevice::GetInstance(); // creates one
	assert(m_pVulkanPhysicalDevice != nullptr);

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

	cout << "[VulkanWindow::CreateGEWindow] - Creating a Vulkan instance. Line: " << __LINE__ << endl;
	bool isSuccessful = CreateVulkanInstance();
	
	// Vulkan instance creation failed
	if (m_VulkanInstance == nullptr || !isSuccessful)
	{
		return false;
	}

	cout << "[VulkanWindow::CreateGEWindow] - Creating window surface. Line: " << __LINE__ << endl;
	if (isSuccessful)
	{
		isSuccessful = CreateWindowSurface();
	}

	if (isSuccessful)
	{
		cout << "[VulkanWindow::CreateGEWindow] - Initializing Vulkan Physical device. Line: " << __LINE__ << endl;
		// Perform physical device selection
		isSuccessful = m_pVulkanPhysicalDevice->InitVulkanPhysicalDevice();
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
	VulkanPhysicalDevice::DeleteInstance();

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
			cerr << "Validation layer requested, is not available" << endl;
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
		std::cout << "Vulkan instance creation successful." << endl;
	}
	else if(VkResult::VK_ERROR_EXTENSION_NOT_PRESENT == result)
	{
		std::cout << "Vulkan extension not present." << endl;
		PrintVulkanAvailableExtensions(glfwExtensions, glfwExtentionCount);
		isSuccessful = false;
	}
	else if (VkResult::VK_ERROR_LAYER_NOT_PRESENT == result)
	{
		std::cout << "Validation Layer not present" << endl;
		isSuccessful = false;
	}
	else
	{
		std::cout << "Vulkan instance creation failed." << endl;
		isSuccessful = false;
	}

	return isSuccessful;
}

//----------------------------------------------------------------------

void VulkanWindow::PrintVulkanAvailableExtensions(const char ** pGivenExtensionNames, u32 givenExtensionCount)
{
	std::cout << "Given Extension names:" << endl;
	for (u32 i = 0; i < givenExtensionCount; i++)
	{
		std::cout << i << ". " << pGivenExtensionNames[i] << endl;
	}
	std::cout << endl;

	// check for extension support
	u32 vulkanExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, nullptr); // get the number of extension count available
	vector<VkExtensionProperties> extensions(vulkanExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, extensions.data()); // get all the extension details


	std::cout << "vulkan supported extensions:" << endl;
	for (const auto& extension : extensions)
	{
		std::cout << extension.extensionName << ", Version : " << extension.specVersion << endl;
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

#endif // VULKAN