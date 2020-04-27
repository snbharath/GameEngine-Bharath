#if defined(VULKAN)

#include <iostream>
#include <vector>
#include <cassert>
#include "VulkanWindow.h"
#include "VulkanValidationLayerWrapper.h"

using namespace GE;
using namespace std;

VulkanWindow* VulkanWindow::s_pInstance = nullptr;

//----------------------------------------------------------------------

VulkanWindow::VulkanWindow()
	: m_pGLFWwindow(nullptr)
	, m_VulkanInstance(nullptr)
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

	CreateVulkanInstance();
	return true;
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

void VulkanWindow::CreateVulkanInstance()
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
			cerr << "Validation layer requested, its not available" << endl;
			instanceCreateInfo.enabledLayerCount = 0;
		}
		else
		{
			auto validationLayers = pValidationLayerInstance->GetValidationLayers();
			instanceCreateInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		}
		// pValidationLayerInstance->PrintAvailableValidationLayer();
	}

	// Vulkan extension support
	u32 glfwExtentionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount);

	instanceCreateInfo.enabledExtensionCount = glfwExtentionCount;
	instanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;
	instanceCreateInfo.enabledLayerCount = 0;

	// Create Vulkan instance
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_VulkanInstance);

	if (VkResult::VK_SUCCESS == result)
	{
		cout << "Vulkan instance creation successful." << endl; 
	}
	else if(VkResult::VK_ERROR_EXTENSION_NOT_PRESENT == result)
	{
		cout << "Vulkan extension not present." << endl;
		PrintVulkanAvailableExtensions(glfwExtensions, glfwExtentionCount);
	}
	else if (VkResult::VK_ERROR_LAYER_NOT_PRESENT == result)
	{
		cout << "Validation Layer not present" << endl;
	}
	else
	{
		cout << "Vulkan instance creation failed." << endl;
	}
}

//----------------------------------------------------------------------

void VulkanWindow::PrintVulkanAvailableExtensions(const char ** pGivenExtensionNames, u32 givenExtensionCount)
{
	cout << "Given Extension names:" << endl;
	for (u32 i = 0; i < givenExtensionCount; i++)
	{
		cout << i << ". " << pGivenExtensionNames[i] << endl;
	}
	cout << endl;

	// check for extension support
	u32 vulkanExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, nullptr); // get the number of extension count available
	vector<VkExtensionProperties> extensions(vulkanExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, extensions.data()); // get all the extension details


	cout << "vulkan supported extensions:" << endl;
	for (const auto& extension : extensions)
	{
		cout << extension.extensionName << ", Version : " << extension.specVersion << endl;
	}
}

//----------------------------------------------------------------------

#endif // VULKAN