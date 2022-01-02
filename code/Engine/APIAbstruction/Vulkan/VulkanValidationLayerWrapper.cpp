#if defined(VULKAN)

#include "../EngineCommonIncludes.h"
#include "VulkanValidationLayerWrapper.h"

namespace GE {

VulkanValidationLayerWrapper * VulkanValidationLayerWrapper::s_pInstance = nullptr;

//----------------------------------------------------------------------

VulkanValidationLayerWrapper* VulkanValidationLayerWrapper::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new VulkanValidationLayerWrapper();
	}

	return s_pInstance;
}

//----------------------------------------------------------------------

bool VulkanValidationLayerWrapper::DeleteInstance()
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

VulkanValidationLayerWrapper::VulkanValidationLayerWrapper()
#if defined(DEBUG)
	: m_cEnableValidationLayers(true)
#else
	: m_cEnableValidationLayers(false)
#endif
{
}

//----------------------------------------------------------------------

bool VulkanValidationLayerWrapper::CheckValidationLayerSupport()
{
	bool anyLayerFound = true;
	u32 validationLayerCount;
	vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);
	vector<VkLayerProperties> availableValidationLayer(validationLayerCount);
	vkEnumerateInstanceLayerProperties(&validationLayerCount, availableValidationLayer.data());

	// If any of the validation layers in m_cValidationLayers not found then return false
	for (const char* layerName : m_cValidationLayers)
	{
		bool checkLayer = false;
		for (const auto& layerProperties : availableValidationLayer)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				checkLayer = true;
				break;
			}
		}
		anyLayerFound &= checkLayer;
	}

	return anyLayerFound;
}

//----------------------------------------------------------------------

void VulkanValidationLayerWrapper::PrintAvailableValidationLayer()
{
	u32 validationLayerCount;
	vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);
	vector<VkLayerProperties> availableValidationLayer(validationLayerCount);
	vkEnumerateInstanceLayerProperties(&validationLayerCount, availableValidationLayer.data());

	cout << "Available Validation Layers:" << endl;
	for (const auto& layerProperties : availableValidationLayer)
	{
		cout << layerProperties.layerName << endl;
	}

	cout << endl;
}

const vector<const char*>& VulkanValidationLayerWrapper::GetValidationLayers() const
{
	return m_cValidationLayers;
}
//----------------------------------------------------------------------

}

#endif